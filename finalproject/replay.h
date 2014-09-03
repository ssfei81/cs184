#ifndef _REPLAY_H_
#define _REPLAY_H_

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include "vect.h"
#include "FreeImage.h"
#include <iostream>
using namespace std;

class replay
{
private:
    vector<float> keeperpos;
    vector<Vector> ballpos;
    vector<float> ballrot;
    vector<float> yPos;
    vector<bool> special;
    
    int totalFrame;
    int framesPlayed;
    int frameDelay;
    int delayCount;
    
public:
	replay()
        {
        totalFrame = 0;
        framesPlayed = 0;
        frameDelay = 3;
        delayCount = 0;
        }
    
    void addFrame(float kpos, Vector bpos, float brot,float yPos2, bool activatedSpecial)
        {
        keeperpos.push_back(kpos);
        ballpos.push_back(bpos);
        ballrot.push_back(brot);
        yPos.push_back(yPos2);
        special.push_back(activatedSpecial);
        totalFrame ++;
        }
    
    int playNextFrame(float &kpos, Vector &bpos, float &brot, float &yPos2, bool &usedSpecial) //return 1 if ended else return 0
        {
        if(framesPlayed == totalFrame) return 0;
        else
            {
            kpos = keeperpos[framesPlayed];
            bpos = ballpos[framesPlayed];
            brot = ballrot[framesPlayed];
            yPos2 = yPos[framesPlayed];
            usedSpecial = special[framesPlayed];
            delayCount++;
            if(delayCount > frameDelay)
                {
                framesPlayed++;
                delayCount = 0;
                }
            return 1;
            }
        }
    int getTotalFrame() {return totalFrame;}
    int getTotalFrame2(){return framesPlayed;}

};
#endif