#ifndef _GUI_H_
#define _GUI_H_

class gameInterface
	{
	private:
	GLuint texture;
	int nWidth, nHeight;

	public:
	gameInterface(string imgUrl)
		{

		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(imgUrl.c_str(), 0),imgUrl.c_str());
		FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
		nWidth = FreeImage_GetWidth(pImage);
		nHeight = FreeImage_GetHeight(pImage);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
		FreeImage_Unload(bitmap);
		FreeImage_Unload(pImage);
		}

	void draw()
		{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
					
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_COLOR_MATERIAL);
		

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-0.70,-0.40,0);
		glRotatef(180, 0, 0, 1);
		glScalef(0.001,0.0015,1);

		glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
			glTexCoord2f(0,1);
			glVertex3f(0,0,0);

			glTexCoord2f(1,1);
			glVertex3f(nWidth,0,0);

			glTexCoord2f(1,0);
			glVertex3f(nWidth, nHeight,0);

			glTexCoord2f(0,0);
			glVertex3f(0,nHeight,0);

		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
		}
    };

#endif