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
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/text2D.hpp>
#include <common/CameraView.hpp>
#include <common/Draw.hpp>

#include <iostream>
#include <list>

bool win = false;
bool lose = false;
bool menu = true;
GLuint programID;
GLuint TextureID;
GLuint MatrixID;
GLuint vertexPosition_modelspaceID ;
GLuint vertexUVID;
player yassin;
glm::mat4 ViewMatrix;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
glm::mat4 ProjectionMatrix;
GLuint * Texture = new GLuint [10];
GLuint vertexNormal_modelspaceID;
std::vector<glm::vec3> * vertices = new std::vector<glm::vec3> [10];
std::vector<glm::vec2> * uvs = new std::vector<glm::vec2> [10];
std::vector<glm::vec3> * normals = new std::vector<glm::vec3> [10];
GLuint * vertexbuffer = new GLuint [10];
GLuint * uvbuffer = new GLuint [10];
GLuint * normalbuffer = new GLuint [10];
bool inversethirdperson = false;
bool modelflag = true;
bool colloss = false;
bool faster = false;
bool movement = true;
bool jump = false;
bool assending = true;
int slice=0;
int maze1 = glfwGetKey(49);
int maze2 = glfwGetKey(50);
int maze3 = glfwGetKey(51);
int** createMatrix(int rows, int cols)
{
    int**mat = new int*[rows];
    for (int i = 0; i < rows; ++i)
        mat[i] = new int[cols];
    return mat;
}



