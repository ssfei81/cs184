#include "sphere.h"
#include "object.h"
#include "brdf.h"
#include "matrix.h"
#include "bvh.h"
#include <vector>
using namespace std;

#ifndef _ALLOBJECTS_H_
#define _ALLOBJECTS_H_
#include <cfloat>

class allObjects
	{
	private:
		vector<object*> o;
		
	public:	
		allObjects() //default constructor
			{
			//do nothing, ignore
			}
		allObjects(vector<object*> &o)
			{
			this->o = o;
			}

		//version A
		bool traverseTreeA(ray &r, float &minHit, vect &objNormal, vect &minIntersect, object* &minObject, bvh *tree, float *thit, vect *normal, vect *intersect)
			{
			//bottom of the tree
			if(tree->getLeft()==NULL && tree->getRight()==NULL)
				{

				bool hit = tree->getObj()->getIntersection(r,thit,normal,intersect);
                    					
					if(hit && *thit <minHit)
						{
						minHit = *thit;
						minObject = tree->getObj();
                        objNormal = *normal;
						minIntersect = *intersect;
						}
				return true;
				}

			bbox box = bbox(tree->getmin(), tree->getmax());
			if(!box.intersect(r)) return false;

			bool hit = false;
			if(tree->getLeft()!=NULL) 
				{
				bool hit2 = traverseTreeA(r, minHit, objNormal, minIntersect, minObject, tree->getLeft(),thit,normal,intersect);
				if(hit2) hit = true;
				}
			if(tree->getRight()!=NULL) 
				{
				bool hit2 = traverseTreeA(r, minHit, objNormal, minIntersect, minObject, tree->getRight(),thit,normal,intersect);
				if(hit2) hit = true;
				}
			return hit;
			}

		//versionB
		bool traverseTreeB(ray &r, bvh *tree)
			{
			if(tree->getLeft()==NULL && tree->getRight()==NULL)
				{
				return tree->getObj()->intersectP(r);
				}

			bbox box = bbox(tree->getmin(), tree->getmax());
			if(!box.intersect(r)) return false;

			bool hit = false;
			if(tree->getLeft()!=NULL) 
				{
				bool hit2 = traverseTreeB(r, tree->getLeft());
				if(hit2) hit = true;
				}
			if(tree->getRight()!=NULL) 
				{
				bool hit2 = traverseTreeB(r, tree->getRight());
				if(hit2) hit = true;
				}
			return hit;
			}

		bool getIntersection(ray &r,float *thit, vect *normal, vect *intersect, object* &obj, int &nextId, int skip, bvh *tree)
			{
			float minHit = FLT_MAX;
			object *minObject;
            vect objNormal;
			vect minIntersect;
			int minId;

			traverseTreeA(r,minHit,objNormal,minIntersect,minObject,tree,thit,normal,intersect);
			//browse through all the objects
			
			/*
			for(int l = 0; l < (int) this->o.size(); l++)
				{
				if(l!=skip)
					{
					bool hit = this->o[l]->getIntersection(r,thit,normal,intersect);
                    					
					if(hit && *thit <minHit)
						{
						minHit = *thit;
						minObject = this->o[l];
                        objNormal = *normal;
						minIntersect = *intersect;
						minId = l;
						}
					}
				}
			*/

			if (minHit < FLT_MAX)
				{
				//ray hits some object!
				*thit = minHit;
                *normal = objNormal;
				*intersect = minIntersect;
				obj = minObject;
				return true;
				}

			return false;
			}
        
		bool intersectP(ray &r, bvh *tree)
        {
			return traverseTreeB(r,tree);
			
			/*
			for(int l = 0; l < (int) this->o.size(); l++)
            {
				bool hit = this->o[l]->intersectP(r);
                
				if(hit) {return true;}
            }
			return false;
			*/
        }
        
	};

#endif