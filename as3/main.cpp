//// CS184 Assignment 3
//#include <vector>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <fstream>
//#include <cmath>
//
//#ifdef _WIN32
//#include <windows.h>
//#else
//#include <sys/time.h>
//#endif
//
//#ifdef OSX
//#include <GLUT/glut.h>
//#include <OpenGL/glu.h>
//#else
//#include <GL/glew.h>
//#include <GL/glut.h>
//#include <GL/glu.h>
//#include <GL/glext.h>
//#endif
//
//
//#include <math.h>
//#include "vect.h"
//#include "bezier.h"
//#include "triangle.h"
//#include "shaders.h"
//
//
//#ifdef _WIN32
//static DWORD lastTime;
//#else
//static struct timeval lastTime;
//#endif
//
//#define PI 3.14159265
//
//using namespace std;
//
////****************************************************
//// Some Classes
////****************************************************
//class Viewport
//{
//public:
//	int w, h; // width and height
//};
//
//
////****************************************************
//// Global Variables
////****************************************************
//Viewport viewport;
//vector<int> curvecount;
//float epsilon = 0.001;
//float step = 0;
//float parameter = 0;
//vector<bezPatch> bezPatches;
//vector<triangle> triangles;
//
////toon shaders
//GLuint toonVertex, toonFragment, toonShader;
//
//
////flags
//bool wmode = 0;
//bool shiftpressed = 0;
//bool adaptiveMode = 0;
//bool smoothShading = 0;
//bool toonShading = 0;
//vector<float> rotatex, rotatey, rotatez, translatex, translatey, translatez, scale, maxz;
//
////extra credit variables
//vector<string> file;
//int totalFiles = 1;
//int noOfObjFiles = 0, noOfBezFiles = 0;
//int selectedObject = 1;
//
//vector<vect> objVertices;
//vector<vect> normalVertices;
//vector<triangle> ObjTriangles;
//vector<triangle> ObjNormals;
//
//vector<int> startpt;
//vector<int> endpt;
//
//bool drawObj = false;
//vector<float> maxObjcoordinate;
//
//struct material
//	{
//	GLfloat diffuse[4];
//	GLfloat specular[4];
//	GLfloat ambient[4];
//	GLfloat shininess[1];
//	};
//
////preset locations for multiple objects
//
//float transx[26], transy[26];
//int transcount = 1;
//
////materials
//int currentMaterial = 0, totalMaterial = 24;
//string materialName[25] = {"emerald","jade","copper","obsidian","pearl","ruby","turquoise","brass","bronze","chrome","gold","silver","black plastic","cyan plastic","green plastic","red plastic","white plastic","yellow plastic","black rubber","cyan rubber","green rubber","red rubber","white rubber","yellow rubber"};
//
//material getMaterial(int id)
//	{
//	//thanks to http://devernay.free.fr/cours/opengl/materials.html
//	
//	material emerald = {{0.07568,0.61424,0.07568, 1.0f},{0.633,0.727811,0.633, 1.0f}, {0.0215,0.1745,0.0215, 1.0f}, {76.8}};
//	material jade = {{0.135,0.2225,0.1575},{0.54,0.89,0.63},{0.316228,0.316228, 0.316228},{0.1*128}};
//	material copper = {{0.7038,0.27048,0.0828, 1.0f},{0.256777,0.137622,0.086014, 1.0f}, {0.19125,0.0735,0.0225, 1.0f},{12.8}};
//	material obsidian =	{{0.05375,0.05,0.06625},{0.18275,0.17,0.22525},{0.332741,0.328634,0.346435},{0.3*128}};
//	material pearl = {{0.25,0.20725,0.20725},{1,0.829,0.829},{0.296648,0.296648,0.296648},{0.088*128}};
//	material ruby =	{{0.1745,0.01175,0.01175},{0.61424,0.04136,0.04136},{0.727811,0.626959,	0.626959},{0.6*128}};
//	material turquoise = {{0.1,0.18725,0.1745},{0.396,0.74151,0.69102},{0.297254,0.30829,0.306678},{0.1*128}};
//	material brass = {{0.329412,0.223529,0.027451},{0.780392, 0.568627,	0.113725},{	0.992157,0.941176,0.807843},{0.21794872*128}};
//	material bronze	= {{0.2125,0.1275,0.054},{0.714,0.4284,0.18144},{0.393548,0.271906,0.166721},{0.2*128}};
//	material chrome	= {{0.25,0.25,0.25},{0.4,0.4,0.4},{0.774597,0.774597,0.774597},{0.6*128}};
//	material gold = {{0.24725,0.1995,0.0745},{0.75164,0.60648,0.22648},{0.628281,0.555802,0.366065},{0.4*128}};
//	material silver	= {{0.19225,0.19225,0.19225},{0.50754,0.50754,0.50754},{0.508273,0.508273,0.508273},{0.4*128}};
//	material black_plastic	= {{0.0,0.0,0.0},{0.01,0.01,0.01},{0.50,0.50,0.50},{0.25*128}};
//	material cyan_plastic	= {{0.0,0.1,0.06},{0.0,0.50980392,0.50980392},{0.50196078,0.50196078,0.50196078},{0.25*128}};
//	material green_plastic	= {{0.0,0.0,0.0},{0.1,0.35,0.1},{0.45,0.55,0.45},{0.25*128}};
//	material red_plastic	= {{0.0,0.0,0.0},{0.5,0.0,0.0},{0.7,0.6,0.6},{0.25*128}};
//	material white_plastic	= {{0.0,0.0,0.0},{0.55,0.55,0.55},{0.70,0.70,0.70},{0.25*128}};
//	material yellow_plastic	= {{0.0,0.0,0.0},{0.5,0.5,0.0},{0.60,0.60,0.50},{0.25*128}};
//	material black_rubber	= {{0.02,0.02,0.02},{0.01,0.01,0.01},{0.4,0.4,0.4},{10}};
//	material cyan_rubber	= {{0.0,0.05,0.05},{0.4,0.5,0.5},{0.04,0.7,0.7},{.078125*128}};
//	material green_rubber	= {{0.0,0.05,0.0},{0.4,0.5,0.4},{0.04,0.7,0.04},{.078125*128}};
//	material red_rubber	= {{0.05,0.0,0.0},{0.5,0.4,0.4},{0.7,0.04,0.04},{.078125*128}};
//	material white_rubber =	{{0.05,0.05,0.05},{0.5,0.5,0.5},{0.7,0.7,0.7},{.078125*128}};
//	material yellow_rubber = {{0.05,0.05,0.0},{0.5,0.5,0.4},{0.7,0.7,0.04},{.078125*128}};
//
//	switch(currentMaterial)
//		{
//		case 0:return emerald;break;
//		case 1:return jade;break;
//		case 2:return copper;break;
//		case 3:return obsidian;break;
//		case 4:return pearl;break;
//		case 5:return ruby;break;
//		case 6:return turquoise;break;
//		case 7:return brass;break;
//		case 8:return bronze;break;
//		case 9:return chrome;break;
//		case 10:return gold;break;
//		case 11:return silver;break;
//		case 12:return black_plastic;break;
//		case 13:return cyan_plastic;break;
//		case 14:return green_plastic;break;
//		case 15:return red_plastic;break;
//		case 16:return white_plastic;break;
//		case 17:return yellow_plastic;break;
//		case 18:return black_rubber;break;
//		case 19:return cyan_rubber;break;
//		case 20:return green_rubber;break;
//		case 21:return red_rubber;break;
//		case 22:return white_rubber;break;
//		case 23:return yellow_rubber;break;
//		}
//	return emerald;
//	}
//
////****************************************************
//// reshape viewport if the window is resized
////****************************************************
//void myReshape(int w, int h)
//{
//	viewport.w = w;
//	viewport.h = h;
//    
//	glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();                // loading the identity matrix for the screen
//    
//	glOrtho(-5, 5, -5, 5, -500, 500);    // resize type = stretch
//    
//	//------------------------------------------------------------
//}
//
////****************************************************
//// sets the window up
////****************************************************
//void initScene()
//	{
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent
//	myReshape(viewport.w,viewport.h);
//	rotatex.push_back(0), rotatey.push_back(0), rotatez.push_back(0), translatex.push_back(0), translatey.push_back(0), translatez.push_back(0), scale.push_back(1.0), maxz.push_back(0);
//	maxObjcoordinate.push_back(0);
//	startpt.push_back(0);
//	endpt.push_back(0);
//
//	for(float x = -2.5; x<=2.5; x+=2.5)
//		{
//		for(float y = -2.5; y<=2.5; y+=2.5)
//			{
//			if(x!=0)
//				{
//				transy[transcount] = x; transx[transcount] = y;
//				transcount++;
//				}
//			}
//		}
//	}
//
////****************************************************
//// functions
////****************************************************
//
//bool parameterCheck(vect onCurve, vect onPatch) 
//	{
//    float distance = sqrt(pow(onCurve.getX() - onPatch.getX(), 2) + pow(onCurve.getY() - onPatch.getY(), 2) + pow(onCurve.getZ() - onPatch.getZ(), 2));
//    return (abs(distance) < parameter);
//	}
//
//void splitTriangle(bezPatch patch, triangle t, int depth)
//{
//    
//    float p12u = (t.p1u + t.p2u) * 0.5;
//    float p12v = (t.p1v + t.p2v) * 0.5;
//    float p13u = (t.p1u + t.p3u) * 0.5;
//    float p13v = (t.p1v + t.p3v) * 0.5;
//    float p23u = (t.p2u + t.p3u) * 0.5;
//    float p23v = (t.p2v + t.p3v) * 0.5;
//    
//    vect curveP12 = patch.bezpatchinterp(patch,p12u,p12v).pt;
//    vect curveP13 = patch.bezpatchinterp(patch,p13u,p13v).pt;
//    vect curveP23 = patch.bezpatchinterp(patch,p23u,p23v).pt;
//    
//    bool check12 = parameterCheck(curveP12, t.mid12());
//    bool check13 = parameterCheck(curveP13, t.mid13());
//    bool check23 = parameterCheck(curveP23, t.mid23());
//    
//	if(depth == 0)
//    {
//		triangles.push_back(t);
//		return;
//    }
//    
//    if (check12 && check13 && check23) { // Passes all checks; Can be pushed back
//        
//        triangles.push_back(t);
//        
//    } else if (check12 && check13 && !check23) { // Edge between 2 and 3 fails
//        
//        triangle newT1(t.p1, t.p2, curveP23);
//        newT1.setUV(t.p1u, t.p1v, t.p2u, t.p2v, p23u, p23v);
//        
//        triangle newT2(t.p1, curveP23, t.p3);
//        newT2.setUV(t.p1u, t.p1v, p23u, p23v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        
//    } else if (check12 && !check13 && check23) { // Edge between 1 and 3 fails
//        
//        triangle newT1(t.p1, t.p2, curveP13);
//        newT1.setUV(t.p1u, t.p1v, t.p2u, t.p2v, p13u, p13v);
//        
//        triangle newT2(curveP13, t.p2, t.p3);
//        newT2.setUV(p13u, p13v, t.p2u, t.p2v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        
//    } else if (!check12 && check13 && check23) { // Edge between 1 and 2 fail
//        
//        triangle newT1(t.p1, curveP12, t.p3);
//        newT1.setUV(t.p1u, t.p1v, p12u, p12v, t.p3u, t.p3v);
//        
//        triangle newT2(curveP12, t.p2, t.p3);
//        newT2.setUV(p12u, p12v, t.p2u, t.p2v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        
//    } else if (check12 && !check13 && !check23) { // Edges between 1 and 3, 2 and 3 fail
//        
//        triangle newT1(curveP13, t.p2, curveP23);
//        newT1.setUV(p13u, p13v, t.p2u, t.p2v, p23u, p23v);
//        
//        triangle newT2(t.p1, t.p2, curveP13);
//        newT2.setUV(t.p1u, t.p1v, t.p2u, t.p2v, p13u, p13v);
//        
//        triangle newT3(curveP13, curveP23, t.p3);
//        newT3.setUV(p13u, p13v, p23u, p23v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        splitTriangle(patch, newT3, depth - 1);
//        
//    } else if (!check12 && check13 && !check23) { // Edges between 1 and 2, 2 and 3 fail
//        
//        triangle newT1(curveP12, curveP23, t.p3);
//        newT1.setUV(p12u, p12v, p23u, p23v, t.p3u, t.p3v);
//        
//        triangle newT2(t.p1, curveP12, t.p3);
//        newT2.setUV(t.p1u, t.p1v, p12u, p12v, t.p3u, t.p3v);
//        
//        triangle newT3(curveP12, t.p2, curveP23);
//        newT3.setUV(p12u, p12v, t.p2u, t.p2v, p23u, p23v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        splitTriangle(patch, newT3, depth - 1);
//        
//    } else if (!check12 && !check13 && check23) {  // Edges between 1 and 2, 1 and 3 fail
//        
//        triangle newT1(curveP13, curveP12, t.p3);
//        newT1.setUV(p13u, p13v, p12u, p12v, t.p3u, t.p3v);
//        
//        triangle newT2(t.p1, curveP12, curveP13);
//        newT2.setUV(t.p1u, t.p1v, p12u, p12v, p13u, p13v);
//        
//        triangle newT3(curveP12, t.p2, t.p3);
//        newT3.setUV(p12u, p12v, t.p2u, t.p2v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        splitTriangle(patch, newT3, depth - 1);
//        
//    } else if (!check12 && !check13 && !check23) {  // All edges fail
//        
//        triangle newT1(curveP12, curveP23, curveP13);
//        newT1.setUV(p12u, p12v, p23u, p23v, p13u, p13v);
//        
//        triangle newT2(t.p1, curveP12, curveP13);
//        newT2.setUV(t.p1u, t.p1v, p12u, p12v, p13u, p13v);
//        
//        triangle newT3(curveP12, t.p2, curveP23);
//        newT3.setUV(p12u, p12v, t.p2u, t.p2v, p23u, p23v);
//        
//        triangle newT4(curveP13, curveP23, t.p3);
//        newT4.setUV(p13u, p13v, p23u, p23v, t.p3u, t.p3v);
//        
//        splitTriangle(patch, newT1, depth - 1);
//        splitTriangle(patch, newT2, depth - 1);
//        splitTriangle(patch, newT3, depth - 1);
//        splitTriangle(patch, newT4, depth - 1);
//        
//    }
//}
//
//void drawPatch(bezPatch patch, float step)
//{
//    if (!adaptiveMode)
//		{
//        // Subdivision Mode
//        int numdiv = (int)(1 / step);
//		if (fmod(1,step) != 0)  numdiv+=1;
//		
//        bezPt *pts = new bezPt[(numdiv+1) * (numdiv+1)];
//        
//        for (int iv = 0; iv <= numdiv; iv++)
//			{
//            float v = min(1.0f, (float)iv * step);
//            for (int iu = 0; iu <= numdiv; iu++)
//				{
//                float u = min(1.0f,(float)iu * step);
//                //save pts
//                pts[iv*(numdiv+1) + iu] = patch.bezpatchinterp(patch,u,v);
//				}
//			}
//        
//        for (int iv = 1; iv <= numdiv; iv++)
//			{
//            for (int iu = 1; iu <= numdiv; iu++)
//				{
//                //points
//                vect tl = pts[(iv-1)*(numdiv+1) + (iu - 1)].pt;
//                vect tr = pts[(iv-1)*(numdiv+1) + (iu)].pt;
//                vect bl = pts[(iv)*(numdiv+1) + (iu - 1)].pt;
//                vect br = pts[(iv)*(numdiv+1)+ (iu)].pt;
//                //gradients
//                vect gtl = pts[(iv-1)*(numdiv+1) + (iu - 1)].gradient;
//                vect gtr = pts[(iv-1)*(numdiv+1) + (iu)].gradient;
//                vect gbl = pts[(iv)*(numdiv+1) + (iu - 1)].gradient;
//                vect gbr = pts[(iv)*(numdiv+1)+ (iu)].gradient;
//                
//                
//				vect tmp1 = ((bl - tl) * (br - tl)).getUnitVector();
//				vect tmp2 = ((br - tl) * (tl - tl)).getUnitVector();
//				//normal correction
//				float anglegtl = acos(gtl.getUnitVector()^tmp1.getUnitVector());
//				float anglegtr = acos(gtr.getUnitVector()^tmp2.getUnitVector());
//				float anglegbl = acos(gbl.getUnitVector()^tmp1.getUnitVector());
//				float anglegbr = acos(gbr.getUnitVector()^tmp2.getUnitVector());
//
//				if(gtl.getMagnitude()==0) gtl = tmp1;
//				else if(!(anglegtl < PI/2 && anglegtl >= 0)) gtl = -1*gtl;
//				if(gtr.getMagnitude()==0) gtr = tmp2;
//				else if(!(anglegtr < PI/2 && anglegtr >= 0)) gtr = -1*gtr;
//				if(gbl.getMagnitude()==0) gbl = tmp1;
//				else if(!(anglegbl < PI/2 && anglegbl >= 0)) gbl = -1*gbl;
//				if(gbr.getMagnitude()==0) gbr = tmp2;
//				else if(!(anglegbr < PI/2 && anglegbr >= 0)) gbr = -1*gbr;
//
//                vect normal = ((br - bl) * (tl - bl)).getUnitVector();
//                if (!smoothShading) 
//					{
//                    gtl = normal, gbl = normal, gbr = normal, gtr = normal;
//					}
//                
//                glBegin(GL_TRIANGLES);
//                glNormal3f(gtl.getX(),gtl.getY(),gtl.getZ());
//                glVertex3f(tl.getX(),tl.getY(),tl.getZ());  // Top Left
//                glNormal3f(gbl.getX(),gbl.getY(),gbl.getZ());
//                glVertex3f(bl.getX(),bl.getY(),bl.getZ());  // Bottom Left
//                glNormal3f(gbr.getX(),gbr.getY(),gbr.getZ());
//                glVertex3f(br.getX(),br.getY(),br.getZ());  // Bottom Right
//                glEnd();
//                
//                glBegin(GL_TRIANGLES);
//                glNormal3f(gtl.getX(),gtl.getY(),gtl.getZ());
//                glVertex3f(tl.getX(),tl.getY(),tl.getZ());  // Top Left
//                glNormal3f(gbr.getX(),gbr.getY(),gbr.getZ());
//                glVertex3f(br.getX(),br.getY(),br.getZ());  // Bottom Right
//                glNormal3f(gtr.getX(),gtr.getY(),gtr.getZ());
//                glVertex3f(tr.getX(),tr.getY(),tr.getZ());  // Top Right
//                glEnd();
//				}
//			}
//    delete[] pts;
//    } else {
//        // Adaptive Mode
//        
//        bezPt *pts = new bezPt[4];
//        for (int v = 0; v < 2; v++)
//			{
//            for (int u = 0; u < 2; u++)
//				{
//                pts[u + 2*v] = patch.bezpatchinterp(patch,u,v);
//				}
//			}
//        
//        vect tl = pts[0].pt;
//        vect tr = pts[1].pt;
//        vect bl = pts[2].pt;
//        vect br = pts[3].pt;
//        
//        triangle firstTriangle(tl, bl, br);
//        firstTriangle.setUV(0, 0, 0, 1, 1, 1);
//        
//        triangle secondTriangle(tl, br, tr);
//        secondTriangle.setUV(0, 0, 1, 1, 1, 0);
//        
//        splitTriangle(patch, firstTriangle, 20);
//        splitTriangle(patch, secondTriangle, 20);
//
//        for (int i = 0; i < (int)triangles.size(); i++) 
//			{
//            triangle t = triangles[i];
//			vect normal =  ((t.p2 - t.p1) * (t.p3 - t.p1)).getUnitVector();
//            vect p1normal, p2normal, p3normal;
//
//            if (smoothShading) 
//				{
//                p1normal = patch.bezpatchinterp(patch,t.p1u,t.p1v).gradient;
//                p2normal = patch.bezpatchinterp(patch,t.p2u,t.p2v).gradient;
//                p3normal = patch.bezpatchinterp(patch,t.p3u,t.p3v).gradient;
//
//				//normal correction
//				float anglep1 = acos(p1normal.getUnitVector()^normal.getUnitVector());
//				float anglep2 = acos(p2normal.getUnitVector()^normal.getUnitVector());
//				float anglep3 = acos(p3normal.getUnitVector()^normal.getUnitVector());
//
//				if(p1normal.getMagnitude() <= 0.01) p1normal = normal;
//				else if(!(anglep1 < PI/2 && anglep1 >= 0)) p1normal = -1*p1normal;
//				if(p2normal.getMagnitude() <= 0.01) p2normal = normal;
//				else if(!(anglep2 < PI/2 && anglep2 >= 0)) p2normal = -1*p2normal;
//				if(p3normal.getMagnitude() <= 0.01) p3normal = normal;
//				else if(!(anglep3 < PI/2 && anglep3 >= 0)) p3normal = -1*p3normal;
//				}
//			else 
//				{
//                p1normal = normal, p2normal = normal, p3normal = normal;
//				}
//			
//			glBegin(GL_TRIANGLES);
//			glNormal3f(p1normal.getX(), p1normal.getY(), p1normal.getZ());
//            glVertex3f(t.p1.getX(), t.p1.getY(), t.p1.getZ());
//            glNormal3f(p2normal.getX(), p2normal.getY(), p2normal.getZ());
//            glVertex3f(t.p2.getX(), t.p2.getY(), t.p2.getZ());
//            glNormal3f(p3normal.getX(), p3normal.getY(), p3normal.getZ());
//            glVertex3f(t.p3.getX(), t.p3.getY(), t.p3.getZ());
//            glEnd();
//        }
//        triangles.clear();
//    }
//}
//
////***************************************************
//// function that does the actual drawing
////***************************************************
//void myDisplay()
//{
//    
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer (sets everything to black)
//    
//    if (toonShading)
//    {
//        toonVertex = initshaders(GL_VERTEX_SHADER, "shaders/toonvert.glsl");
//        toonFragment = initshaders(GL_FRAGMENT_SHADER, "shaders/toonfrag.glsl");
//        toonShader = initprogram(toonVertex, toonFragment);
//    }
//    
//    if (!smoothShading)
//		{
//        glShadeModel(GL_FLAT);
//		} 
//	else 
//		{
//        glShadeModel(GL_SMOOTH);
//		}
//    
//	glDisable(GL_DEPTH_TEST) ;
//    
//	material selectedMaterial;
//	material white = {{1,1,1, 1.0f},{0,0,0, 1.0f}, {0,0,0, 1.0f},{0}};
//	material grey = {{0.1,0.1,0.1, 1.0f},{0.1,0.1,0.1, 1.0f}, {0.1,0.1,0.1, 1.0f},{0}};
//
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getMaterial(currentMaterial).diffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getMaterial(currentMaterial).specular);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMaterial(currentMaterial).ambient);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, getMaterial(currentMaterial).shininess);
//	
//    //add lights
//    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
//    GLfloat lightPos1[] = {0.0f, 3.0f, 10.0f, 1.0f};
//    
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//   
//    if(!wmode) glEnable(GL_LIGHTING);
//	else glDisable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//
//    glEnable(GL_NORMALIZE);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glPushMatrix();
//    
//	glEnable(GL_DEPTH_TEST);
//	
//	if(!drawObj)
//		{
//		selectedObject = 0;
//
//			selectedMaterial = getMaterial(currentMaterial);
//
//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, selectedMaterial.diffuse);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, selectedMaterial.specular);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, selectedMaterial.ambient);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, selectedMaterial.shininess);
//
//		glPushMatrix();
//		translatez[0] = -1*(scale[0]*maxz[0] - maxz[0]);
//		glTranslatef(translatex[0],translatey[0],translatez[0]);
//		glRotatef(rotatex[0], 1, 0, 0);
//		glRotatef(rotatey[0], 0, 1, 0);
//		glRotatef(rotatez[0], 0, 0, 1);
//		for (int i = 0; i < curvecount[0]; i++)
//			{
//			//scale the control points
//			bezPatches[i].resize(scale[0]);
//			drawPatch(bezPatches[i], step);
//			}
//		glPopMatrix();
//		}
//	else
//		{
//		for(int i = 1; i<=noOfObjFiles;i++)
//			{
//
//			if(selectedObject == i) selectedMaterial = getMaterial(currentMaterial);
//			else selectedMaterial = grey;
//
//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, selectedMaterial.diffuse);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, selectedMaterial.specular);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, selectedMaterial.ambient);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, selectedMaterial.shininess);
//
//			glPushMatrix();
//			translatez[i] = -1*(scale[i]*maxz[i] - maxz[i]);
//			glTranslatef(translatex[i],translatey[i],translatez[i]);
//			glRotatef(rotatex[i], 1, 0, 0);
//			glRotatef(rotatey[i], 0, 1, 0);
//			glRotatef(rotatez[i], 0, 0, 1);
//			glScalef(scale[i], scale[i], scale[i]);
//			glScalef(2/maxObjcoordinate[i],2/maxObjcoordinate[i],2/maxObjcoordinate[i]);
//
//			for(int x = startpt[i]; x <endpt[i]; x++)
//				{
//				vect normal = ((ObjTriangles[x].p2 - ObjTriangles[x].p1)*(ObjTriangles[x].p3 - ObjTriangles[x].p1)).getUnitVector();
//				triangle newnormal;
//
//				if(smoothShading)
//					{
//					float angle1 = acos(ObjNormals[x].p1.getUnitVector()^normal.getUnitVector());
//					float angle2 = acos(ObjNormals[x].p2.getUnitVector()^normal.getUnitVector());
//					float angle3 = acos(ObjNormals[x].p3.getUnitVector()^normal.getUnitVector());
//
//					if(angle1 > PI/2+0.1) newnormal.p1 = -1*ObjNormals[x].p1;
//					else newnormal.p1 = ObjNormals[x].p1;
//					if(angle2 > PI/2+0.1) newnormal.p2 = -1*ObjNormals[x].p2;
//					else newnormal.p2 = ObjNormals[x].p2;
//					if(angle3 > PI/2+0.1) newnormal.p3 = -1*ObjNormals[x].p3;
//					else newnormal.p3 = ObjNormals[x].p3;
//				
//					glBegin(GL_TRIANGLES);
//					glNormal3f(newnormal.p1.getX(), newnormal.p1.getY(),newnormal.p1.getZ());
//					glVertex3f(ObjTriangles[x].p1.getX(),ObjTriangles[x].p1.getY(), ObjTriangles[x].p1.getZ());
//					glNormal3f(newnormal.p2.getX(), newnormal.p2.getY(),newnormal.p2.getZ());
//					glVertex3f(ObjTriangles[x].p2.getX(),ObjTriangles[x].p2.getY(), ObjTriangles[x].p2.getZ());
//					glNormal3f(newnormal.p3.getX(), newnormal.p3.getY(),newnormal.p3.getZ());
//					glVertex3f(ObjTriangles[x].p3.getX(),ObjTriangles[x].p3.getY(), ObjTriangles[x].p3.getZ());
//					glEnd();	
//					}
//				else
//					{
//					glBegin(GL_TRIANGLES);
//				
//					glNormal3f(normal.getX(), normal.getY(),normal.getZ());
//					glVertex3f(ObjTriangles[x].p1.getX(),ObjTriangles[x].p1.getY(), ObjTriangles[x].p1.getZ());
//					glVertex3f(ObjTriangles[x].p2.getX(),ObjTriangles[x].p2.getY(), ObjTriangles[x].p2.getZ());
//					glVertex3f(ObjTriangles[x].p3.getX(),ObjTriangles[x].p3.getY(), ObjTriangles[x].p3.getZ());
//					glEnd();
//					}
//				}
//			glPopMatrix();
//			}
//
//		for(int x = 1; x <= noOfBezFiles;x++)
//			{
//			int id = x + noOfObjFiles;
//
//			if(selectedObject == id) selectedMaterial = getMaterial(currentMaterial);
//			else selectedMaterial = grey;
//
//
//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, selectedMaterial.diffuse);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, selectedMaterial.specular);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, selectedMaterial.ambient);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, selectedMaterial.shininess);
//                
//			glPushMatrix();
//			translatez[id] = -1*(scale[id]*maxz[id] - maxz[id]);
//			glTranslatef(translatex[id],translatey[id],translatez[id]);
//			glRotatef(rotatex[id], 1, 0, 0);
//			glRotatef(rotatey[id], 0, 1, 0);
//			glRotatef(rotatez[id], 0, 0, 1);
//			glScalef(2/maxObjcoordinate[id],2/maxObjcoordinate[id],2/maxObjcoordinate[id]);
//
//
//			for (int i = curvecount[x-1]; i < curvecount[x]; i++)
//				{
//				//scale the control points
//				bezPatches[i].resize(scale[id]);
//				drawPatch(bezPatches[i], step);
//				}
//			glPopMatrix();
//			}
//		}
//    
//	//-----------------------------------------------------------------------
//
//	glDisable(GL_LIGHTING);
//	selectedMaterial = white;
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, selectedMaterial.diffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, selectedMaterial.specular);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, selectedMaterial.ambient);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, selectedMaterial.shininess);
//    
//	int x;
//	glRasterPos2f(-4.8, -4.50);
//	for (x = 0; x < (int)strlen(materialName[currentMaterial].c_str());x++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)materialName[currentMaterial][x]);
//	glRasterPos2f(-4.8, -4.85);
//	for (x = 0; x < (int)strlen(file[selectedObject].c_str());x++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)file[selectedObject].c_str()[x]);
//
//	glPopMatrix();
//	glFlush();
//	glutSwapBuffers();
//}
//
//
////****************************************************
//// called by glut when there are no messages to handle
////****************************************************
//void myFrameMove() {
//    //nothing here for now
//#ifdef _WIN32
//    Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
//#endif
//    glutPostRedisplay(); // forces glut to call the display function (myDisplay())
//}
//
//void press(unsigned char k, int x, int y)
//{
//	switch(k)
//    {
//		case 49:
//			currentMaterial -= 1;
//			if(currentMaterial == -1) currentMaterial = totalMaterial - 1;
//		break;
//
//		case 50:
//			currentMaterial += 1;
//			if(currentMaterial == totalMaterial) currentMaterial = 0;
//		break;
//
//		case 90: case 122:
//			selectedObject -=1;
//			if(selectedObject == 0)
//				{
//				selectedObject = totalFiles - 1;
//				}
//		break;
//
//		case 88: case 120:
//			selectedObject +=1;
//			if(selectedObject > totalFiles - 1)
//				{
//				selectedObject = 1;
//				}
//		break;
//
//		case 87: case 119:
//			if(!wmode)
//				{
//				wmode = true;
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//				}
//			else
//				{
//				wmode = false;
//				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//				}
//            break;
//            
//        case 83: case 115:
//            if (!smoothShading)
//				{
//                smoothShading = true;
//				}
//            else
//				{
//                smoothShading = false;
//				}
//            break;
//            
//		case 43: case 61:
//			scale[selectedObject] += 0.25;
//            break;
//            
//		case 45: case 95:
//			scale[selectedObject] -= 0.25;
//            break;
//            
//		case 32:
//			exit(0);
//            break;
//    }
//}
//
//void SpecialInput(int key, int x, int y)
//{
//	switch(key)
//    {
//		case GLUT_KEY_UP:
//			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey[selectedObject] += scale[selectedObject] * 0.25;
//			else rotatex[selectedObject]-=10;
//            break;
//            
//		case GLUT_KEY_DOWN:
//			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatey[selectedObject] -= scale[selectedObject] * 0.25;
//			else rotatex[selectedObject]+=10;
//            break;
//            
//		case GLUT_KEY_LEFT:
//			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex[selectedObject] -= scale[selectedObject] * 0.25;
//			else rotatey[selectedObject]+=10;
//            break;
//            
//		case GLUT_KEY_RIGHT:
//			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) translatex[selectedObject] +=scale[selectedObject] * 0.25;
//			else rotatey[selectedObject]-=10;
//            break;
//		case GLUT_KEY_PAGE_UP:
//			rotatez[selectedObject] += 10;
//			break;
//		case GLUT_KEY_PAGE_DOWN:
//			rotatez[selectedObject] -= 10;
//			break;
//    }
//}
//
////****************************************************
//// the usual stuff, nothing exciting here
////****************************************************
//int main(int argc, char *argv[])
//{
//	//This initializes glut
//	glutInit(&argc, argv);
//    
//	//This tells glut to use a double-buffered window with red, green, and blue channels
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//
//	// Initalize theviewport size
//	viewport.w = 600;
//	viewport.h = 600;
//    
//	//The size and position of the window
//	glutInitWindowSize(viewport.w, viewport.h);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("Assignment 3");
//    
//	initScene();                                 // quick function to set up scene
//	//read in bez file
//	file.push_back(argv[1]);
//	step = atof(argv[2]);
//	parameter = step;
//	
//	if(argc > 3)
//		{
//            if(strcmp(argv[3], "-a")==0) adaptiveMode = 1;
//            if(strcmp(argv[3], "-t")==0) toonShading = 1;
//            if(strcmp(argv[3], "-at")==0) {
//                toonShading = 1;
//                adaptiveMode = 1;
//            }
//
//		}
//
//	if(adaptiveMode && argc > 4)
//		{
//		noOfObjFiles = atoi(argv[4]);
//		totalFiles += noOfObjFiles;
//		if(noOfObjFiles > 0) 
//			{
//			drawObj = true;
//			for(int x = 5; x < 5 + noOfObjFiles; x++) file.push_back(argv[x]);
//			}
//		if(argc > 5 + noOfObjFiles)
//			{
//			noOfBezFiles = atoi(argv[noOfObjFiles + 5]);
//			totalFiles += noOfBezFiles;
//			if(noOfBezFiles)
//				{
//				drawObj = true;
//				for(int x = noOfObjFiles + 6; x < 6 + noOfObjFiles+noOfBezFiles; x++) 
//					{
//					file.push_back(argv[x]);
//					}
//				}
//			}
//		}
//	else if(!adaptiveMode && argc > 3)
//		{
//		noOfObjFiles = atoi(argv[3]);
//		totalFiles += noOfObjFiles;
//		if(noOfObjFiles > 0) 
//			{
//			drawObj = true;
//			for(int x = 4; x < 4 + noOfObjFiles; x++) file.push_back(argv[x]);
//			}
//		if(argc > 4 + noOfObjFiles)
//			{
//			noOfBezFiles = atoi(argv[noOfObjFiles + 4]);
//			totalFiles += noOfBezFiles;
//			if(noOfBezFiles)
//				{
//				drawObj = true;
//				for(int x = noOfObjFiles + 5; x < 5 + noOfObjFiles+noOfBezFiles; x++) 
//					{
//					file.push_back(argv[x]);
//					}
//				}
//			}
//		}
//
//	std::ifstream inpfile(file[0].c_str());
//    
//	if(inpfile.good())
//		{
//		int tmp;
//		inpfile>>tmp;
//		curvecount.push_back(tmp);
//		for (int i = 0; i < curvecount[0]; i++)
//			{
//			vect cp[4][4];
//			//read in the 16 control points
//			for (int j = 0; j < 4; j++)
//				{
//				for(int k = 0; k < 4; k++)
//					{
//					float x,y,z;
//					inpfile>>x>>y>>z;
//					cp[j][k] = vect(x,y,z,1);
//					if(z> maxz[0]) maxz[0] = z;
//					}
//				}
//			bezPatches.push_back(bezPatch(cp));
//			}
//	 }
//	else
//		{
//		cout<<"File does not exist."<<endl;
//		}
//
//	inpfile.close();
//
//	if(drawObj)
//		{
//		for(int x = 1; x <= noOfObjFiles;x++)
//			{
//			objVertices.clear();
//			normalVertices.clear();
//			maxObjcoordinate.push_back(0);
//			objVertices.push_back(vect(0,0,0,0));
//			normalVertices.push_back(vect(0,0,0,0));
//			rotatex.push_back(0), rotatey.push_back(0), rotatez.push_back(0), translatex.push_back(transx[x%7]), translatey.push_back(transy[x%7]), translatez.push_back(0), scale.push_back(1.0), maxz.push_back(0);
//
//			startpt.push_back((int)ObjTriangles.size());
//
//			inpfile.open(file[x].c_str());
//
//			std::string line;
//			
//			while(inpfile.good()) 
//				{
//					  std::vector<std::string> splitline;
//					  std::string buf;
//
//					  std::getline(inpfile,line);
//					  std::stringstream ss(line);
//
//					  while (ss >> buf) 
//						{
//						splitline.push_back(buf);
//						}
//					  //Ignore blank lines
//					  if(splitline.size() == 0) 
//						{
//						continue;
//						}
//
//					  //Ignore comments
//					  if(splitline[0][0] == '#') {continue;}
//
//					  else if(!splitline[0].compare("v")) 
//						{
//						float v1 = (float)atof(splitline[1].c_str());
//						float v2 = (float)atof(splitline[2].c_str());
//						float v3 = (float)atof(splitline[3].c_str());
//						if (v1 > maxObjcoordinate[x]) maxObjcoordinate[x] = v1;
//						if (v2 > maxObjcoordinate[x]) maxObjcoordinate[x] = v2;
//						if (v3 > maxObjcoordinate[x]) maxObjcoordinate[x] = v3;
//						objVertices.push_back(vect(v1,v2,v3,0));
//						}
//
//					  else if(!splitline[0].compare("vn")) 
//						{
//						float v1 = (float)atof(splitline[1].c_str());
//						float v2 = (float)atof(splitline[2].c_str());
//						float v3 = (float)atof(splitline[3].c_str());
//						normalVertices.push_back(vect(v1,v2,v3,0));
//						}
//				
//					  else if(!splitline[0].compare("f")) 
//						{
//						string token1, token2, token3, token4;
//						string delimiter = "/", token;
//						int p1,p2,p3,p4,length;
//						float normal1, normal2, normal3,normal4;
//
//						if(splitline.size()==4)
//							{
//							bool normalexists = false;
//							length = splitline[1].find(delimiter);
//							token1 = splitline[1].substr(0, length);
//							p1 = atoi(token1.c_str());
//							token1 = splitline[1].substr(length+1, 100);
//
//							if(strlen(token1.c_str())!=0) normalexists = true;
//
//							if(token1.find(delimiter) == 0)
//								{
//								normal1 = atoi(token1.substr(token1.find(delimiter)+1, 100).c_str());
//								}
//							else if(token1.find(delimiter) != strlen(token1.c_str()))
//								{
//								normal1 = atoi(token1.substr(token1.find(delimiter)+1, 100).c_str());
//								}
//
//							length = splitline[2].find(delimiter);
//							token2 = splitline[2].substr(0, length);
//							p2 = atoi(token2.c_str());
//							token2 = splitline[2].substr(length+1, 100);
//						
//							if(token2.find(delimiter) == 0)
//								{
//								normal2 = atoi(token2.substr(token2.find(delimiter)+1, 100).c_str());
//								}
//
//							else if(token2.find(delimiter) != strlen(token2.c_str()))
//								{
//								normal2 = atoi(token2.substr(token2.find(delimiter)+1, 100).c_str());
//								}	
//
//							length = splitline[3].find(delimiter);
//							token3 = splitline[3].substr(0, length);
//							p3 = atoi(token3.c_str());
//							token3 = splitline[3].substr(length+1, 100);
//						
//							if(token3.find(delimiter) == 0)
//								{
//								normal3 = atoi(token3.substr(token3.find(delimiter)+1, 100).c_str());
//								}
//							else if(token3.find(delimiter) != strlen(token3.c_str()))
//								{
//								normal3 = atoi(token3.substr(token3.find(delimiter)+1, 100).c_str());
//								}
//
//							ObjTriangles.push_back(triangle(objVertices[p1],objVertices[p2],objVertices[p3]));
//							if(normalexists) {ObjNormals.push_back(triangle(normalVertices[normal1],normalVertices[normal2],normalVertices[normal3]));}
//
//							}
//						else
//							{
//						
//							bool normalexists = false;
//					
//							length = splitline[1].find(delimiter);
//							token1 = splitline[1].substr(0, length);
//							p1 = atoi(token1.c_str());
//							token1 = splitline[1].substr(length+1, 100);
//
//							if(strlen(token1.c_str())!=0) normalexists = true;
//
//							if(token1.find(delimiter) == 0)
//								{
//								normal1 = atoi(token1.substr(token1.find(delimiter)+1, 100).c_str());
//								}
//							else if(token1.find(delimiter) != strlen(token1.c_str()))
//								{
//								normal1 = atoi(token1.substr(token1.find(delimiter)+1, 100).c_str());
//								}
//
//							length = splitline[2].find(delimiter);
//							token2 = splitline[2].substr(0, length);
//							p2 = atoi(token2.c_str());
//							token2 = splitline[2].substr(length+1, 100);
//						
//							if(token2.find(delimiter) == 0)
//								{
//								normal2 = atoi(token2.substr(token2.find(delimiter)+1, 100).c_str());
//								}
//
//							else if(token2.find(delimiter) != strlen(token2.c_str()))
//								{
//								normal2 = atoi(token2.substr(token2.find(delimiter)+1, 100).c_str());
//								}	
//
//							length = splitline[3].find(delimiter);
//							token3 = splitline[3].substr(0, length);
//							p3 = atoi(token3.c_str());
//							token3 = splitline[3].substr(length+1, 100);
//						
//							if(token3.find(delimiter) == 0)
//								{
//								normal3 = atoi(token3.substr(token3.find(delimiter)+1, 100).c_str());
//								}
//							else if(token3.find(delimiter) != strlen(token3.c_str()))
//								{
//								normal3 = atoi(token3.substr(token3.find(delimiter)+1, 100).c_str());
//								}
//
//							length = splitline[4].find(delimiter);
//							token4 = splitline[4].substr(0, length);
//							p4 = atoi(token4.c_str());
//							token4 = splitline[4].substr(length+1, 100);
//
//
//							if(token4.find(delimiter) == 0)
//								{
//								normal4 = atoi(token4.substr(token4.find(delimiter)+1, 100).c_str());
//								}
//							if(token4.find(delimiter) != strlen(token4.c_str()))
//								{
//								normal4 = atoi(token4.substr(token4.find(delimiter)+1, 100).c_str());
//								}
//
//							ObjTriangles.push_back(triangle(objVertices[p1],objVertices[p2],objVertices[p4]));
//							if(normalexists) ObjNormals.push_back(triangle(normalVertices[normal1],normalVertices[normal2],normalVertices[normal4]));
//							ObjTriangles.push_back(triangle(objVertices[p2],objVertices[p3],objVertices[p4]));
//							if(normalexists) ObjNormals.push_back(triangle(normalVertices[normal2],normalVertices[normal3],normalVertices[normal4]));
//							}
//						}
//					  else
//						{
//						continue;
//						//std::cerr << "Unknown command: " << splitline[0] << std::endl;
//						}
//					}
//				inpfile.close();   
//				endpt.push_back((int)ObjTriangles.size());
//				}
//			}
// 
//		for(int a = noOfObjFiles+1; a <totalFiles;a++)
//			{
//			maxObjcoordinate.push_back(0);
//			rotatex.push_back(0), rotatey.push_back(0), rotatez.push_back(0), translatex.push_back(transx[a%7]), translatey.push_back(transy[a%7]), translatez.push_back(0), scale.push_back(1.0), maxz.push_back(0);
//			inpfile.open(file[a].c_str());
//    
//			if(inpfile.good())
//				{
//				int tmp;
//				inpfile>>tmp;
//				tmp = tmp+curvecount[curvecount.size()-1];
//				curvecount.push_back(tmp);
//
//				for (int i =  curvecount[a-noOfObjFiles-1]; i < curvecount[a-noOfObjFiles]; i++)
//					{
//					vect cp[4][4];
//					//read in the 16 control points
//					for (int j = 0; j < 4; j++)
//						{
//						for(int k = 0; k < 4; k++)
//							{
//							float x,y,z;
//							inpfile>>x>>y>>z;
//
//							if(x > maxObjcoordinate[a])  maxObjcoordinate[a] = x;
//							if(y > maxObjcoordinate[a])  maxObjcoordinate[a] = y;
//							if(z > maxObjcoordinate[a])  maxObjcoordinate[a] = z;
//
//							cp[j][k] = vect(x,y,z,1);
//							if(z > maxz[a]) maxz[a] = z;
//							}
//						}
//					bezPatches.push_back(bezPatch(cp));
//					}
//			 }
//			else
//				{
//				cout<<"File does not exist."<<endl;
//				}
//
//			inpfile.close();
//			}
//
//		glutSpecialFunc(SpecialInput);
//		glutKeyboardFunc(press); 
//		glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
//		glutReshapeFunc(myReshape);                  // function to run when the window gets resized
//		glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
//		glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else
//
//	return 0;
//}
//
//
//
//
//
//
//
//

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <GL/glut.h>
#include <GL/glu.h>


