#ifndef _VIDEO_H_
#define _VIDEO_H_

extern "C" 
	{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	}
#include <stdio.h>
#include <string>
using namespace std;

class videoDecode
	{
	private:
	bool firstRendered;
	bool finishedRendering;
	bool frameRendered;
	AVFormatContext *pFormatCtx;
	int             i, videoStream;
	AVCodecContext  *pCodecCtx;
	AVCodec         *pCodec;
	AVFrame         *pFrame; 
	AVFrame         *pFrameRGB;
	AVPacket        packet;
	int             frameFinished;
	int             numBytes;
	uint8_t         *buffer;
	GLuint texture;
	int width, height;

	AVDictionary *optionsDict;
	struct SwsContext *sws_ctx;


	public:
	videoDecode(string fileName) 
		{
 		pFormatCtx = NULL;
		pCodecCtx = NULL;
		pCodec = NULL;
		pFrame = NULL; 
		pFrameRGB = NULL;
		buffer = NULL;
		optionsDict = NULL;
		sws_ctx = NULL;
		firstRendered = true;
		finishedRendering = false;

		//generate a texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Register all formats and codecs
		av_register_all();
  
		// Open video file
		if(avformat_open_input(&pFormatCtx, fileName.c_str(), NULL, NULL)!=0){} // Couldn't open file
  
		// Retrieve stream information
		if(avformat_find_stream_info(pFormatCtx, NULL)<0) {} // Couldn't find stream information
  
		// Dump information about file onto standard error
		//av_dump_format(pFormatCtx, 0, fileName.c_str(), 0);
  
		// Find the first video stream
		videoStream=-1;
		for(i=0; i<pFormatCtx->nb_streams; i++)
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) 
			{
			videoStream=i;
			break;
			}
		if(videoStream==-1){} // Didn't find a video stream
  
		// Get a pointer to the codec context for the video stream
		pCodecCtx=pFormatCtx->streams[videoStream]->codec;
  
		// Find the decoder for the video stream
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		if(pCodec==NULL) 
			{
			fprintf(stderr, "Unsupported codec!\n");
			//return -1; // Codec not found
			}
		// Open codec
		if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0) {}// Could not open codec
  
		// Allocate video frame
		pFrame=avcodec_alloc_frame();
  
		// Allocate an AVFrame structure
		pFrameRGB=avcodec_alloc_frame();
		if(pFrameRGB==NULL) {}
  
		// Determine required buffer size and allocate buffer
		numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
		buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

		sws_ctx =
		sws_getContext
		(
			pCodecCtx->width,
			pCodecCtx->height,
			pCodecCtx->pix_fmt,
			pCodecCtx->width,
			pCodecCtx->height,
			PIX_FMT_RGB24,
			SWS_BICUBIC,
			NULL,
			NULL,
			NULL
		);
  
		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
		width = pCodecCtx->width;
		height = pCodecCtx->height;
		}

	void getNextFrame()
		{
		if(!finishedRendering)
		{
		frameRendered = false;
		while(av_read_frame(pFormatCtx, &packet)>=0) 
			{
			// Is this a packet from the video stream?
			if(packet.stream_index==videoStream) 
				{
				// Decode video frame
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
                    
				// Did we get a video frame?
				if(frameFinished) 
					{
					frameRendered = true;
					// Convert the image from its native format to RGB
					sws_scale
					(
					sws_ctx,
					pFrame->data,
					pFrame->linesize,
					0,
					pCodecCtx->height,
					pFrameRGB->data,
					pFrameRGB->linesize
					);
        
				if(firstRendered)
					{
					glBindTexture(GL_TEXTURE_2D,texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pCodecCtx->width,pCodecCtx->height ,0,GL_RGB,GL_UNSIGNED_BYTE, pFrameRGB->data[0]);

					firstRendered = false;
					}
				else
					{
					glBindTexture(GL_TEXTURE_2D,texture);
					glTexSubImage2D(GL_TEXTURE_2D,0,0,0,pCodecCtx->width,pCodecCtx->height,GL_RGB,GL_UNSIGNED_BYTE,pFrameRGB->data[0]);
					}
					

					glEnable(GL_TEXTURE_2D);
					//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					
					glBindTexture(GL_TEXTURE_2D, texture);
					glEnable(GL_COLOR_MATERIAL);
					glBegin(GL_QUADS);

					glColor3f(1.0,1.0,1.0);
						glTexCoord2f(0,1);
						glVertex3f(0,0,0);

						glTexCoord2f(1,1);
						glVertex3f(pCodecCtx->width,0,0);

						glTexCoord2f(1,0);
						glVertex3f(pCodecCtx->width, pCodecCtx->height,0);

						glTexCoord2f(0,0);
						glVertex3f(0,pCodecCtx->height,0);

					glEnd();
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_COLOR_MATERIAL);


					av_free_packet(&packet);
					break;
					}
				}

			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
			}
		if(!frameRendered) 
			{
			cleanup();
			}
		}
	}
	void cleanup()
		{
		if(!finishedRendering)
			{
			finishedRendering  = true;
			// Free the RGB image
			av_free(buffer);
			av_free(pFrameRGB);
  
			// Free the YUV frame
			av_free(pFrame);
  
			// Close the codec
			avcodec_close(pCodecCtx);
  
			// Close the video file
			avformat_close_input(&pFormatCtx);
			}
		}

	int getWidth(){return this->width;}
	int getHeight(){return this->height;}
	bool finished() {return this->finishedRendering;}
	};
#endif