#ifndef GLOBAL_HPP
#define GLOBAL_HPP


struct ModelObj
{
    GLuint * Texture;
    std::vector<glm::vec3> * vertices;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 position;
    GLuint * vertexbuffer;
    GLuint * normalbuffer;
    GLuint * uvbuffer;
    std::vector<glm::vec3>  * normals;
    std::vector<glm::vec2>  * uvs;

    float Length;
    int type;
};

struct player
{
    ModelObj Place;
    int keys ;
};

#endif