#define ANGLE 15.0
#define MAX_RECUR 10

using namespace std;

// Globals


float limit;
bool adaptive = false;
int numPatches;
bool lines = true;
bool smooth = true;
GLfloat mat_specular[] = {21.0, 21.0, 21.0, 1.0};
GLfloat mat_diffuse[] = {12.5, 12.5, 12.5, 0.5};
GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_shininess[] = {20.0};
GLfloat light_position[] = {0.0, -1.0, -1.0, -1.0};

// Classes

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};

Viewport	viewport;

class Point {
private:
	float x, y, z, u, v;
public:
    Point(): x(0), y(0), z(0), u(0), v(0) {}
    Point(float X, float Y, float Z): x(X), y(Y), z(Z), u(0), v(0) {}
    Point(float X, float Y, float Z, float U, float V): x(X), y(Y), z(Z), u(U), v(V) {}
    Point operator+(Point p) {
	return Point(x+p.x, y+p.y, z+p.z, u+p.u, v+p.v);
    }
    Point add(Point p) {
	return Point(x+p.x, y+p.y, z+p.z, u+p.u, v+p.v);
    }	
    Point operator*(float m) {
	return Point(x*m, y*m, z*m, u*m, v*m);
    }
    float distance(Point p) {
	return sqrt(pow(x-p.x, 2) + pow(y-p.y, 2) + pow(z-p.z, 2));
    }
    Point midpoint(Point p) {
	return add(p) * 0.5;
    }
    bool far(Point p) {
	return distance(p) > limit;
    }
    void putVertex() {
	glVertex3f(x, y, z);
	return;
    }
    void drawFrom(Point p) {
	glBegin(GL_LINES);
	p.putVertex();
	putVertex();
	//	this.putVertex();
	glEnd();
	return;
    }
    void print() {
	printf(" %.2f %.2f %.2f %.2f %.2f ", x, y, z, u, v);
    }
    
};

