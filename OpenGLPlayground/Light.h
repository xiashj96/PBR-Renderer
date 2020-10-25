#pragma once
#include <glm/glm.hpp>
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"

const float LightSize = 0.02f;

class Light
{
public:
	glm::vec3 Position;
	glm::vec3 Color; // note color values can go beyond 1 to indicate higher intensity
	
	Light(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f))
		:Position(position), Color(color) {}

	void Draw(const Shader& shader, const Camera& camera)
	{
		if (Color.length() > 0.01f)
		{
			shader.Use();
			shader.SetMat4("model", this->GetModelMatrix());
			shader.SetMat4("view", camera.GetViewMatrix());
			shader.SetMat4("projection", camera.GetProjectionMatrix());
			shader.SetVec3("lightColor", Color);
			Sphere::GetInstance().Draw();
		}
	}
private:

	glm::mat4 GetModelMatrix() const
	{
		// we do not care about rotation for light
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = glm::scale(model, glm::vec3(LightSize));
		return model;
	}
};

