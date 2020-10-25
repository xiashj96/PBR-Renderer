#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include "Texture.h"

unsigned int LoadCubemap(std::vector<std::string> faces);
unsigned int EquirectanglarToCubemap(const Texture& equirectangular);
unsigned int PrefilterSpecular(unsigned int envCubemap);

class Cubemap
{
public:
	Cubemap(std::vector<std::string> faces)
	{
		id = LoadCubemap(faces);
	}

	Cubemap(const Texture& equirectangular)
	{
		id = EquirectanglarToCubemap(equirectangular);
	}

	Cubemap(unsigned int id = 0) :id(id) {}

	void Bind(int textureUnit) const {
		if (id)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		}
		else {
			std::cout << "trying to bind uninitialized cubemap" << std::endl;
		}
	}

	Cubemap GetPrefiltered() const {
		return Cubemap(PrefilterSpecular(id));
	}
private:
	unsigned int id;
};