class Curve {
public:
    Curve(Point p0, Point p1, Point p2, Point p3): pt0(p0), pt1(p1), pt2(p2), pt3(p3) {}
    Point interpolate(float t) {
	Point a = pt0*(1.0-t) + pt1*t;
	Point b = pt1*(1.0-t) + pt2*t;
	Point c = pt2*(1.0-t) + pt3*t;
	
	Point d = a*(1.0-t) + b*t;
	Point e = b*(1.0-t) + c*t;
	return d*(1.0-t) + e*t;
    }
    Point pt0, pt1, pt2, pt3;
};

class Patch {
public:
    Curve getCurveU(int v) { // might not be good to create a new curve every time
	return Curve(pts[v*4], pts[v*4+1], pts[v*4+2], pts[v*4+3]);
    }
    Curve getCurveV(int u) {
	return Curve(pts[u], pts[u+4], pts[u+8], pts[u+12]);
    }
    Curve interpolateU(float v) {
	Point a = getCurveV(0).interpolate(v);
	Point b = getCurveV(1).interpolate(v);
	Point c = getCurveV(2).interpolate(v);
	Point d = getCurveV(3).interpolate(v);
	return Curve(a, b, c, d);
    }
    Curve interpolateV(float u) {
	Point a = getCurveU(0).interpolate(u);
	Point b = getCurveU(1).interpolate(u);
	Point c = getCurveU(2).interpolate(u);
	Point d = getCurveU(3).interpolate(u);
	return Curve(a, b, d, d);
    }
    Point interpolate(float u, float v) {
	return interpolateU(v).interpolate(u);
    }
    Point pts[16];
};

