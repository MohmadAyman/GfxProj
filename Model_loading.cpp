// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
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
#include <common/controls.hpp>
#include <common/objloader.hpp>

#include <iostream>
#include <list>

bool win = false;
bool lose = false;

struct ModelObj
{
    GLuint * Texture;
    std::vector<glm::vec3> * normal;
    std::vector<glm::vec3> * vertices;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 position;
    GLuint * vertexbuffer;
    GLuint * uvbuffer;
    GLuint * normalbuffer;
    glm::vec2 * uvs;

    float Length;
    int type;
};

struct player
{
    ModelObj Place;
    int keys ;
};

GLuint programID;
GLuint TextureID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint vertexPosition_modelspaceID ;
GLuint vertexUVID;
GLuint vertexNormal_modelspaceID;
player yassin;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


 void Follow()
 {
     glm::vec3 Position =  yassin.Place.position + glm::vec3(cos(yassin.Place.rotation[0])*-4.0f,3.0f,sin(-yassin.Place.rotation[0])*-4.0f);
     glm::vec3 Lookat =yassin.Place.position+glm::vec3(0,1,0);
     glm::vec3 up =glm::vec3(0.0f,1.0f,0.0f);
     ViewMatrix  = glm::lookAt( Position , Lookat  , up );

 }

