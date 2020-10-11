#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// TODO: use quaternion to get rotation matrix
class Transform
{
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float Yaw = -90.0f; // in degrees
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = glm::rotate(model, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, Scale);
		return model;
	}
};