Patch** patches;

//****************************************************
// Simple init function
//****************************************************
void initScene() {

  // Nothing to do here for this simple example.
    glLoadIdentity();
    glutInitDisplayMode(GLUT_DEPTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    //glEnable(GL_DEPTH_TEST);
}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-4, 4, -4, 4, 4, -4);
  //gluOrtho2D(0, viewport.w, 0, viewport.h);

}

// clockwise from face up
void putNormal(Point a, Point b, Point c) {
    Point ab = a * -1.0 + b;
    Point ac = a * -1.0 + c;
    glNormal3f(-ab.y*ac.z + ab.z*ac.y, -ab.z*ac.x+ab.x*ac.z, -ab.x*ac.y+ab.y*ac.x);
}

void interpolatePoints(Point* points, int patchNum, int numSteps) {
    for (int j = 0; j < numSteps - 1; j++) {
	for (int i = 0; i < numSteps - 1; i++) {
	    points[j*numSteps+i] = patches[patchNum]->interpolate(i*limit, j*limit);
	}
	points[(j+1)*numSteps-1] = patches[patchNum]->interpolate(1.0, j*limit);
    }
    for (int i = 0; i < numSteps - 1; i++) {
	points[(numSteps-1)*numSteps+i] = patches[patchNum]->interpolate(i*limit, 1.0);
    }
    points[numSteps*numSteps-1] = patches[patchNum]->interpolate(1.0, 1.0);
}

