#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace std;
#include "Shader.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    glm::vec3 Tangent;
};

class Mesh
{
public:
    Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture*>& textures)
        :vertices(vertices), indices(indices), textures(textures)
    {
        setupMesh();
    }

    // bind all textures and render, set uniforms before calling this function
    void Draw(const Shader& shader) const;
private:
    void setupMesh();

    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture*>     textures;

    unsigned int VAO, VBO, EBO;
};

