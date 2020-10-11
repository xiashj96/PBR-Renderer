#include "Mesh.h"

void Mesh::Draw(const Shader& shader) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string TextureTypeStrings[6] = {
            "texture_diffuse",
            "texture_specular",
            "texture_normal",
            "texture_roughness",
            "texture_ambient",
            "texture_irridiance"
        };

        // set texture uniforms
        string uniformName = "material.";
        TextureType type = textures[i].getType();
        if (type == TextureType::Albedo)
            uniformName.append(TextureTypeStrings[static_cast<int>(type)] + std::to_string(diffuseNr++));
        else if (type == TextureType::Metallic)
            uniformName.append(TextureTypeStrings[static_cast<int>(type)] + std::to_string(specularNr++));
        else if (type == TextureType::Normal)
            uniformName.append(TextureTypeStrings[static_cast<int>(type)] + std::to_string(normalNr++));

        shader.SetInt(uniformName, i);
        textures[i].Bind(i);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glBindVertexArray(0);
}