void printPoints(Point *points, int numSteps) {
    for (int j = 0; j < numSteps; j++) {
	for (int i = 0; i < numSteps; i++) {
	    Point p = points[j*numSteps+i];
	    printf("%.2f %.2f %.2f  ", p.x, p.y, p.z);
	}
	printf("\n");
    }
}

void uniTesQuad() {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // should be in init, then keyboard
    glBegin(GL_QUADS);
    int numSteps = (int)(1.0/ limit) + 2;
    for (int x = 0; x < numPatches; x++) {
	Point* points = new Point[numSteps*numSteps];
	interpolatePoints(points, x, numSteps);
	//printPoints(points, numSteps);
	for(int j = 0; j < numSteps - 1; j++) {
	    for(int i = 0; i < numSteps - 1; i++) {
		putNormal(points[j*numSteps+i], points[j*numSteps+i+1], points[(j+1)*numSteps+i]);
		points[j*numSteps+i].putVertex();
		points[j*numSteps+i+1].putVertex();
		points[(j+1)*numSteps+i+1].putVertex();
		points[(j+1)*numSteps+i].putVertex();
	    }
	}
    }
    glEnd();
}

void uniformTesselation() {
    int numSteps = (int)(1.0 / limit) + 1;
    for (int i = 0; i < numPatches; i++) {
	glColor3f(((float)i)/numPatches, 1.0 - ((float)i)/numPatches, 0.0f);
	//printf("%d", numSteps);
	Point* curr = new Point[numSteps + 1];
	Point* prev = new Point[numSteps + 1];
	Point* temp;
	prev[0] = patches[i]->pts[0];
	for (int u = 1; u < numSteps; u++) {
	    prev[u] = patches[i]->interpolate(u*limit,0.0);
	    prev[u].drawFrom(prev[u-1]);
	}
	prev[numSteps] = patches[i]->interpolate(1.0, 0.0);
	prev[numSteps].drawFrom(prev[numSteps-1]);
	for (int v = 1; v < numSteps; v++) {
	    curr[0] = patches[i]->interpolate(0.0, v*limit);
	    curr[0].drawFrom(prev[0]);
	    for (int u = 1; u < numSteps; u++) {
		curr[u] = patches[i]->interpolate(u*limit, v*limit);
		curr[u].drawFrom(curr[u-1]);
		curr[u].drawFrom(prev[u]);
	    }
	    curr[numSteps] = patches[i]->interpolate(1.0, v*limit);
	    curr[numSteps].drawFrom(curr[numSteps-1]);
	    curr[numSteps].drawFrom(prev[numSteps]);
	    temp = prev;
	    prev = curr;
	    curr = temp;
	}
	curr[0] = patches[i]->interpolate(0.0, 1.0);
	curr[0].drawFrom(prev[0]);
	for (int u = 1; u < numSteps; u++) {
	    curr[u] = patches[i]->interpolate(u*limit, 1.0);
	    curr[u].drawFrom(curr[u-1]);
	    curr[u].drawFrom(prev[u]);
	}
    }
}

