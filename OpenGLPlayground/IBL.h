#pragma once
#include "Cubemap.h"

/*
	image-based lighting
*/



class IBL
{
public:
	IBL(Cubemap background, Cubemap diffuse, Cubemap specular) :
		background(background), diffuse(diffuse), specular(specular.GetPrefiltered()) {}
	Cubemap background;		// non-hdr image used to render skybox
	Cubemap diffuse;		// hdr irridiance map for diffuse lighting
	Cubemap specular;		// hdr image used for specular lighting, need to generate mip-map first
};