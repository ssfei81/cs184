#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "ray.h"
#include "color.h"
#include "camera.h"
#include "allObjects.h"
#include "light.h"
#include "bvh.h"

//auto converts to unit ray
class rayTracer
	{
	private:
		allObjects allObjs;
        vector<light*> lights;
		bvh *tree;
	
	public:	

		rayTracer(allObjects &objs, vector<light*> &lightSources, bvh *tree)
			{
			this->allObjs = objs;
            this->lights = lightSources;
			this->tree = tree;
			}

		color trace(ray &rayT, int depth,int skip)
			{
			if (depth < 0) return color();
			
			float thit;
            vect normal;
			vect intersect;
			object *obj;
			int nextId = -1;
            float r=0.0f, g=0.0f, b=0.0f;
			
			color c = color();
			
			bool hit = this->allObjs.getIntersection(rayT, &thit, &normal, &intersect, obj, nextId, skip, tree);

			//if(skip!=-1) cout<<depth<<" "<<skip<<endl;

			if (!hit) return color();

            //perform shading
            vect lightPt, normalVector, lightVector, reflectedVector;
			vect viewPt = (rayT.getPoint()-intersect).getUnitVector();
               

            //ambient term
            r += (*obj).getBRDF().getem().getR();
            g += (*obj).getBRDF().getem().getG();
            b += (*obj).getBRDF().getem().getB();

            //ambient term
            r += (*obj).getBRDF().getka().getR();
            g += (*obj).getBRDF().getka().getG();
            b += (*obj).getBRDF().getka().getB();
                    
            for (int i = 0; i < (int)(this->lights).size(); i++)
				{
                light* currLight = this->lights.at(i);
				ray lray = (*currLight).generateLightRay(intersect, (*currLight).getPos());

                bool hit2 = this->allObjs.intersectP(lray, tree);
				normalVector = normal.getUnitVector();
				if (!hit2) 
					{ //nothing is blocking the light
                    float angle;
                    float att = (*currLight).getAttenuation(intersect); //attenuation

                    if ((*currLight).lightType() == 0) 
						{ //direction
                        lightPt = (*currLight).getPos();
                        lightVector = (lightPt).getUnitVector();
                        //this is just l.n
                        angle = lightVector^normalVector;
                        reflectedVector = (lightVector * -1 + (angle * normalVector * 2)).getUnitVector();
						}
					else 
						{ //point
                        lightPt = (*currLight).getPos();
                        lightVector = (lightPt-intersect).getUnitVector();
                        //this is just l.n
                        angle = lightVector^normalVector;
                        reflectedVector = (lightVector * -1 + (angle * normalVector * 2)).getUnitVector();
						}

                    //diffuse term(s)
                    r += (*obj).getBRDF().getkd().getR() * (*currLight).getColor().getR() * max(0.0f, angle) * att;
                    g += (*obj).getBRDF().getkd().getG() * (*currLight).getColor().getG() * max(0.0f, angle) * att;
                    b += (*obj).getBRDF().getkd().getB() * (*currLight).getColor().getB() * max(0.0f, angle) * att;
                        
                    //specular term(s), no specular power?
                    r += (*obj).getBRDF().getks().getR() * (*currLight).getColor().getR() * pow(max(0.0f, reflectedVector^viewPt), (*obj).getBRDF().getsp()) * att;
                    g += (*obj).getBRDF().getks().getG() * (*currLight).getColor().getG() * pow(max(0.0f, reflectedVector^viewPt), (*obj).getBRDF().getsp()) * att;
                    b += (*obj).getBRDF().getks().getB() * (*currLight).getColor().getB() * pow(max(0.0f, reflectedVector^viewPt), (*obj).getBRDF().getsp()) * att;
                    }		
				}

			//Handle reflection
			if ((*obj).getBRDF().getkr().getR() > 0 || (*obj).getBRDF().getkr().getG() > 0 || (*obj).getBRDF().getkr().getB() > 0) 
				{
				float angle = -1 * (rayT.getDirection()^normalVector);
				vect reflectedVector2 = (rayT.getDirection() + (angle * normalVector * 2)).getUnitVector();
				ray reflectedRay = ray(intersect, reflectedVector2,(float)0.001, FLT_MAX);

				c = c + ((trace(reflectedRay, depth-1,nextId)) * (*obj).getBRDF().getkr());
				}
			//handle refraction
			if((*obj).getBRDF().getrindex() != 0)
				{
				float angle = -1*(normal^rayT.getDirection());

				//eqn from textbook
				vect d = rayT.getDirection().getUnitVector();
				vect n = normal.getUnitVector();

				
				float rindex = (*obj).getBRDF().getrindex();

				float nin, nout;

				if(angle > 0)
					{
					nin = 1.0;
					nout = rindex;
					}
				else
					{
					nin = rindex;
					nout = 1.0;
					n=-1*n;
					}
				//cout<<nin<<" "<<nout<<endl;
				float dn = d^n;
				float determinant = sqrt(1-(pow(nin,2)*(1-pow(n^d,2)))/pow(nout,2));

				//cout<<determinant<<endl;
				if(determinant >= 0 )
					{
						vect refractedVector = ((nin/nout)*(d-dn*n)-n*determinant).getUnitVector();
						ray refractedRay = ray(intersect, refractedVector,(float)0.01, FLT_MAX);
						//cout<<refractedVector<<endl;

						c = c + ((trace(refractedRay, depth-1,nextId)) * color(1, 1, 1));

					}
				}
			c = c+color(r,g,b);
            return c;
			}
            
    };

#endif