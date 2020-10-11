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

// timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse input
float lastX;
float lastY;
bool firstMouse = true;

// screen dimension
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// model rotation speed
const float degreesPerSecond = 90.0f;

// render settings
bool DisplayNormal = false;
bool LineMode = false;

// skybox
int skyboxIndex = 0;

Camera cam((float)SCR_WIDTH / (float)SCR_HEIGHT);
Transform mainObj;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cam.ProcessWindowResize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool pressed = (action != GLFW_RELEASE);
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (pressed)
            glfwSetWindowShouldClose(window, true);
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
    //glfwWindowHint(GLFW_SAMPLES, 4);

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
    Shader simpleShader("resources/shaders/simple.vs", "resources/shaders/simple.fs");
    Shader phongShader("resources/shaders/phong.vs", "resources/shaders/phong.fs");
    Shader lightShader("resources/shaders/light.vs", "resources/shaders/light.fs");
    Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    Shader reflectionShader("resources/shaders/reflection.vs", "resources/shaders/reflection.fs");
    Shader refractionShader("resources/shaders/refraction.vs", "resources/shaders/refraction.fs");
    Shader depthShader("resources/shaders/depth.vs", "resources/shaders/depth.fs");
    Shader explodeShader("resources/shaders/explode.vs", "resources/shaders/explode.fs", "resources/shaders/explode.gs");
    Shader normalShader("resources/shaders/normal.vs", "resources/shaders/normal.fs", "resources/shaders/normal.gs");
    Shader phongShaderNormalMapping("resources/shaders/phong_normal.vs", "resources/shaders/phong_normal.fs");
    Shader pbrShader("resources/shaders/pbr.vs", "resources/shaders/pbr.fs");

    // resource loading
    stbi_set_flip_vertically_on_load(true);
    //Model backpack_model("resources/models/backpack/backpack.obj");
    Texture albedo("resources/textures/broken_down_concrete1_ue/broken_down_concrete1_albedo.png", TextureType::Albedo);
    Texture normal("resources/textures/broken_down_concrete1_ue/broken_down_concrete1_Normal-dx.png", TextureType::Normal);
    Texture metallic("resources/textures/broken_down_concrete1_ue/broken_down_concrete1_Metallic.png", TextureType::Metallic);
    Texture roughness("resources/textures/broken_down_concrete1_ue/broken_down_concrete1_Roughness.png", TextureType::Roughness);
    Texture ao("resources/textures/broken_down_concrete1_ue/broken_down_concrete1_ao.png", TextureType::AO);
    IBL subway{
        {Texture{"resources/IBL/Subway_Lights/20_Subway_Lights_8k.jpg", TextureType::Albedo}},
        {Texture{"resources/IBL/Subway_Lights/20_Subway_Lights_Env.hdr", TextureType::HDR}},
        {Texture{"resources/IBL/Subway_Lights/20_Subway_Lights_3k.hdr", TextureType::HDR}} };


    //Cubemap envMap[4] = { {alexApartment},  {backlot}, {emptyroom}, {office} };

    // scene definition
    Light light(glm::vec3(1.2f, 1.0f, -2.0f));
    //GameObject backpack(backpack_model);

    // enable depth testing (z-buffer)
    glEnable(GL_DEPTH_TEST);

    // enable gamma correction globally
    glEnable(GL_FRAMEBUFFER_SRGB);

    // enable back face culling
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW); // by default counter-clockwise vertex order is front-facing, except for skybox

    // enable MSAA
    //glEnable(GL_MULTISAMPLE);

    // initialize brdfLUT
    Texture::GetBrdfLUT();

    //before rendering, configure the viewport to the original framebuffer's screen dimensions
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
        light.Draw(lightShader, cam);

        // draw object of interest
        // 1. use shader
        Shader& shader = pbrShader;
        shader.Use();

        // 2. set uniforms
        shader.SetMat4("model", mainObj.GetModelMatrix());
        shader.SetMat4("view", cam.GetViewMatrix());
        shader.SetMat4("projection", cam.GetProjectionMatrix());
        shader.SetVec3("cameraPos", cam.GetPosition());
        shader.SetVec3("light.position", light.Position);
        shader.SetVec3("light.color", light.Color);
        shader.SetFloat("time", glfwGetTime());
        shader.SetInt("irradianceMap", 0);
        shader.SetInt("prefilterMap", 1);
        shader.SetInt("brdfLUT", 2);
        shader.SetInt("albedoMap", 3);
        shader.SetInt("normalMap", 4);
        shader.SetInt("metallicMap", 5);
        shader.SetInt("roughnessMap", 6);
        shader.SetInt("aoMap", 7);

        // 3. bind textures
        subway.diffuse.Bind(0);
        subway.specular.Bind(1);
        Texture::GetBrdfLUT().Bind(2);
        albedo.Bind(3);
        normal.Bind(4);
        metallic.Bind(5);
        roughness.Bind(6);
        ao.Bind(7);

        // 4. draw call
        glPolygonMode(GL_FRONT_AND_BACK, (LineMode) ? GL_LINE : GL_FILL);
            Sphere::GetInstance().Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (DisplayNormal)
        {
            normalShader.Use();
            normalShader.SetMat4("model", mainObj.GetModelMatrix());
            normalShader.SetMat4("view", cam.GetViewMatrix());
            normalShader.SetMat4("projection", cam.GetProjectionMatrix());
            Sphere::GetInstance().Draw();
        }

        // draw skybox last since it has maximum depth
        Skybox::Draw(subway.background, skyboxShader, cam);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
	return 0;
}