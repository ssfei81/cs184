#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "triangle.h"
#include "material.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

class object
	{
	private:
	map<string, material> materialList;
	vector<Vector> v;
	vector<Vector> vn;
	vector<Vector> vt;
	vector<triangle> f;

	//vertex array
	vector<GLfloat> *varray[1000];
	vector<GLfloat> *vnarray[1000];
	vector<GLfloat> *vtarray[1000];
	string mtl[1000];
	int current;

	//helper string functions
	vector<string> splitString(string input, string delimiter)
		{
		vector<string> output;
		char *pch;
		char *str = strdup(input.c_str());
		pch = strtok(str, delimiter.c_str());
		while (pch != NULL)
			{
			output.push_back(pch);
			pch = strtok (NULL,  delimiter.c_str());
			}
		free(str);
		return output;
		}

	bool replace(std::string& str, const std::string& from, const std::string& to) 
		{
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
		}

	public:
	object(){}//default constructor

	object(string fileName, string folder)
		{
		//construct, alot of clumsy coding

		/////////////////
		current = -1;
		////////////////

		vt.push_back(Vector());
		vn.push_back(Vector());
		v.push_back(Vector());

		string mtlFile;
		string currentMtl = "";

		ifstream inpfile((folder+fileName).c_str());
		string line;
		while(inpfile.good()) 
			{
			vector<std::string> splitline;
			string buf;

			getline(inpfile,line);
			stringstream ss(line);

			while (ss >> buf) {splitline.push_back(buf);}
			//Ignore blank lines
			if(splitline.size() == 0) {continue;}

			//Ignore comments
			if(splitline[0][0] == '#') {continue;}

			else if(!splitline[0].compare("mtllib")) 
				{
				mtlFile = splitline[1];
				//read in MTL
				ifstream inpfile2((folder+mtlFile).c_str());
				string line2;

				while(inpfile2.good()) 
					{
					float ns = 0, ni = 0, d = 0;
					int illum = 0;
					Vector ka(0,0,0), kd(0,0,0), ks(0,0,0);

					vector<string> splitline2;
					string buf2;

					getline(inpfile2,line2);
					stringstream ss2(line2);

					while (ss2 >> buf2) {splitline2.push_back(buf2);}

					//Ignore blank lines
					if(splitline2.size() == 0) {continue;}

					//Ignore comments
					if(splitline2[0][0] == '#') {continue;}

							
					else if(!splitline2[0].compare("newmtl")) 
						{
						currentMtl = splitline2[1];
						materialList[currentMtl] = material();
						}
					//ns
					else if(!splitline2[0].compare("Ns")) 
						{
						float ns = (float)atof(splitline2[1].c_str());
						materialList[currentMtl].setNs(ns);
						}
					//ni
					else if(!splitline2[0].compare("Ni")) 
						{
						float ni = (float)atof(splitline2[1].c_str());
						materialList[currentMtl].setNi(ni);
						}
					//d
					else if(!splitline2[0].compare("d")) 
						{
						float d = (float)atof(splitline2[1].c_str());
						materialList[currentMtl].setD(d);
						}
					//illum
					else if(!splitline2[0].compare("illum")) 
						{
						int illum = (int)atoi(splitline2[1].c_str());
						materialList[currentMtl].setIllum(illum);
						}
					//ka
					else if(!splitline2[0].compare("Ka")) 
						{
						Vector ka((float)atof(splitline2[1].c_str()), (float)atof(splitline2[2].c_str()), (float)atof(splitline2[3].c_str()));
						materialList[currentMtl].setKa(ka);
						}
					//kd
					else if(!splitline2[0].compare("Kd")) 
						{
						Vector kd((float)atof(splitline2[1].c_str()), (float)atof(splitline2[2].c_str()), (float)atof(splitline2[3].c_str()));
						materialList[currentMtl].setKd(kd);
						}
					//ks
					else if(!splitline2[0].compare("Ks")) 
						{
						Vector ks((float)atof(splitline2[1].c_str()), (float)atof(splitline2[2].c_str()), (float)atof(splitline2[3].c_str()));
						materialList[currentMtl].setKs(ks);
						}
					else if(!splitline2[0].compare("map_Kd")) 
						{
						replace(splitline2[1],"\\\\", "/");
						materialList[currentMtl].setKdMap(folder+splitline2[1]);
						}
					else
						{
						continue;
						}
					}
				inpfile2.close();
			    }

			else if(!splitline[0].compare("usemtl")) 
				{
				//varray addon
				current++;
				mtl[current] = splitline[1];
				varray[current] = new vector<GLfloat>;
				vnarray[current] = new vector<GLfloat>;
				vtarray[current] = new vector<GLfloat>;
				////////////////////////////
				currentMtl = splitline[1];
				}

			//v
			else if(!splitline[0].compare("v")) 
				{
				float v1 = (float)atof(splitline[1].c_str());
				float v2 = (float)atof(splitline[2].c_str());
				float v3 = (float)atof(splitline[3].c_str());
				this->v.push_back(Vector(v1,v2,v3));
				}
			//vn
			else if(!splitline[0].compare("vn")) 
				{
				float v1 = (float)atof(splitline[1].c_str());
				float v2 = (float)atof(splitline[2].c_str());
				float v3 = (float)atof(splitline[3].c_str());
				this->vn.push_back(Vector(v1,v2,v3));
				}
			//vt
			else if(!splitline[0].compare("vt")) 
				{
				float v1 = (float)atof(splitline[1].c_str());
				float v2 = (float)atof(splitline[2].c_str());
				this->vt.push_back(Vector(v1,v2,0));
				}

			  else if(!splitline[0].compare("f")) 
				{
				char vIndex[5][10], vnIndex[5][10], vtIndex[5][10];
				bool hasVT = false, hasVN = false; //has texture vertex? or normal vertex?

				if(splitline.size()==4)
					{
					for(int x=1;x<=3;x++)
						{
						float t = splitline[x].find('/');
						hasVN = splitline[x][t+1]=='/';

						vector<string> splitStr = splitString(splitline[x], "/");
						if(splitStr.size() == 3) 
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							strcpy(vtIndex[x], splitStr[1].c_str()); 
							strcpy(vnIndex[x], splitStr[2].c_str()); 
							hasVN = true; hasVT = true;
							}
						else if(splitStr.size() == 2)
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							if(hasVN)
								{
								strcpy(vnIndex[x], splitStr[1].c_str()); 
								}
							else
								{
								hasVT = true;
								strcpy(vtIndex[x], splitStr[1].c_str()); 
								}
							}
						else
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							}
						}
					if(hasVT && hasVN)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[2])],vn[atoi(vnIndex[3])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[2])],vt[atoi(vtIndex[3])]));
						
						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
							}
						/////////
						}
					else if(hasVN)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[2])],vn[atoi(vnIndex[3])],0));

						////add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

							vtarray[current]->push_back(0);
							vtarray[current]->push_back(0);
							}
						///////////
						}
					else if(hasVT)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[2])],vt[atoi(vtIndex[3])],1));
						
						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);

							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
							}
						/////////
						}
					else
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])]));

						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);

							vtarray[current]->push_back(0);
							vtarray[current]->push_back(0);
							}
						/////////
						}
					//f.back().setMtl(currentMtl);
					}
				else
					{
					for(int x=1;x<=4;x++)
						{
						float t = splitline[x].find('/');
						hasVN = splitline[x][t+1]=='/';

						vector<string> splitStr = splitString(splitline[x], "/");
						if(splitStr.size() == 3) 
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							strcpy(vtIndex[x], splitStr[1].c_str()); 
							strcpy(vnIndex[x], splitStr[2].c_str()); 
							hasVN = true; hasVT = true;
							}
						else if(splitStr.size() == 2)
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							if(hasVN)
								{
								strcpy(vnIndex[x], splitStr[1].c_str()); 
								}
							else
								{
								hasVT = true;
								strcpy(vtIndex[x], splitStr[1].c_str()); 
								}
							}
						else
							{
							strcpy(vIndex[x], splitStr[0].c_str()); 
							}
						}
					if(hasVT && hasVN)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[2])],vn[atoi(vnIndex[3])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[2])],vt[atoi(vtIndex[3])]));
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[3])],v[atoi(vIndex[4])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[3])],vn[atoi(vnIndex[4])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[3])],vt[atoi(vtIndex[4])]));
						
						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
							}

						for(int x = 1; x <=4; x++)
							{
							if(x!=2)
								{
								varray[current]->push_back(v[atoi(vIndex[x])].getX());
								varray[current]->push_back(v[atoi(vIndex[x])].getY());
								varray[current]->push_back(v[atoi(vIndex[x])].getZ());

								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

								vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
								vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
								}
							}
						/////////					
					
						}
					else if(hasVN)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[2])],vn[atoi(vnIndex[3])],0));
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[3])],v[atoi(vIndex[4])],vn[atoi(vnIndex[1])],vn[atoi(vnIndex[3])],vn[atoi(vnIndex[4])],0));

						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
							vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

							vtarray[current]->push_back(0);
							vtarray[current]->push_back(0);
							}

						for(int x = 1; x <=4; x++)
							{
							if(x!=2)
								{
								varray[current]->push_back(v[atoi(vIndex[x])].getX());
								varray[current]->push_back(v[atoi(vIndex[x])].getY());
								varray[current]->push_back(v[atoi(vIndex[x])].getZ());

								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getX());
								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getY());
								vnarray[current]->push_back(vn[atoi(vnIndex[x])].getZ());

								vtarray[current]->push_back(0);
								vtarray[current]->push_back(0);
								}
							}
						/////////	
						}
					else if(hasVT)
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[2])],vt[atoi(vtIndex[3])],1));
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[3])],v[atoi(vIndex[4])],vt[atoi(vtIndex[1])],vt[atoi(vtIndex[3])],vt[atoi(vtIndex[4])],1));

						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);

							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
							vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
							}

						for(int x = 1; x <=4; x++)
							{
							if(x!=2)
								{
								varray[current]->push_back(v[atoi(vIndex[x])].getX());
								varray[current]->push_back(v[atoi(vIndex[x])].getY());
								varray[current]->push_back(v[atoi(vIndex[x])].getZ());

								vnarray[current]->push_back(0);
								vnarray[current]->push_back(0);
								vnarray[current]->push_back(0);

								vtarray[current]->push_back(vt[atoi(vtIndex[x])].getX());
								vtarray[current]->push_back(vt[atoi(vtIndex[x])].getY());
								}
							}
						/////////	
						}
					else
						{
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[2])],v[atoi(vIndex[3])]));
						//f.push_back(triangle(v[atoi(vIndex[1])],v[atoi(vIndex[3])],v[atoi(vIndex[4])]));

						//add on
						for(int x = 1; x <=3; x++)
							{
							varray[current]->push_back(v[atoi(vIndex[x])].getX());
							varray[current]->push_back(v[atoi(vIndex[x])].getY());
							varray[current]->push_back(v[atoi(vIndex[x])].getZ());

							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);
							vnarray[current]->push_back(0);

							vtarray[current]->push_back(0);
							vtarray[current]->push_back(0);
							}

						for(int x = 1; x <=4; x++)
							{
							if(x!=2)
								{
								varray[current]->push_back(v[atoi(vIndex[x])].getX());
								varray[current]->push_back(v[atoi(vIndex[x])].getY());
								varray[current]->push_back(v[atoi(vIndex[x])].getZ());

								vnarray[current]->push_back(0);
								vnarray[current]->push_back(0);
								vnarray[current]->push_back(0);

								vtarray[current]->push_back(0);
								vtarray[current]->push_back(0);
								}
							}
						/////////	
						}
					//f[f.size()-2].setMtl(currentMtl);
					//f.back().setMtl(currentMtl);
					}
				}
			  else
				{
				continue;
				//std::cerr << "Unknown command: " << splitline[0] << std::endl;
				}
			}
		inpfile.close();
		}

	Vector getV(int index) {return v[index];}
	Vector getVn(int index) {return vn[index];}
	Vector getVt(int index) {return vt[index];}
	material getMaterial(string key) {return this->materialList[key];}

	void draw()
		{
		vector<triangle>::iterator tri;
		string currentMaterial;


	//immediate mode
	/*	for(tri = this->f.begin(); tri != this->f.end(); tri++)
			{
			string triMaterial = (*tri).getMtl();

			if(triMaterial != currentMaterial)
				{
				currentMaterial = triMaterial;
				
				if(strlen(currentMaterial.c_str())!=0)
					{
					material mtl = this->materialList[currentMaterial];
					GLfloat ka[] = {mtl.getKa().x,mtl.getKa().y,mtl.getKa().z};
					GLfloat ks[] = {mtl.getKs().x,mtl.getKs().y,mtl.getKs().z};
					GLfloat kd[] = {mtl.getKd().x,mtl.getKd().y,mtl.getKd().z};
					GLfloat ns[] = {mtl.getNs()};

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ka);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ns);
					}
				}
		
			GLuint kdMap = this->materialList[currentMaterial].getKdMap();


			if(kdMap)
				{
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glBindTexture(GL_TEXTURE_2D, kdMap);
				glEnable(GL_COLOR_MATERIAL);
				glColor3f(0.8,0.8,0.8);
				}

			Vector p1 = (*tri).getP1(); Vector p2 = (*tri).getP2(); Vector p3 = (*tri).getP3();
			Vector n1 = (*tri).getN1(); Vector n2 = (*tri).getN2(); Vector n3 = (*tri).getN3();
			Vector t1 = (*tri).getT1(); Vector t2 = (*tri).getT2(); Vector t3 = (*tri).getT3();


				glBegin(GL_TRIANGLES);
				glNormal3f(n1.x, n1.y, n1.z);
				if(kdMap) glTexCoord2f(t1.x, t1.y);
				glVertex3f(p1.x, p1.y, p1.z);

				glNormal3f(n2.x, n2.y, n2.z);
				if(kdMap) glTexCoord2f(t2.x, t2.y);
				glVertex3f(p2.x, p2.y, p2.z);

				glNormal3f(n3.x, n3.y, n3.z);
				if(kdMap)  glTexCoord2f(t3.x, t3.y);
				glVertex3f(p3.x, p3.y, p3.z);
				glEnd();

				if(kdMap) 
					{
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_COLOR_MATERIAL);
					}
			}*/


		//vertex array mode
		for(int x = 0; x<=current;x++)
			{
			string currentMaterial = mtl[x];

			material mtl = this->materialList[currentMaterial];
			GLfloat ka[] = {mtl.getKa().x,mtl.getKa().y,mtl.getKa().z};
			GLfloat ks[] = {mtl.getKs().x,mtl.getKs().y,mtl.getKs().z};
			GLfloat kd[] = {mtl.getKd().x,mtl.getKd().y,mtl.getKd().z};
			GLfloat ns[] = {mtl.getNs()};

			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ns);

			GLuint kdMap = this->materialList[currentMaterial].getKdMap();

			if(kdMap)
				{
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glBindTexture(GL_TEXTURE_2D, kdMap);
				glEnable(GL_COLOR_MATERIAL);
				glColor3f(0.8,0.8,0.8);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				}


			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0,  varray[x]->data());
			glNormalPointer(GL_FLOAT, 0, vnarray[x]->data());

			if(kdMap) glTexCoordPointer(2, GL_FLOAT, 0, vtarray[x]->data());

			glDrawArrays(GL_TRIANGLES, 0, varray[x]->size()/3);


			if(kdMap) 
				{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_COLOR_MATERIAL);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			}
		}
	};
#endif