void drawTriangle(Point a, Point b, Point c, int patchNum, int recur) {
    // printf("triangle:");
    // a.print();
    // b.print();
    // c.print();
    // printf("\n");
    if (recur > MAX_RECUR) {
	putNormal(c, b, a);
	a.putVertex();
	b.putVertex();
	c.putVertex();
	return;
    }
    Point ab = a.midpoint(b);
    Point nab = patches[patchNum]->interpolate(ab.u, ab.v);
    Point bc = b.midpoint(c);
    Point nbc = patches[patchNum]->interpolate(bc.u, bc.v);
    Point ca = c.midpoint(a);
    Point nca = patches[patchNum]->interpolate(ca.u, ca.v);
    // the latter should probably go in the close function
    bool eab = ab.far(nab);
    bool ebc = bc.far(nbc);
    bool eca = ca.far(nca);
    // printf("depth: %d split at:", recur);
    // if (eab) {
    // 	ab.print();
    // 	printf("is far from");
    // 	nab.print();
    // } if (ebc) {
    // 	bc.print();
    // 	printf("is far from");
    // 	nbc.print();
    // } if (eca) {
    // 	ca.print();
    // 	printf("is far from");
    // 	nca.print();
    // }
    // printf("\n");
    if (eab && ebc && eca) {
	nab.u = ab.u;
	nab.v = ab.v;
	nbc.u = bc.u;
	nbc.v = bc.v;
	nca.u = ca.u;
	nca.v = ca.v;
	drawTriangle(a, nab, nca, patchNum, recur + 1); // ok
	drawTriangle(b, nbc, nab, patchNum, recur + 1); // ok
	drawTriangle(c, nca, nbc, patchNum, recur + 1); // ok
	drawTriangle(nab, nbc, nca, patchNum, recur + 1);
    } else if (eab && ebc) {
	nab.u = ab.u;
	nab.v = ab.v;
	nbc.u = bc.u;
	nbc.v = bc.v;	
	drawTriangle(b, nbc, nab, patchNum, recur + 1);
	drawTriangle(c, nab, nbc, patchNum, recur + 1);
	drawTriangle(a, nab, c, patchNum, recur + 1); // need to change
    } else if (ebc && eca) {
	nbc.u = bc.u;
	nbc.v = bc.v;
	nca.u = ca.u;
	nca.v = ca.v;
	drawTriangle(c, nca, nbc, patchNum, recur + 1);
	drawTriangle(a, nbc, nca, patchNum, recur + 1);
	drawTriangle(b, nbc, a, patchNum, recur + 1);
    } else if (eca && eab) {
	nab.u = ab.u;
	nab.v = ab.v;
	nca.u = ca.u;
	nca.v = ca.v;
	drawTriangle(a, nab, nca, patchNum, recur + 1);
	drawTriangle(b, nca, nab, patchNum, recur + 1);
	drawTriangle(c, nca, b, patchNum, recur + 1);
    } else if (eab) {
	nbc.u = bc.u;
	nbc.v = bc.v;
	drawTriangle(c, a, nab, patchNum, recur + 1);
	drawTriangle(c, nab, b, patchNum, recur + 1);
    } else if (ebc) {
	nbc.u = bc.u;
	nbc.v = bc.v;
	drawTriangle(a, b, nbc, patchNum, recur + 1);
	drawTriangle(a, nbc, c, patchNum, recur + 1);
    } else if (eca) {
	nca.u = ca.u;
	nca.v = ca.v;
	drawTriangle(b, c, nca, patchNum, recur + 1);
	drawTriangle(b, nca, a, patchNum, recur + 1);
    } else {
	putNormal(c, b, a);
	a.putVertex();
	b.putVertex();
	c.putVertex();
    }
}

