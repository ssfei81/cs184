#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int main()
{
int type,count = 0;
string filename,outfile,mtlfile;

cout<<"File Name:";
cin>>filename;


cout<<"Output File Name:";
cin>>outfile;
ofstream fout(outfile);

fout<<"size 640 400"<<endl;
fout<<"camera 100 5 20 0 0 -1 0 1 0 45"<<endl;
fout<<"output output.bmp"<<endl;

fout<<"diffuse 0.2 0.5 0.2"<<endl;
fout<<"ambient 0.0 0.0 0.0"<<endl;
fout<<"directional 5 5 5 1 1 1"<<endl;
fout<<"shininess 20"<<endl;
fout<<"specular 0.2 0.2 0.2"<<endl;
fout<<"emission 0.2 0.2 0.2"<<endl;
fout<<"maxdepth 0"<<endl;
fout<<"pushTransform"<<endl;


	ifstream inpfile(filename.c_str());
	fout<<"vertex 0 0 0"<<endl;
	std::string line;
	while(inpfile.good()) {
			  std::vector<std::string> splitline;
			  std::string buf;

			  std::getline(inpfile,line);
			  std::stringstream ss(line);

			  while (ss >> buf) 
				{
				splitline.push_back(buf);
				}
			  //Ignore blank lines
			  if(splitline.size() == 0) 
				{
				continue;
				}

			  //Ignore comments
			  if(splitline[0][0] == '#') {
				continue;
			  }
			  else if(!splitline[0].compare("mtllib")) 
				{
				mtlfile = splitline[1];
				cout<<mtlfile.c_str()<<endl;
			    }
			  else if(!splitline[0].compare("usemtl")) 
				{
				ifstream inpfile2(mtlfile.c_str());
				std::string line2;
				bool startWriting = false;
				while(inpfile2.good()) 
					{
						std::vector<std::string> splitline2;
						std::string buf2;

						std::getline(inpfile2,line2);
						std::stringstream ss3(line2);

					while (ss3 >> buf2) 
						{
						splitline2.push_back(buf2);
						}
						//Ignore blank lines
						if(splitline2.size() == 0) 
						{
						continue;
						}
					else if(!splitline2[0].compare("newmtl")) 
						{
						if(!splitline[1].compare(splitline2[1])) startWriting = true;
						else startWriting = false;
						}
					else if(!splitline2[0].compare("Ka")) 
						{
						if(startWriting)
							{
							fout<<"emission "<<splitline2[1]<<" "<<splitline2[2]<<" "<<splitline2[3]<<endl;
							}
						}
					else if(!splitline2[0].compare("Ks")) 
						{
						if(startWriting)
							{
							fout<<"specular "<<splitline2[1]<<" "<<splitline2[2]<<" "<<splitline2[3]<<endl;
							}
						}
					else if(!splitline2[0].compare("Kd")) 
						{
						if(startWriting)
							{
							fout<<"diffuse "<<splitline2[1]<<" "<<splitline2[2]<<" "<<splitline2[3]<<endl;
							}
						}
					else if(!splitline2[0].compare("Ns")) 
						{
						if(startWriting)
							{
							fout<<"shininess "<<splitline2[1]<<endl;
							}
						}
					}
				inpfile2.close();
					
			    }

			  else if(!splitline[0].compare("v")) 
				{
				float v1 = (float)atof(splitline[1].c_str());
				float v2 = (float)atof(splitline[2].c_str());
				float v3 = (float)atof(splitline[3].c_str());
				fout<<"vertex "<<v1<<" "<<v2<<" "<<v3<<endl;
				//count++;
				}

			  else if(!splitline[0].compare("f")) 
				{
				
				string delimiter = "/", token;
				if(splitline.size()==4)
					{
					fout<<"tri ";
					token = splitline[1].substr(0, splitline[1].find(delimiter));
					fout<<token<<" ";
					token = splitline[2].substr(0, splitline[2].find(delimiter));
					fout<<token<<" ";
					token = splitline[3].substr(0, splitline[3].find(delimiter));
					fout<<token<<endl;
					}
				else
					{
					string token1, token2, token3,token4;
					
					token1 = splitline[1].substr(0, splitline[1].find(delimiter));
					fout<<token<<" ";
					token2 = splitline[2].substr(0, splitline[2].find(delimiter));
					fout<<token<<" ";
					token3 = splitline[3].substr(0, splitline[3].find(delimiter));
					fout<<token<<" ";
					token4 = splitline[4].substr(0, splitline[4].find(delimiter));
					fout<<"tri "<<token1<<" "<<token2<<" "<<token4<<endl;
					fout<<"tri "<<token2<<" "<<token3<<" "<<token4<<endl;
					}

				count++;
				}
			  else if(!splitline[0].compare("usemtl")) 
				{
					fout<<"#mtl "<<splitline[1].c_str()<<endl;
			    }
			  else
				{
				continue;
				//std::cerr << "Unknown command: " << splitline[0] << std::endl;
				}
			}
		   inpfile.close();


/*
ifstream inpfile2(filename.c_str());
	while(inpfile2.good()) {
			  std::vector<std::string> splitline;
			  std::string buf;

			  std::getline(inpfile2,line);
			  std::stringstream ss2(line);

			  while (ss2 >> buf) 
				{
				splitline.push_back(buf);
				}
			  //Ignore blank lines
			  if(splitline.size() == 0) 
				{
				continue;
				}

			  //Ignore comments
			  if(splitline[0][0] == '#') {
				continue;
			  }

			  else
				{
				continue;
				//std::cerr << "Unknown command: " << splitline[0] << std::endl;
				}
			}
		   inpfile2.close();
*/

		   cout<<count<<endl;	
system("pause");
return 0;
}