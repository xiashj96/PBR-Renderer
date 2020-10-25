# PBR Render

This is a demo of [physically based rendering](https://en.wikipedia.org/wiki/Physically_based_rendering) written in C++ and OpenGL. A lot of the source codes are adapted from the popular OpenGL tutorial website [learnopengl.com](https://learnopengl.com/). A graphics card that supports OpenGL 3.3+ is required to run this demo. Below are several screenshots of the program.

![](Screenshot_1.png)

![](Screenshot_2.png)

![](Screenshot_3.png)

## Libraries and resources used

[GLFW](https://www.glfw.org/): Context creation and input handling library

[Glad](https://glad.dav1d.de/): OpenGL function loader

[GLM](https://glm.g-truc.net/): OpenGL Mathematics library

[Assimp](https://assimp.org/): Open asset import library

[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h): single header image loader

amazing PBR model and textures from http://artisaverb.info/PBT.html, converted to glTF format using [Blender's glTF exporter](https://docs.blender.org/manual/en/2.80/addons/io_scene_gltf2.html)

HDR environment texture from [hdrlabs](http://www.hdrlabs.com/sibl/archive/)

## Interaction Guide

W/A/S/D and mouse:  navigate through the scene as you would in a FPS game, press down left shift to increase move speed

Mouse wheel: zoom in/out

Q/E: rotate object of interest

0-9: switch between different environment maps

space: place point light at camera (maximum number: 4)

shift+space: clear all lights

N: toggle normal visualization

L: toggle wireframe/fill

## TODO (if I have the time)

- height map support (geometry shader)
- text and GUI rendering