void DrawModel( glm::mat4 ProjectionMatrix , glm::mat4 ViewMatrix ,  ModelObj Object )
{

   glm::mat4 translation = translate(mat4(), glm::vec3(Object.position[0],Object.position[1],Object.position[2]));
   glm::mat4 RotationMatrix = eulerAngleYXZ(Object.rotation[0], Object.rotation[1], Object.rotation[2]);
   glm::mat4 ScalingMatrix = scale(mat4(), vec3(Object.scale[0], Object.scale[1], Object.scale[2]));

   glm::mat4 ModelMatrix = translation*RotationMatrix*ScalingMatrix;
   glm::mat4 MVP = ProjectionMatrix * ViewMatrix *ModelMatrix;

   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
   glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
   glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

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
   // 3rd attribute buffer : normals
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

void Drawlist(std::list<ModelObj> Object)
{
    for (std::list<ModelObj>::iterator it = Object.begin(); it != Object.end(); it++)
         DrawModel(  ProjectionMatrix , ViewMatrix ,*it );
}

int** createMatrix(int rows, int cols)
{
    int**mat = new int*[rows];
    for (int i = 0; i < rows; ++i)
        mat[i] = new int[cols];
    return mat;
}

GLuint * Texture = new GLuint [10];
std::vector<glm::vec3> * vertices = new std::vector<glm::vec3> [10];
std::vector<glm::vec2> * uvs = new std::vector<glm::vec2> [10];
std::vector<glm::vec3> * normal = new std::vector<glm::vec3> [10];
GLuint * vertexbuffer = new GLuint [10];
GLuint * uvbuffer = new GLuint [10];
GLuint * normalbuffer = new GLuint [10];

bool check_colision(std::list<ModelObj> &Object)
{   bool re = false;
    float X1 = yassin.Place.position[0];
    float Y1 = yassin.Place.position[2];
    std::list<ModelObj>::iterator it = Object.begin();
    while( it != Object.end())
      {
        bool shouldRemove = false;
        float X = it->position[0];
        float Y = it->position[2];
        if((it->Length+yassin.Place.Length)>abs(X-X1))
        {  if ((it->Length+yassin.Place.Length)>abs(Y-Y1))
             {   re = true;
                if (it->type==4)
                {
                    yassin.keys++;
                    cout << "I collected A Key, #Keys = " << yassin.keys << endl;
                    shouldRemove = true;
                }
                else if (it->type==5)
                    {
                       win= true;
                    }
                 else if (it->type==3)
                        {
                    if(yassin.keys)
                    {
                    cout << "I collected A Key, #Keys = " << yassin.keys << endl;
                    shouldRemove = true;
                    yassin.keys--;
                    }

                        }
             }
        }
        if(shouldRemove)
            it = Object.erase(it);
        else
            it++;
    }


    return re;
}

int Initialization()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetWindowTitle( "Model Loading" );

    // Ensure we can capture the escape key being pressed below
    glfwEnable( GLFW_STICKY_KEYS );
    glfwSetMousePos(1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Create and compile our GLSL program from the shaders
//    programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
      programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");

    // Get a handle for our buffers
    vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    vertexUVID = glGetAttribLocation(programID, "vertexUV");
    vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");
    // Get a handle for our "myTextureSampler" uniform
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");
}

void ModelAndTextures ( std::string* TextureName , std::string * ModelName , int number )
{
    for (int i = 0 ; i < number ; i++)
    {

    cout << "I loading Texture[" << i << "]: " << TextureName[i] << endl;
    GLuint Text = loadBMP_custom(TextureName[i].c_str());
    cout << "I loaded Texture[" << i << "]: " << TextureName[i] << endl;

    // Read our .obj file
    std::vector<glm::vec3> vert;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals; // Won't be used at the moment.

    cout << "I loading Model[" << i << "]: " << ModelName[i] << endl;
    bool res = loadOBJ(ModelName[i].c_str(), vert, uv, normals);
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

    GLuint norm;
    glGenBuffers(1, &norm);
    glBindBuffer(GL_ARRAY_BUFFER, norm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    Texture[i] = Text;
    vertices[i] = vert;
    uvs[i] = uv;
    normal[i] = normals;

    normalbuffer[i] = norm;
    vertexbuffer[i] = vertexbuf ;
    uvbuffer[i] = uvbuf;

    }
}

int main( )
{
    int time;
    cout << "Initializing..." << endl;
    Initialization() ;
    ProjectionMatrix = glm::perspective(30.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    std::string TextureName [6] ;
    std::string ModelName [6] ;

    ModelName[0] = "cubetest.obj";
    ModelName[1] = "cubetest.obj";
    ModelName[2] = "Bulbasaur1.obj";
    ModelName[3] = "cubetest.obj";
    ModelName[4] = "key.obj";
    ModelName[5] = "coin.obj";

    TextureName[0] = "Ground.bmp";
    TextureName[1] = "wall.bmp";
    TextureName[2] = "Bulba_D.bmp";
    TextureName[3] = "hazard.bmp";
    TextureName[4] = "Gold.bmp";
    TextureName[5] = "coin.bmp";


    cout << "Start Loading Models" << endl;
    ModelAndTextures( TextureName , ModelName , 6 );
    cout << "End Loading Models" << endl;
    yassin.keys=0;
    yassin.Place. Texture = &(Texture[2]);
    yassin.Place. uvbuffer = &(uvbuffer[2]);
    yassin.Place.Length = 0.315;
    yassin.Place.normalbuffer = &(normalbuffer[2]);
    yassin.Place. vertices = &(vertices[2]);
    yassin.Place. vertexbuffer = &(vertexbuffer[2]);
    yassin.Place.position =  glm::vec3(2.0*1.0f,1.0f,2.0*1.0f) ;
    yassin.Place.scale =  glm::vec3(0.7f,0.7f,0.7f) ;
    yassin.Place.rotation =  glm::vec3(0.0f,0.0f,0.0f) ;

     float xp=0;
     float zp=0;

     cout << "I'm gonna read dem files" << endl;
     ifstream in ("MAZE");
     cout << "yeah yeah yeah yeah" << endl;
     int length;
     int width;
       if (in.is_open())
       {
           in >> length;
           in >> width;
       }

       cout << "pfft it's just " << length << "x" << width << endl;
     // Camera matrix
     int ** mat = createMatrix(length, width);

     for(int i = 0 ; i <length ; i++)
     {
        for(int j = 0 ; j <width ; j++)
        {
         in >> mat[i][j];
        }
     }

     in>>time;
     in.close();


     std::list<ModelObj> ground;
      std::list<ModelObj> WallsAndItems;
      ModelObj Object_1;

      for(int i = 0 ; i<length ; i++)
     {

         for(int j = 0 ; j<width ; j++)
         {
             float x = i*2;
             float z = j*2;
             Object_1 . Texture = &(Texture[0]);
             Object_1 . uvbuffer = &(uvbuffer[0]);
             Object_1 . normalbuffer = &(normalbuffer[0]);
             Object_1 . vertices = &vertices[0];
             Object_1 . vertexbuffer = &vertexbuffer[0];
             Object_1.position=glm::vec3(x,0.0f,z);
             Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
             Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
             Object_1.type = 0;
             Object_1.Length = 1;
             ground.push_back (Object_1);
             if(mat[i][j]==1 || mat[i][j]==3  )
             {
                 Object_1 . Texture = &Texture[mat[i][j]];
                 Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
                 Object_1 . vertices = &vertices[mat[i][j]];
                 Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];
                 Object_1.position=glm::vec3(x,2.0f,z);
                 Object_1.rotation=glm::vec3(0.0f,0.0f,0.0f);
                 Object_1.scale=glm::vec3(1.0f,1.0f,1.0f);
                 Object_1.type = mat[i][j];
                 Object_1.Length = 1;
                 WallsAndItems.push_back (Object_1);
             }
             else if(mat[i][j]==4 )
             {
                 cout << "found a key " << i << " " << j << endl;
                 Object_1.Texture=&Texture[mat[i][j]];
                 Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];
                 Object_1 . vertices = &vertices[mat[i][j]];
                 Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
                 Object_1.position=glm::vec3(x,2.0f,z);
                 Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
                 Object_1.scale=glm::vec3(0.5f,0.5f,0.5f);
                 Object_1.type = mat[i][j];
                 Object_1.Length = 0.36;
                 WallsAndItems.push_back (Object_1);
             }
             else if(mat[i][j]==5 )
             {
                 Object_1 . Texture = &Texture[mat[i][j]];
                 Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
                 Object_1 . vertices = &vertices[mat[i][j]];
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];
                 Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
                 Object_1.position=glm::vec3(x,2.0f,z);
                 Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
                 Object_1.scale=glm::vec3(2.0f,2.0f,2.0f);
                 Object_1.Length = 0.4;
                 Object_1.type = mat[i][j];
                 WallsAndItems.push_back (Object_1);
             }

             else if(mat[i][j]==2)
             {
                 yassin.Place.position=glm::vec3(x,1.0f,z) ;
             }
       }
     }

     double lastTime = glfwGetTime();
     double lastFrameTime = lastTime;
     GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
     GLuint LightID2 = glGetUniformLocation(programID, "LightPosition_2_worldspace");

    do{

         double currentTime = glfwGetTime();
         float deltaTime = (float)(currentTime - lastFrameTime);
         lastFrameTime = currentTime;
         if ( currentTime - lastTime >= 1.0 ){
                 lastTime += 1.0;
                 time--;
                 cout << "time remaining = " << time << " seconds" << endl;
         }
         if(!time)
         {
             lose =true;
         }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform

        glm::vec3 lightPos = glm::vec3(3.0f,2.0f,3.0f);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        glm::vec3 lightPos2 = glm::vec3(2.0f,1.0f,2.0f);
        glUniform3f(LightID2, lightPos2.x, lightPos2.y, lightPos2.z);

        zp = 0;
        xp=0;

        if (glfwGetKey( GLFW_KEY_LEFT) ==GLFW_PRESS ) //left arrow is pressed, so move the traingle left
        {
            zp+=1;
        }
        if (glfwGetKey( GLFW_KEY_RIGHT) ==GLFW_PRESS )
        {
            zp+=-1;
        }

        if (glfwGetKey( GLFW_KEY_UP) ==GLFW_PRESS )
        {
            xp+=5;
        }
        if (glfwGetKey( GLFW_KEY_DOWN) ==GLFW_PRESS )
        {
             xp+=-5;
        }


        Drawlist(ground);
        Drawlist(WallsAndItems);

        Follow();

        yassin.Place.position = glm::vec3(cos(yassin.Place.rotation[0])*xp*deltaTime,0,sin(-yassin.Place.rotation[0])*xp*deltaTime) + yassin.Place.position;

        if(check_colision(WallsAndItems)==true)
        {
           yassin.Place.position = -glm::vec3(cos(yassin.Place.rotation[0])*xp*deltaTime,0,sin(-yassin.Place.rotation[0])*xp*deltaTime) + yassin.Place.position;
        }
        else
        {
             yassin.Place.rotation = glm::vec3(zp*deltaTime,0,0) + yassin.Place.rotation;
        }
        //cout << " yassin's keys " << yassin.keys << endl;
        DrawModel(  ProjectionMatrix , ViewMatrix ,yassin.Place );


        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);

        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED ) && (!win && !lose));
    if(win==true)
    { cout<<"you have won"<<endl;
    }
    else if(lose == true)
    {
        cout<<"you have lost"<<endl;

    }


    // Cleanup VBO and shader
    glDeleteBuffers(3, normalbuffer);
    glDeleteBuffers(3, vertexbuffer);
    glDeleteBuffers(3, uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

