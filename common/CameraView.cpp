// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "CameraView.hpp"

glm::mat4 ViewMat;
glm::mat4 ProjectionMat;


void FollowFP(player yassin)
{
    glm::vec3 Position =  yassin.Place.position + glm::vec3(cos(yassin.Place.rotation[0])*0.1f,1.0f,sin(-yassin.Place.rotation[0])*0.1f);
    glm::vec3 Lookat =yassin.Place.position+glm::vec3(cos(yassin.Place.rotation[0])*2,1,sin(-yassin.Place.rotation[0])*2);
    glm::vec3 up =glm::vec3(0.0f,1.0f,0.0f);
    ViewMat = glm::lookAt( Position , Lookat  , up );
    ProjectionMat = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 200.0f);
}

void Follow(player yassin, bool inversethirdperson)//camera third person
 {
     glm::vec3 Position =  yassin.Place.position + glm::vec3(cos(yassin.Place.rotation[0])*-2.0f,2.5f,sin(-yassin.Place.rotation[0])*-2.0f);
     glm::vec3 Lookat =yassin.Place.position+glm::vec3(0,1,0);
     glm::vec3 up =glm::vec3(0.0f,1.0f,0.0f);
     if(!inversethirdperson)
          ViewMat = glm::lookAt( Position , Lookat  , up );
          else
          ViewMat = glm::lookAt( Position , Lookat  ,-up );
     ProjectionMat = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 200.0f);

 }

void FollowTop(player yassin)//camera top view
 {
     glm::vec3 Position =  yassin.Place.position + glm::vec3(0,10.0f,0);
     glm::vec3 Lookat =yassin.Place.position;
     glm::vec3 up =glm::vec3(cos(yassin.Place.rotation[0]),0.0f,sin(-yassin.Place.rotation[0]));
     ViewMat  = glm::lookAt( Position , Lookat  , up );
     ProjectionMat= glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 200.0f);

 }

glm::mat4 getViewMatrix(){
    return ViewMat;
}

glm::mat4 getProjectionMatrix(){
    return ProjectionMat;
}


