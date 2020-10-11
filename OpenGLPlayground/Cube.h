#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Cube
{
public:
    // lazy constructed, guaranteed to be destroyed singleton
    static Cube& GetInstance()
    {
        static Cube instance; 
        return instance;
    }

    Cube(Cube const&) = delete;
    void operator=(const Cube&) = delete;

    Cube();
    ~Cube();
    void Draw() const;
private:
    unsigned int VBO, VAO;
};

