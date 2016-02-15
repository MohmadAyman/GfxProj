#ifndef DRAW_HPP
#define DRAW_HPP

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

void Drawlist(std::list<ModelObj> Object, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint TextureID ,GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID,GLuint vertexNormal_modelspaceID);
void DrawModel( glm::mat4 ProjectionMatrix , glm::mat4 ViewMatrix ,  ModelObj Object , GLuint TextureID ,GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID,GLuint vertexNormal_modelspaceID);


#endif
