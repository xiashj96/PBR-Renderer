#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Shader.h"
#include "Cube.h"
#include "Camera.h"
#include "Cubemap.h"

class Skybox {
public:
    // better draw skybox last because it has maximum depth
    static void Draw(const Cubemap& cubemap, const Shader& shader, const Camera& camera)
    {
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CW);
        shader.Use();
        // set view and projection matrix
        shader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix()))); // remove the translation part of view matrix
        shader.SetMat4("projection", camera.GetProjectionMatrix());
        shader.SetInt("skybox", 0);
        
        cubemap.Bind(0);
        Cube::GetInstance().Draw();
        glDepthFunc(GL_LESS);
        glFrontFace(GL_CCW);
    }
};