#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "vect.h"


void loadScene(std::string file) {
    
    //store variables and set stuff at the end
    int width, height;
    std::string fname = "output.bmp";
    
    int curvecount;
    vector<vect*> curves;
    
    std::ifstream inpfile(file.c_str());
    if(!inpfile.is_open()) {
        std::cout << "Unable to open file" << std::endl;
    } else {
        std::string line;
        //MatrixStack mst;
        
        while(inpfile.good()) {
            std::vector<std::string> splitline;
            std::string buf;
            
            std::getline(inpfile,line);
            std::stringstream ss(line);
            
            while (ss >> buf) {
                splitline.push_back(buf);
            }
            //Ignore blank lines
            if(splitline.size() == 0) {
                continue;
            }
            
            //Ignore comments
            if(splitline[0][0] == '#') {
                continue;
            }
            else if(splitline.size() == 1) {
                curvecount = atoi(splitline[0].c_str());
            }
            //Vertices
            else if(splitline.size() == 12) {
                vect p1 = vect(atof(splitline[0].c_str()), atof(splitline[1].c_str()), atof(splitline[2].c_str()), 1);
                vect p2 = vect(atof(splitline[3].c_str()), atof(splitline[4].c_str()), atof(splitline[5].c_str()), 1);
                vect p3 = vect(atof(splitline[6].c_str()), atof(splitline[7].c_str()), atof(splitline[8].c_str()), 1);
                vect p4 = vect(atof(splitline[9].c_str()), atof(splitline[10].c_str()), atof(splitline[11].c_str()), 1);
                vect curve[4] = {p1, p2, p3, p4};
                curves.push_back(curve);
            }
            else {
                std::cerr << "Unknown command: " << splitline[0] << std::endl;
            }
        }
        
        inpfile.close();
    }
    
}