void adaptiveTessalation() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numPatches; i++) {
	//	printf("what about");
	Point a = patches[i]->pts[0];
	Point b = patches[i]->pts[3];
	Point c = patches[i]->pts[12];
	Point d = patches[i]->pts[15];
	b.u = 1;
	c.v = 1;
	d.u = 1;
	d.v = 1;
	//	printf("getshere");
	drawTriangle(a, c, b, i, 0);
	drawTriangle(d, b, c, i, 0);
	//	printf("here?");
    }
    glEnd();
}



//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the color buffer

    glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
    //glLoadIdentity();				        // make sure transformation is "zero'd"


    // Start drawing
    glColor3f(1.0f, 1.0f, 1.0f);

  // Glbegin(Gl_LINE_STRIP);
  // glVertex3f(0.0f, 0.0f, 0.0f);
  // glVertex3f(0.0f, 0.5f, 0.0f);
  // glVertex3f(0.5f, 0.0f, 0.0f);
  // glEnd();  
  
    if (adaptive) {
	adaptiveTessalation();
    } else {
	uniTesQuad();
    }
    glFlush();
    glutSwapBuffers();					// swap buffers (we earlier set double buffer)
    //printf("hello world");
}

void myDirectionalKeys(int key, int mouseX, int mouseY) {
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
	switch (key) {
	case GLUT_KEY_RIGHT:
	    glTranslatef(1.0f, 0.0f, 1.0f);
	    break;
	case GLUT_KEY_LEFT:
	    glTranslatef(-1.0f, 0.0f, 1.0f);
	    break;
	case GLUT_KEY_UP:
	    glTranslatef(0.0f, 1.0f, 0.0f);
	    break;
	case GLUT_KEY_DOWN:
	    glTranslatef(0.0f, -1.0f, 0.0f);
	    break;
	    // translate object with shift+arrow keys
	default:
	    break;
	}
    } else {
	switch (key) {
	case GLUT_KEY_RIGHT:
	    glRotatef(ANGLE, 0.0f, 0.0f, 1.0f);
	    break;
	case GLUT_KEY_LEFT:
	    glRotatef(-ANGLE, 0.0f, 0.0f, 1.0f);
	    break;
	case GLUT_KEY_UP:
	    glRotatef(ANGLE, 1.0f, 0.0f, 0.0f);
	    break;
	case GLUT_KEY_DOWN:
	    glRotatef(-ANGLE, 1.0f, 0.0f, 0.0f);
	    break;
	    // translate object with shift+arrow keys
	default:
	    break;
	}
    }
}

