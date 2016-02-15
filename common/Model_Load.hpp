#ifndef MODEL_LOAD_HPP
#define MODEL_LOAD_HPP

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
#include <common/Global.hpp>

std::string TextureName [9] ;
std::string ModelName [9] ;
GLuint * Texture = new GLuint [10];
std::vector<glm::vec3> * vertices = new std::vector<glm::vec3> [10];
std::vector<glm::vec2> * uvs = new std::vector<glm::vec2> [10];
std::vector<glm::vec3> * normals = new std::vector<glm::vec3> [10];
GLuint * vertexbuffer = new GLuint [10];
GLuint * uvbuffer = new GLuint [10];

void ModelAndTextures();
void init_ModelAndTextures();
void LoadModels(int** mat,
                int width,
                int length,
                std::list<ModelObj>* ground,
                std::list<ModelObj>* WallsAndItems,
                float* xx,
                float* zz);


#endif
