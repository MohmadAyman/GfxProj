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

#include "Draw.hpp"


void DrawModel( glm::mat4 ProjectionMatrix , glm::mat4 ViewMatrix ,  ModelObj Object , GLuint TextureID ,GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID,GLuint vertexNormal_modelspaceID)
{

   glm::mat4 translation = translate(mat4(), glm::vec3(Object.position[0],Object.position[1],Object.position[2]));
   glm::mat4 RotationMatrix = eulerAngleYXZ(Object.rotation[0], Object.rotation[1], Object.rotation[2]);
   glm::mat4 ScalingMatrix = scale(mat4(), vec3(Object.scale[0], Object.scale[1], Object.scale[2]));

   glm::mat4 ModelMatrix = translation*RotationMatrix*ScalingMatrix;
   glm::mat4 MVP = ProjectionMatrix * ViewMatrix *ModelMatrix;
   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

   // Bind our texture in Texture Unit 0
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, *(Object.Texture));
   // Set our "myTextureSampler" sampler to user Texture Unit 0
   glUniform1i(TextureID, 0);

   // 1rst attribute buffer : vertices
   glEnableVertexAttribArray(vertexPosition_modelspaceID);
   glBindBuffer(GL_ARRAY_BUFFER, *(Object.vertexbuffer));
   glVertexAttribPointer(
       vertexPosition_modelspaceID,  // The attribute we want to configure
       3,                            // size
       GL_FLOAT,                     // type
       GL_FALSE,                     // normalized?
       0,                            // stride
       (void*)0                      // array buffer offset
   );

   // 2nd attribute buffer : UVs
   glEnableVertexAttribArray(vertexUVID);
   glBindBuffer(GL_ARRAY_BUFFER, *(Object.uvbuffer));
   glVertexAttribPointer(
       vertexUVID,                   // The attribute we want to configure
       2,                            // size : U+V => 2
       GL_FLOAT,                     // type
       GL_FALSE,                     // normalized?
       0,                            // stride
       (void*)0                      // array buffer offset
   );

   glEnableVertexAttribArray(vertexNormal_modelspaceID);
   glBindBuffer(GL_ARRAY_BUFFER, *(Object. normalbuffer));
   glVertexAttribPointer(
       vertexNormal_modelspaceID,    // The attribute we want to configure
       3,                            // size
       GL_FLOAT,                     // type
       GL_FALSE,                     // normalized?
       0,                            // stride
       (void*)0                      // array buffer offset
   );

   glDrawArrays(GL_TRIANGLES, 0, (*(Object.vertices)).size() );

}
void Drawlist(std::list<ModelObj> Object, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint TextureID ,GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID,GLuint vertexNormal_modelspaceID)
{
    for (std::list<ModelObj>::iterator it = Object.begin(); it != Object.end(); it++)
    {   if(it->type==-2)
            continue;
        DrawModel(  ProjectionMatrix , ViewMatrix ,*it,TextureID, MatrixID, vertexPosition_modelspaceID, vertexUVID,vertexNormal_modelspaceID  );}
}

