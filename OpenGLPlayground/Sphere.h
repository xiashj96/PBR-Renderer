#pragma once

class Sphere
{
public:
    // lazy constructed, guaranteed to be destroyed singleton
    static Sphere& GetInstance()
    {
        static Sphere instance;
        return instance;
    }

    Sphere(Sphere const&) = delete;
    void operator=(const Sphere&) = delete;

    Sphere();
    ~Sphere();
    void Draw() const;
private:
    unsigned int VBO, VAO, EBO;
    unsigned int indexCount;
};