bool check_colision(std::list<ModelObj> &Object , float &t)
{   bool re = false;
    float X1 = yassin.Place.position[0];
    float Y1 = yassin.Place.position[2];
    std::list<ModelObj>::iterator it = Object.begin();
    while( it != Object.end())
      {
        bool shouldRemove = false;
        float X = it->position[0];
        float Y = it->position[2];
        if((it->Length+yassin.Place.Length)>abs(X-X1) )
        {  if ((it->Length+yassin.Place.Length)>abs(Y-Y1))
             {  if((it->type != -1) && (it->type != -4) && (it->type != 8))
                   re = true;
                if(it->type==-2 || it->type==-6  )
                    lose=true;
                if(it->type== 1)
                {   if(colloss)
                    lose = true;}
                if (it->type==4)
                {
                    yassin.keys++;
                    cout << "I collected A Key, #Keys = " << yassin.keys << endl;
                    shouldRemove = true;
                }
                else if (it->type==9)
                    {
                    cout << "I gained 10 seconds" <<endl;
                    slice = slice + 10;
                    shouldRemove = true;
                    }
                else if (it->type==10)
                    {
                    cout << "I gained 20 seconds" <<endl;
                    slice = slice + 20;
                    shouldRemove = true;
                    }
                else if (it->type==5)
                    {
                       win= true;
                    }
                else if (it->type==6)
                    {
                       t +=5 ;
                       shouldRemove = true;
                    }
                 else if (it->type ==3)
                        {
                    if(yassin.keys)
                    {
                    cout << "I collected A Key, #Keys = " << yassin.keys << endl;
                    shouldRemove = true;
                    yassin.keys--;
                    }

                        }
                else if(it->type == 8)
                { shouldRemove = true;
                  faster = true; }
                if (it->type==-2 && yassin.Place.position[1] >1)
                {
                    re = false;
                    lose = false;
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
    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Create and compile our GLSL program from the shaders
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

    GLuint norm;
    glGenBuffers(1, &norm);
    glBindBuffer(GL_ARRAY_BUFFER, norm);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);

    Texture[i] = Text;
    vertices[i] = vert;
    uvs[i] = uv;

    normalbuffer[i] = norm;
    normals[i] = normal;

    vertexbuffer[i] = vertexbuf ;
    uvbuffer[i] = uvbuf;

    }
}

int main( )
{
 /*   ////////////////
    ALuint buffer, source;
        ALuint state;

        // Initialize the environment
        alutInit(0, NULL);

        // Capture errors
        alGetError();

        // Load pcm data into buffer
        buffer = alutCreateBufferFromFile("FILENAME");

        // Create sound source (use buffer to fill source)
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);

        // Play
        alSourcePlay(source);

        // Wait for the song to complete


        // Clean up sources and buffers
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);

        // Exit everything
        alutExit();
////////////////////////////////
    /// \brief v
///*/
    int v = 0;
    float time;
    cout << "Initializing..." << endl;
    Initialization() ;
    ProjectionMatrix = glm::perspective(30.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    std::string TextureName [11] ;
    std::string ModelName [11] ;

    ModelName[0] = "cubetest.obj";
    ModelName[1] = "cubetest.obj";
    ModelName[2] = "Bulbasaur1.obj";
    ModelName[3] = "cubetest.obj";
    ModelName[4] = "key.obj";
    ModelName[5] = "chest.obj";
    ModelName[6] = "coin.obj";
    ModelName[7] = "sphere.obj";
    ModelName[8] = "cubetest.obj";
    ModelName[9] = "coin.obj";
    ModelName[10] = "coin.obj";

    TextureName[0] = "Ground.bmp";
    TextureName[1] = "wall.bmp";
    TextureName[2] = "Bulba_D.bmp";
    TextureName[3] = "hazard.bmp";
    TextureName[4] = "Gold.bmp";
    TextureName[5] = "Texture.bmp";
    TextureName[6] = "coin.bmp";
    TextureName[7] = "Skysphere1.bmp";
    TextureName[8] = "ferrari-logo_0.bmp";
    TextureName[9] = "Colors.bmp";
    TextureName[10] = "Colors.bmp";


    cout << "Start Loading Models" << endl;
    ModelAndTextures( TextureName , ModelName , 10 );
    cout << "End Loading Models" << endl;
    yassin.keys=0;
    yassin.Place. Texture = &(Texture[2]);
    yassin.Place. uvbuffer = &(uvbuffer[2]);
    yassin.Place.normalbuffer = &(normalbuffer[2]);
    yassin.Place.Length = 0.315;
    yassin.Place. vertices = &(vertices[2]);
    yassin.Place. vertexbuffer = &(vertexbuffer[2]);
    yassin.Place.position =  glm::vec3(2.0*1.0f,1.0f,2.0*1.0f) ;
    yassin.Place.scale =  glm::vec3(0.7f,0.7f,0.7f) ;
    yassin.Place.rotation =  glm::vec3(0.0f,0.0f,0.0f) ;
    yassin.Place.normals=  &(normals[2]);
    yassin.Place.uvs= &(uvs[2]);

     float xp=0;
     float zp=0;
     float yp=0;
     cout << "I'm gonna read dem files" << endl;
     initText2D( "Holstein.tga" );
     string mazechoice ="";
     do{
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // Use our shader
         glUseProgram(programID);
     printText2D("Maze 1", 200, 350, 40);
     printText2D("Maze 2", 200, 300, 40);
     printText2D("Maze 3", 200, 250, 40);
     printText2D("Exit", 200, 200, 40);
     if(glfwGetKey(49) == GLFW_PRESS )
     {
      mazechoice="Maze1";

     }
     if(glfwGetKey(50) == GLFW_PRESS )
     {
      mazechoice="Maze2";

     }
     if(glfwGetKey(51) == GLFW_PRESS )
     {
      mazechoice="Maze3";

     }
     glfwSwapBuffers();
     }
     while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) && mazechoice == "" );
     if(mazechoice=="")
     return 0;
     ifstream in (mazechoice.c_str());
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
     int fog ;
     in>> fog;
     in.close();

      std::list<ModelObj> ground;
      std::list<ModelObj> WallsAndItems;
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
      sky . normalbuffer = &normalbuffer[7];

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

             Object_1 . normalbuffer = &(normalbuffer[0]);
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
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

                 Object_1.normals=  &(normals[abs(mat[i][j])]);
                 Object_1.uvs= &(uvs[abs(mat[i][j])]);
                 WallsAndItems.push_back (Object_1);

                 Object_1 . Texture = &Texture[mat[i][j]];
                 Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
                 Object_1 . vertices = &vertices[mat[i][j]];
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];
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
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

                 Object_1.Length = 1-0.315;
                 Object_1.normals=  &(normals[mat[i][j]]);
                 Object_1.uvs= &(uvs[mat[i][j]]);
                 WallsAndItems.push_back (Object_1);
             }
             else if(abs(mat[i][j])==1 || mat[i][j] ==3  )
             {
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

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
             else if(mat[i][j]==9 || mat[i][j]==10 )
             {
                 cout << "Found time bonus " << i << " " << j << endl;
                 Object_1.Texture=&Texture[mat[i][j]];
                 Object_1 . uvbuffer = &uvbuffer[mat[i][j]];
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];
                 Object_1 . vertices = &vertices[mat[i][j]];
                 Object_1 . vertexbuffer = &vertexbuffer[mat[i][j]];
                 Object_1.  position=glm::vec3(x,2.0f,z);
                 Object_1.rotation=glm::vec3(1.0f,0.0f,0.0f);
                 Object_1.scale=glm::vec3(2.0f,2.0f,2.0f);
                 Object_1.type = mat[i][j];
                 Object_1.Length = 0.36;
                 WallsAndItems.push_back (Object_1);
             }
             else if(abs(mat[i][j])==4 )
             {
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

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
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

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
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

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
                 Object_1 . normalbuffer = &normalbuffer[mat[i][j]];

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
             {
                 yassin.Place.position=glm::vec3(x,1.0f,z) ;
             }
       }
     }
     int cameraKeyLastState = !GLFW_PRESS;
     int inverseviewkeylast = !GLFW_PRESS;
     int modeldissapearlast = !GLFW_PRESS;
     int collosslastkey = !GLFW_PRESS;
     GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
     GLuint f = glGetUniformLocation(programID, "fog");

     glm::vec3 lightPos = yassin.Place.position;

     lightPos[0] = yassin.Place.position[0];
     lightPos[2] = yassin.Place.position[2];
     lightPos[1] = yassin.Place.position[1]+7;

     double lastTime = glfwGetTime();
     double lastFrameTime = lastTime;
     initText2D( "Holstein.tga" );

    do{
         double currentTime = glfwGetTime();
         float deltaTime = (float)(currentTime - lastFrameTime);
         lastFrameTime = currentTime;
         if ( currentTime - lastTime >= 1.0 ){
                 lastTime += 1.0;
                 if(time > 0)
                 {if (slice == 0)
                         time--;
                         else
                         {
                             time = time + slice;
                             slice = 0;
                             time--;
                         }
                 cout << "time remaining = " << time << " seconds" << endl;}
         }
         if(!time)
         {
             lose =true;
         }


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        glDisable(GL_CULL_FACE);
        sky.position = yassin.Place.position;
        DrawModel(  ProjectionMatrix , ViewMatrix ,sky ,TextureID, MatrixID, vertexPosition_modelspaceID, vertexUVID,vertexNormal_modelspaceID );
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);

        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        if (fog==0)
        {
           glUniform3f(f,1,1,1);
         }
        else if(fog==1)
        {
            glUniform3f(f,1,1,0);

        }
        else if (fog==2)
        {
            glUniform3f(f,1,0,0);
        }

        zp = 0;
        xp = 0;
        yp = 0;
        if(movement)
        {
         ComputeInputs(&xp, &zp, &jump, &assending, &v, &inversethirdperson, &modelflag, &colloss, faster);
        }

        Drawlist(ground, ProjectionMatrix,ViewMatrix, TextureID, MatrixID, vertexPosition_modelspaceID, vertexUVID ,vertexNormal_modelspaceID );
        Drawlist(WallsAndItems ,ProjectionMatrix,ViewMatrix, TextureID, MatrixID, vertexPosition_modelspaceID, vertexUVID,vertexNormal_modelspaceID );

        bool colis = true;
        yassin.Place.position = glm::vec3(cos(yassin.Place.rotation[0])*xp*deltaTime,0,0) + yassin.Place.position;

        if(check_colision(WallsAndItems,time)==true )
        {
           yassin.Place.position = -glm::vec3(cos(yassin.Place.rotation[0])*xp*deltaTime,0,0) + yassin.Place.position;
        }
        else
        {
             yassin.Place.rotation = glm::vec3(zp*deltaTime,0,0) + yassin.Place.rotation;
             colis=false;
        }

        yassin.Place.position = glm::vec3(0,0,sin(-yassin.Place.rotation[0])*xp*deltaTime) + yassin.Place.position;
        if (jump) {
        if(yassin.Place.position[1] <= 4 && assending )
        { if(yassin.Place.position[1] > 3.5 )
          assending = false;
        yassin.Place.position = glm::vec3(0,7*deltaTime,0) + yassin.Place.position;}
        if( yassin.Place.position[1] > 1 && !assending ) {
        yassin.Place.position = glm::vec3(0,-7*deltaTime,0) + yassin.Place.position;
        }
        if (yassin.Place.position[1] <= 1 && !assending){
        yassin.Place.position[1] = 1;
        jump = false;
        cout<<"bug"<<endl;}
        }
        if(check_colision(WallsAndItems,time)==true)
        {
           yassin.Place.position = -glm::vec3(0,0,sin(-yassin.Place.rotation[0])*xp*deltaTime) + yassin.Place.position;
        }
        else if(colis)
        {
             yassin.Place.rotation = glm::vec3(zp*deltaTime,0,0) + yassin.Place.rotation;
        }


        //cout << " yassin's keys " << yassin.keys << endl;

        if(v == 0)
         {
            Follow(yassin, inversethirdperson);
            ViewMatrix = getViewMatrix();
            ProjectionMatrix = getProjectionMatrix();
            sky.rotation=glm::vec3(0.0f,3.141526f,0.0f);
         }
        else if(v==1)
         {
            FollowFP(yassin);
            ViewMatrix = getViewMatrix();
            ProjectionMatrix = getProjectionMatrix();
            sky.rotation=glm::vec3(0.0f,3.141526f,0.0f);
         }
        else
         {
            FollowTop(yassin);
            ViewMatrix = getViewMatrix();
            ProjectionMatrix = getProjectionMatrix();
            sky.rotation=glm::vec3(0.0f,0.0f,0.0f);
            //sky.rotation=glm::vec3(0.0f,3.141526f,0.0f);
         }
        if(modelflag)
        DrawModel(  ProjectionMatrix , ViewMatrix ,yassin.Place, TextureID, MatrixID, vertexPosition_modelspaceID, vertexUVID,vertexNormal_modelspaceID  );
        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        char text[256];
        if(time == -1 )
        sprintf(text,"No timer");
        else
        sprintf(text,"%.f sec", time );
        printText2D(text, 630, 550, 20);
        sprintf(text,"keys:%d ", yassin.keys );
        printText2D(text, 10, 550, 20);
        if (win)
        { movement = false;
        printText2D("you have won", 200, 200, 40);
        }
        if (lose)
        { movement = false;
        printText2D("you have lost", 200, 200, 40);
        }

        glfwSwapBuffers(); // Swap buffers

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED ) );
    if(win==true)
    { cout<<"you have won"<<endl;
    }
    else if(lose == true)
    {
        cout<<"you have lost"<<endl;

    }


    // Cleanup VBO and shader
    glDeleteBuffers(3, vertexbuffer);
    glDeleteBuffers(3, uvbuffer);
    glDeleteProgram(programID);
    glDeleteBuffers(3,normalbuffer);
    glDeleteTextures(1, &TextureID);
    cleanupText2D();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

