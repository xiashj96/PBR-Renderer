#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <assimp/material.h>
#include "Shader.h"

unsigned int LoadTexture(const char* path, bool srgb = false);
unsigned int LoadTextureHDR(const char* path);
unsigned int LoadCubemap(std::vector<std::string> faces);

enum class TextureType {
	Albedo,
	Normal,
	ORM,
	Emissive,
	HDR,
	BRDF,
	Unknown
};


class Texture
{
public:
	static Texture& GetBrdfLUT();
	std::string GetFilename() const { return filename; }

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

		std::cout << "load texture type " << GetTypeString() <<  " at " << path << std::endl;
	}

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

	void Bind(int textureUnit, const Shader& shader) const
	{
		shader.SetInt(GetTypeString(), textureUnit);
		Bind(textureUnit);
	}


	Texture(unsigned int id, TextureType type) : id(id), type(type) {}
	Texture& operator=(const Texture& other)
	{
		id = other.id;
		type = other.type;
		filename = other.filename;
		return *this;
	}
	
private:
	unsigned int id;
	TextureType type;
	std::string filename;

	std::string GetTypeString() const {
		return TextureTypeStrings[static_cast<int>(type)];
	}

	const std::string TextureTypeStrings[7] = {
		"albedoMap",
		"normalMap",
		"ormMap",
		"emissiveMap",
		"HDR",
		"BRDF",
		"Unknown"
	};
};