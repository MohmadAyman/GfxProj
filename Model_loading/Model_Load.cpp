// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
// Include GLEW
#include <GL/glew.h>
#include <fstream>
using namespace std;
// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <iostream>
#include <list>

#include "Model_Load.hpp"
void init_ModelAndTextures(){
ModelName[0] = "cubetest.obj";
ModelName[1] = "cubetest.obj";
ModelName[2] = "Bulbasaur1.obj";
ModelName[3] = "cubetest.obj";
ModelName[4] = "key.obj";
ModelName[5] = "chest.obj";
ModelName[6] = "coin.obj";
ModelName[7] = "sphere.obj";
ModelName[8] = "cubetest.obj";
TextureName[0] = "Ground.bmp";
TextureName[1] = "wall.bmp";
TextureName[2] = "Bulba_D.bmp";
TextureName[3] = "hazard.bmp";
TextureName[4] = "Gold.bmp";
TextureName[5] = "Texture.bmp";
TextureName[6] = "coin.bmp";
TextureName[7] = "Skysphere1.bmp";
TextureName[8] = "ferrari-logo_0.bmp"; }
