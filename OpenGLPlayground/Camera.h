#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    
    // euler Angles
    float Yaw;
    float Pitch;

    // camera movement options
    const float MovementSpeed = 2.5f;
    const float MouseSensitivity = 0.1f;

    // view frustum
    float Aspect;
    float FOV;
    const float Near = 0.1f;
    const float Far = 100.0f;

    // constructor with vectors
    Camera(float aspect, float fov = 45.0f, glm::vec3 position = glm::vec3(0.0f, 1.0f, 3.0f), float yaw = -90.f, float pitch = 0.f):
        Aspect(aspect),
        FOV(fov),
        Position(position),
        Yaw(yaw),
        Pitch(pitch)
    {
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(FOV), Aspect, Near, Far);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        FOV -= (float)yoffset;
        if (FOV < 1.0f)
            FOV = 1.0f;
        if (FOV > 60.0f)
            FOV = 60.0f;
    }

    void ProcessWindowResize(int width, int height)
    {
        Aspect = (float)width / (float)height;
    }

    glm::vec3 GetPosition() const { return Position; }

private:
    // camera vectors
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
