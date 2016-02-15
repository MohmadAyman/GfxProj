// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"


// Initial position : on +Z
glm::vec3 position = glm::vec3( -5, 5,-5 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
int cameraKeyLastState = !GLFW_PRESS;
int inverseviewkeylast = !GLFW_PRESS;
int modeldissapearlast = !GLFW_PRESS;
int collosslastkey = !GLFW_PRESS;

void ComputeInputs(float *xp, float *zp, bool *jump, bool *assending, int *view, bool *inversethirdperson, bool *modelflag, bool *colloss, bool faster){

    int cameraKeyState = glfwGetKey(67);
    int inverseview = glfwGetKey(73);
    int modeldisappear = glfwGetKey(68);
    int collosskey = glfwGetKey(72);

    if (glfwGetKey( GLFW_KEY_LEFT) ==GLFW_PRESS ) //left arrow is pressed, so move the traingle left
    { if(faster)
        *zp+=7.5;
        else
        *zp+=2.5;
    }
    if (glfwGetKey( GLFW_KEY_RIGHT) ==GLFW_PRESS )
    {   if(faster)
            *zp-=7.5;
            else
        *zp+=-2.5;
    }

    if (glfwGetKey( GLFW_KEY_UP) ==GLFW_PRESS )
    {   if(faster)
          *xp+=15;
        else
        *xp+=5;
    }
    if (glfwGetKey( GLFW_KEY_DOWN) ==GLFW_PRESS )
    {    if(faster)
            *xp+=-15;
          else
         *xp+=-5;
    }
    if( glfwGetKey(90) == GLFW_PRESS )
    {    if(faster)
            *xp=*xp*4;
          else
         *xp = *xp*2;
    }
    if( glfwGetKey(GLFW_KEY_SPACE ) == GLFW_PRESS )
    {
        if(!*jump) //check if he is already jumping
        {
        *jump = true;
        *assending = true;
        }
    }
    if(cameraKeyState == GLFW_PRESS && cameraKeyLastState != GLFW_PRESS)
    {

          *view =(*view+1)%3;

    }
    cameraKeyLastState = cameraKeyState;

    if(inverseview == GLFW_PRESS &&  inverseviewkeylast!= GLFW_PRESS)
    {

          if(*inversethirdperson)
              *inversethirdperson = false;
          else
              *inversethirdperson = true;

    }
    inverseviewkeylast = inverseview;

    if(modeldisappear == GLFW_PRESS &&  modeldissapearlast!= GLFW_PRESS)
    {

          if(*modelflag)
              *modelflag= false;
          else
              *modelflag = true;

    }
    modeldissapearlast = modeldisappear;

    if(collosskey == GLFW_PRESS &&  collosslastkey!= GLFW_PRESS)
    {

          if(*colloss)
              *colloss= false;
          else
              *colloss = true;

    }
    collosslastkey = collosskey;

}
