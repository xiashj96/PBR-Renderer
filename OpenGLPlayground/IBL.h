#pragma once
#include "Cubemap.h"

/*
	image-based lighting asset management class
	note: diffuse maps and specular reflection maps should be generated in 3rd party applications such as cmftStudio or IBLBaker
*/


class IBL
{
public:
	IBL(std::string backgroundTexture, std::string diffuseTexture, std::string specularTexture):
		background{ Texture{ backgroundTexture, TextureType::Albedo } },
		diffuse{ Texture{ diffuseTexture, TextureType::HDR } },
		specular{ Cubemap{Texture{ specularTexture, TextureType::HDR } }.GetPrefiltered() }
	{}
	
	Cubemap background;		// non-hdr image used to render skybox
	Cubemap diffuse;		// hdr irridiance map for diffuse lighting
	Cubemap specular;		// hdr image used for specular lighting, need to generate prefiltered mip-map first
};