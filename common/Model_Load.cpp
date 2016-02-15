// Include standard headers
/*#include <stdio.h>
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
#include "objloader.hpp"

void init_ModelAndTextures(){
ModelName[0] = "Models/cubetest.obj";
ModelName[1] = "Models/cubetest.obj";
ModelName[2] = "Models/Bulbasaur1.obj";
ModelName[3] = "Models/cubetest.obj";
ModelName[4] = "Models/key.obj";
ModelName[5] = "Models/chest.obj";
ModelName[6] = "Models/coin.obj";
ModelName[7] = "Models/sphere.obj";
ModelName[8] = "Models/cubetest.obj";
TextureName[0] = "Textures/Ground.bmp";
TextureName[1] = "Textures/wall.bmp";
TextureName[2] = "Textures/Bulba_D.bmp";
TextureName[3] = "Textures/hazard.bmp";
TextureName[4] = "Textures/Gold.bmp";
TextureName[5] = "Textures/Texture.bmp";
TextureName[6] = "Textures/coin.bmp";
TextureName[7] = "Textures/Skysphere1.bmp";
TextureName[8] = "Textures/ferrari-logo_0.bmp"; }

void ModelAndTextures ()
{
    init_ModelAndTextures();
    for (int i = 0 ; i < 9 ; i++)
    {

    cout << "I loading Texture[" << i << "]: " << TextureName[i] << endl;
    GLuint Text = loadBMP_custom(TextureName[i].c_str());
    cout << "I loaded Texture[" << i << "]: " << TextureName[i] << endl;

    // Read our .obj file
    std::vector<glm::vec3> vert;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normal; // Won't be used at the moment.

    cout << "I loading Model[" << i << "]: " << ModelName[i] << endl;
    bool res = loadOBJ(ModelName[i].c_str(), vert, uv, normal);
    cout << "I loaded Model[" << i << "]: " << ModelName[i] << endl;

    // Load it into a VBO

    GLuint vertexbuf;
    glGenBuffers(1, &vertexbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuf);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), &vert[0], GL_STATIC_DRAW);

    GLuint uvbuf;
    glGenBuffers(1, &uvbuf);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuf);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

    Texture[i] = Text;
    vertices[i] = vert;
    uvs[i] = uv;
    normals[i] = normal;
    vertexbuffer[i] = vertexbuf ;
    uvbuffer[i] = uvbuf;

    }
}

void LoadModels(int** mat,
                int width,
                int length,
                std::list<ModelObj> ground,
                std::list<ModelObj> WallsAndItems, float *xx, float *zz){
    ModelObj Object_1;
    ModelObj sky;
    sky . Texture = &(Texture[7]);
    sky . uvbuffer = &(uvbuffer[7]);
    sky . vertices = &vertices[7];
    sky . vertexbuffer = &vertexbuffer[7];
    sky.position=glm::vec3(0.0f,0.0f,0.0f);
    sky.rotation=glm::vec3(0.0f,3.141526f,0.0f);
    sky.scale= 100.0f * glm::vec3(1.0f,1.0f,1.0f);
    sky.type = 0;
    sky.Length = 1;
    sky.normals=  &(normals[7]);
    sky.uvs= &(uvs[7]);


   for(int i = 0 ; i<length ; i++)
   {

       for(int j = 0 ; j<width ; j++)
       {

           float x = i*2;
           float z = j*2;
           if( (mat[i][j] != -3) && (mat[i][j] != -2) )

           {  Object_1 . Texture = &(Texture[0]);
           Object_1 . uvbuffer = &(uvbuffer[0]);
           Object_1 . vertices = &vertices[0];
           Object_1 . vertexbuffer = &vertexbuffer[0];
           Object_1.position=glm::vec3(x,0.75f,z);
           Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
           Object_1.scale=glm::vec3(1.0f,0.25f,1.0f);
           Object_1.type = 0;
           Object_1.Length = 1;
           Object_1.normals=  &(normals[0]);
           Object_1.uvs= &(uvs[0]);
            ground.push_back (Object_1);}
           if(mat[i][j] == -3 )
           {
               Object_1 . Texture = &Texture[abs(mat[i][j])];
               Object_1 . uvbuffer = &uvbuffer[abs(mat[i][j])];
               Object_1 . vertices = &vertices[abs(mat[i][j])];
               Object_1 . vertexbuffer = &vertexbuffer[abs(mat[i][j])];
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
               Object_1.type = -mat[i][j];
               Object_1.Length = 1;
               Object_1.normals=  &(normals[abs(mat[i][j])]);
               Object_1.uvs= &(uvs[abs(mat[i][j])]);
               WallsAndItems.push_back (Object_1);

               Object_1 . Texture = &Texture[mat[i][j]];
               Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
               Object_1 . vertices = &vertices[mat[i][j]];
               Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
               Object_1.type = -2;
               Object_1.Length = 1-0.315;
               Object_1.normals=  &(normals[mat[i][j]]);
               Object_1.uvs= &(uvs[mat[i][j]]);
               WallsAndItems.push_back (Object_1);

           }
           else if(mat[i][j]==-2)
           {
               Object_1 . Texture = &Texture[mat[i][j]];
               Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
               Object_1 . vertices = &vertices[mat[i][j]];
               Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
               Object_1.type = -2;
               Object_1.Length = 1-0.315;
               Object_1.normals=  &(normals[mat[i][j]]);
               Object_1.uvs= &(uvs[mat[i][j]]);
               WallsAndItems.push_back (Object_1);
           }
           else if(abs(mat[i][j])==1 || mat[i][j] ==3  )
           {
               Object_1 . Texture = &Texture[abs(mat[i][j])];
               Object_1 . uvbuffer = &uvbuffer[abs(mat[i][j])];
               Object_1 . vertices = &vertices[abs(mat[i][j])];
               Object_1 . vertexbuffer = &vertexbuffer[abs(mat[i][j])];
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
               Object_1.type = mat[i][j];
               Object_1.Length = 1;
               Object_1.normals=  &(normals[abs(mat[i][j])]);
               Object_1.uvs= &(uvs[abs(mat[i][j])]);
               WallsAndItems.push_back (Object_1);
           }
           else if(abs(mat[i][j])==4 )
           {

               Object_1.Texture=&Texture[abs(mat[i][j])];
               Object_1 . uvbuffer = &uvbuffer[abs(mat[i][j])];
               Object_1 . vertices = &vertices[abs(mat[i][j])];
               Object_1 . vertexbuffer = &vertexbuffer[abs(mat[i][j])];
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(0.5f,0.5f,0.5f);
               Object_1.type = mat[i][j];
               Object_1.Length = 0.36;
               Object_1.normals=  &(normals[abs(mat[i][j])]);
               Object_1.uvs= &(uvs[abs(mat[i][j])]);
               WallsAndItems.push_back (Object_1);
           }
           else if(abs(mat[i][j])==5 )
           {
               Object_1 . Texture = &Texture[mat[i][j]];
               Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
               Object_1 . vertices = &vertices[mat[i][j]];
               Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
               Object_1.normals=  &(normals[mat[i][j]]);
               Object_1.uvs= &(uvs[mat[i][j]]);
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
               Object_1.Length = 1;
               Object_1.type = mat[i][j];
               WallsAndItems.push_back (Object_1);
           }
           else if(abs(mat[i][j])== 6 )
           {
               Object_1 . Texture = &Texture[abs(mat[i][j])];
               Object_1 . uvbuffer = &uvbuffer[abs(mat[i][j])];
               Object_1 . vertices = &vertices[abs(mat[i][j])];
               Object_1 . vertexbuffer = &vertexbuffer[abs(mat[i][j])];
               Object_1.normals=  &(normals[abs(mat[i][j])]);
               Object_1.uvs= &(uvs[abs(mat[i][j])]);
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(1.5f,1.5f,1.5f);
               Object_1.Length = 0.3;
               Object_1.type = mat[i][j];
               WallsAndItems.push_back (Object_1);
           }
           else if(abs(mat[i][j])== 8 )
           {
               Object_1 . Texture = &Texture[abs(mat[i][j])];
               Object_1 . uvbuffer = &uvbuffer[abs(mat[i][j])];
               Object_1 . vertices = &vertices[abs(mat[i][j])];
               Object_1 . vertexbuffer = &vertexbuffer[abs(mat[i][j])];
               Object_1.normals=  &(normals[abs(mat[i][j])]);
               Object_1.uvs= &(uvs[abs(mat[i][j])]);
               Object_1.position=glm::vec3(x,2.0f,z);
               Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
               Object_1.scale=glm::vec3(0.6f,0.6f,0.6f);
               Object_1.Length = 0.3;
               Object_1.type = mat[i][j];
               WallsAndItems.push_back (Object_1);
           }
           else if(mat[i][j]==2)
           {         *xx=x;
                     *zz=z;
           }
     }
   }
}
*/
