#include <iostream>
#include <glad/glad.h> // be sure to include glad before any library that requires openGL
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Light.h"
#include "Transform.h"
#include "Skybox.h"
#include "Texture.h"
#include "IBL.h"
#include "Model.h"

// timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse input
float lastX;
float lastY;
bool firstMouse = true;

// screen dimension
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// model rotation speed
const float degreesPerSecond = 90.0f;

// render settings
bool DisplayNormal = false;
bool LineMode = false;

// skybox
int skyboxIndex = 0;

Camera cam((float)SCR_WIDTH / (float)SCR_HEIGHT);
Light lights[4] = {
    {glm::vec3(0.0f), glm::vec3(0.0f)},
    {glm::vec3(0.0f), glm::vec3(0.0f)},
    {glm::vec3(0.0f), glm::vec3(0.0f)},
    {glm::vec3(0.0f), glm::vec3(0.0f)}
}; // all lights turned off by default
unsigned int lightIndex = 0;
Transform mainObj;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cam.ProcessWindowResize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool pressed = (action != GLFW_RELEASE);
    bool shifted = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (pressed)
            glfwSetWindowShouldClose(window, true);
        break;

    case GLFW_KEY_LEFT_SHIFT:
        if (pressed)
            cam.MovementSpeed = MovementSpeedFast;
        else
            cam.MovementSpeed = MovementSpeedSlow;
        break;

    case GLFW_KEY_L:
        if (pressed)
            LineMode = !LineMode;
        break;

    case GLFW_KEY_N:
        if (pressed)
            DisplayNormal = !DisplayNormal;
        break;

    case GLFW_KEY_1:
        skyboxIndex = 0;
        break;

    case GLFW_KEY_2:
        skyboxIndex = 1;
        break;

    case GLFW_KEY_3:
        skyboxIndex = 2;
        break;

    case GLFW_KEY_4:
        skyboxIndex = 3;
        break;

    case GLFW_KEY_5:
        skyboxIndex = 4;
        break;

    case GLFW_KEY_6:
        skyboxIndex = 5;
        break;

    case GLFW_KEY_7:
        skyboxIndex = 6;
        break;

    case GLFW_KEY_8:
        skyboxIndex = 7;
        break;

    case GLFW_KEY_9:
        skyboxIndex = 8;
        break;

    case GLFW_KEY_0:
        skyboxIndex = 9;
        break;

    case GLFW_KEY_SPACE:
        if (pressed)
            if (shifted)
                for (int i = 0; i < 4; ++i)
                {
                    lights[i] = Light(glm::vec3(0.0f), glm::vec3(0.0f));
                }
            else
                lights[(lightIndex++) % 4] = Light(cam.GetPosition(), glm::vec3(1.0f));
        break;

    default:
        break;
    }
}

void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cam.ProcessKeyboard(FORWARD, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cam.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cam.ProcessKeyboard(RIGHT, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mainObj.Yaw += deltaTime * degreesPerSecond;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mainObj.Yaw -= deltaTime * degreesPerSecond;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;
    cam.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);
}

