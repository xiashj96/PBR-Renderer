#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <assimp/material.h>

unsigned int LoadTexture(const char* path, bool srgb = false);
unsigned int LoadTextureHDR(const char* path);
unsigned int LoadCubemap(std::vector<std::string> faces);

enum class TextureType {
	Generic,
	Albedo,
	Metallic,
	Roughness,
	Normal,
	AO,
	HDR,
	BRDF
};


class Texture
{
public:
	static Texture& GetBrdfLUT();
	std::string getFilename() const { return filename; }
	TextureType getType() const { return type; }

	Texture(const std::string path, TextureType type)
		:type(type)
	{
		// extract filename from path
		filename = path.substr(path.find_last_of("/\\") + 1);

		if (type == TextureType::HDR)
		{
			id = LoadTextureHDR(path.c_str());
		}
		else {
			// map from srgb to linear color space, according to texture type
			bool srgb = (type == TextureType::Albedo);
			id = LoadTexture(path.c_str(), srgb);
		}
	}

	// since Texture is a resource management class, we want to disable copy construction, but implement move semantics
	Texture(Texture const&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept :id(other.id), type(other.type), filename(other.filename) {}

	void Bind(int textureUnit) const {
		if (id)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit); // activate proper texture unit (0~31)
			glBindTexture(GL_TEXTURE_2D, id);
		}
		else {
			std::cout << "trying to bind uninitialized texture" << std::endl;
		}
	}

	Texture(unsigned int id = 0, TextureType type = TextureType::Generic) : id(id), type(type) {}
	~Texture() { if (id)	glDeleteTextures(1, &id);}
	 
private:
	unsigned int id;
	TextureType type;
	std::string filename;
};