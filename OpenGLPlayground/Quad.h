#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Quad
{
public:
    // lazy constructed, guaranteed to be destroyed singleton
    static Quad& GetInstance()
    {
        static Quad instance;
        return instance;
    }

    Quad(Quad const&) = delete;
    void operator=(const Quad&) = delete;

    Quad();
    ~Quad();
    void Draw() const;
private:
    unsigned int VBO, VAO;
};

