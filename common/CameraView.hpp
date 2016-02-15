#ifndef CAMERAVIEW_HPP
#define CAMERAVIEW_HPP

#include <vector>

using namespace std;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "Global.hpp"

using namespace glm;



void FollowFP(player yassin);
void Follow(player yassin, bool inversethirdperson);
void FollowTop(player yassin);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
#endif