void myKeyboard(unsigned char key, int mouseX, int mouseY) {
    switch (key) {
    case ' ': // make sure this is working
	exit(0);
	break;
    case 's':
	if (smooth) {
	    glShadeModel(GL_FLAT);
	    smooth = false;
	} else {
	    glShadeModel(GL_SMOOTH);
	    smooth = true;
	}
	break;
    case 'w':
	if (lines) {
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    lines = false;
	} else {
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	    lines = true;
	}
	break;
    case 'h': // optional
	// toggle between filled and hidden-line mode
	break;
    // translate object with shift+arrow keys
    default:
	break;
    }
}

void printPatches() {
    for (int i = 0; i < numPatches; i++) {
	for (int j = 0; j < 4; j++) {
	    printf("%.2f %.2f %.2f  ", patches[i]->pts[j].x, patches[i]->pts[j].y, patches[i]->pts[j].z);
	    printf("%.2f %.2f %.2f  ", patches[i]->pts[j+1].x, patches[i]->pts[j+1].y, patches[i]->pts[j+1].z);
	    printf("%.2f %.2f %.2f  ", patches[i]->pts[j+2].x, patches[i]->pts[j+2].y, patches[i]->pts[j+2].z);
	    printf("%.2f %.2f %.2f\n", patches[i]->pts[j+3].x, patches[i]->pts[j+3].y, patches[i]->pts[j+3].z);
	}
    }
}

void print12(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l) {
    printf("%.2f %.2f %.2f  %.2f %.2f %.2f  %.2f %.2f %.2f  %.2f %.2f %.2f\n", a, b, c, d, e, f, g, h, i, j, k, l);
}

void loadPatches(string file) {
    ifstream inpfile(file.c_str());
    if (!inpfile.is_open()) {
	cout << "Unable to open file" << endl;
    } else {
	inpfile >> numPatches;
	patches = new Patch*[numPatches];
	float a, b, c, d, e, f, g, h, m, j, k, l;
	for (int i = 0; i < numPatches; i++) {
	    inpfile >> a >> b >> c >> d >> e >> f >> g >> h >> m >> j >> k >> l;
	    //print12(a, b, c, d, e, f, g, h, m, j, k, l);
	    patches[i] = new Patch();
	    patches[i]->pts[0] = *(new Point(a, b, c)); // I feel like you should do this another way
	    patches[i]->pts[1] = *(new Point(d, e, f));
	    patches[i]->pts[2] = *(new Point(g, h, m));
	    patches[i]->pts[3] = *(new Point(j, k, l));
	    inpfile >> a >> b >> c >> d >> e >> f >> g >> h >> m >> j >> k >> l;
	    //print12(a, b, c, d, e, f, g, h, m, j, k, l);
	    patches[i]->pts[4] = *(new Point(a, b, c));
	    patches[i]->pts[5] = *(new Point(d, e, f));
	    patches[i]->pts[6] = *(new Point(g, h, m));
	    patches[i]->pts[7] = *(new Point(j, k, l));
	    inpfile >> a >> b >> c >> d >> e >> f >> g >> h >> m >> j >> k >> l;
	    //print12(a, b, c, d, e, f, g, h, m, j, k, l);
	    patches[i]->pts[8] = *(new Point(a, b, c));
	    patches[i]->pts[9] = *(new Point(d, e, f));
	    patches[i]->pts[10] = *(new Point(g, h, m));
	    patches[i]->pts[11] = *(new Point(j, k, l));
	    inpfile >> a >> b >> c >> d >> e >> f >> g >> h >> m >> j >> k >> l;
	    //print12(a, b, c, d, e, f, g, h, m, j, k, l);
	    patches[i]->pts[12] = *(new Point(a, b, c));
	    patches[i]->pts[13] = *(new Point(d, e, f));
	    patches[i]->pts[14] = *(new Point(g, h, m));
	    patches[i]->pts[15] = *(new Point(j, k, l));
	} 
	inpfile.close();
    }
}

//****************************************************
// called by glut when there are no messages to handle
 //****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //printf("not even");

    //This initializes glut
    glutInit(&argc, argv);
    
    //This tells glut to use a double-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Initalize theviewport size
    viewport.w = 400;
    viewport.h = 400;

    string file = argv[1];
    limit = atof(argv[2]);
    
    //printf("what about");

    if (argc > 3 && !strncmp(argv[3], "-a", 2)) {
	adaptive = true;
    }

    //printf("okay, here?");

    loadPatches(file);
    //printPatches();

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  glutReshapeFunc(myReshape);				// function to run when the window gets resized
  glutKeyboardFunc(myKeyboard);
  glutSpecialFunc(myDirectionalKeys);
  glutIdleFunc(myFrameMove);

  glutMainLoop();							// infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}