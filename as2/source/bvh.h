#ifndef _BVH_H_
#define _BVH_H_

#include "object.h"
#include "bbox.h"
#include <vector>
#include <algorithm> 
using namespace std;

class bvh
	{
	private:
		vect min;
		vect max;
		bvh *left;
		bvh *right;
		object* obj;

	public:
		bvh()
			{
			this->min = vect(0,0,0,0);
			this->max = vect(0,0,0,0);
			this->left = NULL;
			this->right = NULL;
			}
		vect getmin() {return this->min;}
		vect getmax() {return this->max;}
		bvh *getLeft() {return this->left;}
		bvh *getRight() {return this->right;}
		object *getObj() {return this->obj;}
		void setmin(vect min) {this->min = min;}
		void setmax(vect max) {this->max = max;}
		void setleft(bvh *left) {this->left = left;}
		void setright(bvh *right) {this->right = right;}
		void setObj(object *obj) {this->obj = obj;}
	};


bool sortfunctionX (object* i,object* j) { return i->getBbox().getMin().getX() < j->getBbox().getMin().getX(); }
bool sortfunctionY (object* i,object* j) { return i->getBbox().getMin().getY() < j->getBbox().getMin().getY(); }
bool sortfunctionZ (object* i,object* j) { return i->getBbox().getMin().getZ() < j->getBbox().getMin().getZ(); }

void buildBVH(vector<object*> objs, bvh *parent, int start, int end, int axis)
	{
	if (start-end == 0)
		{
		//add element to bvh tree then return tree

		//cout<<objs.at(0)->getBbox().getMin()<<endl;
		//cout<<objs.at(0)->getBbox().getMax()<<endl;

		parent->setmin(objs.at(0)->getBbox().getMin());
		parent->setmax(objs.at(0)->getBbox().getMax());
		parent->setObj(objs.at(0));
		return;
		}

	//copy list
	vector<object*> objsCopy;


	int i;
	float minx=FLT_MAX,miny=FLT_MAX,minz=FLT_MAX,maxx=FLT_MIN,maxy=FLT_MIN,maxz=FLT_MIN;

	for (i = 0; i < (int)objs.size(); i++)
		{
		//keep on expanding the min and max of current tree
		bbox box = objs.at(i)->getBbox();
		if(box.getMin().getX() < minx) minx = box.getMin().getX();
		if(box.getMin().getY() < miny) miny = box.getMin().getY();
		if(box.getMin().getZ() < minz) minz = box.getMin().getZ();
		if(box.getMax().getX() > maxx) maxx = box.getMax().getX();
		if(box.getMax().getY() > maxy) maxy = box.getMax().getY();
		if(box.getMax().getZ() > maxz) maxz = box.getMax().getZ();
		objsCopy.push_back(objs.at(i));
		}

		parent->setmin(vect(minx, miny, minz, 1));
		parent->setmax(vect(maxx, maxy, maxz, 1));

		//sort the list of objects based on current axis
		if (axis == 0) sort(objsCopy.begin(), objsCopy.end(),sortfunctionX);
		else if (axis == 1) sort(objsCopy.begin(), objsCopy.end(),sortfunctionY);
		else if (axis == 2) sort(objsCopy.begin(), objsCopy.end(),sortfunctionZ);

		
		//rebuild 2 lists: left and right
		bvh *l = new bvh();
		bvh *r = new bvh();
		parent->setleft(l);
		parent->setright(r);

		int half = objsCopy.size()/2;


		//create leftBVH and RightBVH
		int nextAxis;
		if (axis == 0) nextAxis = 1;
		else if (axis == 1) nextAxis = 2;
		else if (axis == 2) nextAxis = 0;

		vector<object*> lList;
		vector<object*> rList;

	for (i = 0; i <= half-1; i++)
		{
		lList.push_back(objsCopy.at(i));
		}
	for (i = half; i <= (int)objsCopy.size()-1; i++)
		{
		rList.push_back(objsCopy.at(i));
		}

		buildBVH(lList, l, 0, half-1, nextAxis);
		buildBVH(rList, r, half, objsCopy.size()-1, nextAxis);
	}
#endif