int main()
{
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PBR Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // register callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // compile and link shaders
    //Shader simpleShader("resources/shaders/simple.vs", "resources/shaders/simple.fs");
    //Shader phongShader("resources/shaders/phong.vs", "resources/shaders/phong.fs");
    Shader lightShader("resources/shaders/light.vs", "resources/shaders/light.fs");
    Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    //Shader reflectionShader("resources/shaders/reflection.vs", "resources/shaders/reflection.fs");
    //Shader refractionShader("resources/shaders/refraction.vs", "resources/shaders/refraction.fs");
    //Shader depthShader("resources/shaders/depth.vs", "resources/shaders/depth.fs");
    //Shader explodeShader("resources/shaders/explode.vs", "resources/shaders/explode.fs", "resources/shaders/explode.gs");
    Shader normalShader("resources/shaders/normal.vs", "resources/shaders/normal.fs", "resources/shaders/normal.gs");
    //Shader phongShaderNormalMapping("resources/shaders/phong_normal.vs", "resources/shaders/phong_normal.fs");
    Shader pbrShader("resources/shaders/pbr.vs", "resources/shaders/pbr.fs");

    // resource loading
    stbi_set_flip_vertically_on_load(true);

    // models
    Model myObj("resources/models/Cerberus/Cerberus.gltf");
    // environment Maps
    IBL envMap[10] = {
        {"resources/IBL/Alexs_Apartment/Alexs_Apt_8k.jpg", "resources/IBL/Alexs_Apartment/Alexs_Apt_Env.hdr", "resources/IBL/Alexs_Apartment/Alexs_Apt_2k.hdr"},
        {"resources/IBL/Desert_Highway/Road_to_MonumentValley_8k.jpg", "resources/IBL/Desert_Highway/Road_to_MonumentValley_Env.hdr", "resources/IBL/Desert_Highway/Road_to_MonumentValley_Ref.hdr"},
        {"resources/IBL/empty_room/Hires_pano.jpg", "resources/IBL/empty_room/diffuse.hdr", "resources/IBL/empty_room/medium_res.hdr"},
        {"resources/IBL/snow_machine/test8_Bg.jpg", "resources/IBL/snow_machine/test8_Env.hdr", "resources/IBL/snow_machine/test8_Ref.hdr"},
        {"resources/IBL/Subway_Lights/20_Subway_Lights_8k.jpg", "resources/IBL/Subway_Lights/20_Subway_Lights_Env.hdr", "resources/IBL/Subway_Lights/20_Subway_Lights_3k.hdr"},
        {"resources/IBL/Tokyo_BigSight/Tokyo_BigSight_8k.jpg", "resources/IBL/Tokyo_BigSight/Tokyo_BigSight_Env.hdr", "resources/IBL/Tokyo_BigSight/Tokyo_BigSight_3k.hdr"},
        {"resources/IBL/Walk_Of_Fame/Mans_Outside_8k_TMap.jpg", "resources/IBL/Walk_Of_Fame/Mans_Outside_Env.hdr", "resources/IBL/Walk_Of_Fame/Mans_Outside_2k.hdr"},
        {"resources/IBL/Ueno-Shrine/03-Ueno-Shrine_8k.jpg", "resources/IBL/Ueno-Shrine/03-Ueno-Shrine_Env.hdr", "resources/IBL/Ueno-Shrine/03-Ueno-Shrine_3k.hdr"},
        {"resources/IBL/Circus_Backstage/Circus_Backstage_8k.jpg", "resources/IBL/Circus_Backstage/Circus_Backstage_Env.hdr", "resources/IBL/Circus_Backstage/Circus_Backstage_3k.hdr"},
        {"resources/IBL/QueenMary_Chimney/QueenMary_Chimney_8k.jpg", "resources/IBL/QueenMary_Chimney/QueenMary_Chimney_Env.hdr", "resources/IBL/QueenMary_Chimney/QueenMary_Chimney_Ref.hdr"}
    };

    // enable depth testing (z-buffer)
    glEnable(GL_DEPTH_TEST);

    // enable gamma correction globally
    glEnable(GL_FRAMEBUFFER_SRGB);

    // enable back face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // by default counter-clockwise vertex order is front-facing, except for skybox

    // enable MSAA
    glEnable(GL_MULTISAMPLE);

    // initialize brdfLUT
    Texture& brdfLUT = Texture::GetBrdfLUT();

    // before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    // the render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw light
        for (int i = 0; i < 4; ++i)
        {
            lights[i].Draw(lightShader, cam);
        }

        // draw object of interest
        // 1. use shader
        Shader& shader = pbrShader;
        shader.Use();

        // 2. set uniforms
        shader.SetMat4("model", mainObj.GetModelMatrix());
        shader.SetMat4("view", cam.GetViewMatrix());
        shader.SetMat4("projection", cam.GetProjectionMatrix());
        shader.SetVec3("cameraPos", cam.GetPosition());
        for (int i = 0; i < 4; ++i)
        {
            shader.SetVec3("lights[" + std::to_string(i)+"].position", lights[i].Position);
            shader.SetVec3("lights[" + std::to_string(i) + "].color", lights[i].Color);
        }
        shader.SetFloat("time", glfwGetTime());
        shader.SetInt("irradianceMap", 0);
        shader.SetInt("prefilterMap", 1);
        shader.SetInt("brdfLUT", 2);

        // 3. bind textures
        envMap[skyboxIndex].diffuse.Bind(0);
        envMap[skyboxIndex].specular.Bind(1);
        brdfLUT.Bind(2);

        // 4. draw call
        glPolygonMode(GL_FRONT_AND_BACK, (LineMode) ? GL_LINE : GL_FILL);
        myObj.Draw(shader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (DisplayNormal)
        {
            normalShader.Use();
            normalShader.SetMat4("model", mainObj.GetModelMatrix());
            normalShader.SetMat4("view", cam.GetViewMatrix());
            normalShader.SetMat4("projection", cam.GetProjectionMatrix());
            myObj.Draw(normalShader);
        }

        // draw skybox last since it has maximum depth
        Skybox::Draw(envMap[skyboxIndex].background, skyboxShader, cam);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
	return 0;
}