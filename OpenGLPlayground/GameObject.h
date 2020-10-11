#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

/*
	A GameObject contains a Model and its transform information such as position, rotation and scale, and material parameters
	TODO: implement rotation with quaternion
*/

// phong-blinn shading parameters
struct Material
{
public:
	Material(glm::vec3 albedo = glm::vec3(0.5f, 0.5f, 0.5f), float metallic = 1.0f, float roughness = 0.5f, float ao = 0.05f, float shininess = 32.0f) :
		albedo(albedo), metallic(metallic), roughness(roughness), ao(ao), shininess(shininess){}
	glm::vec3  albedo;
	float metallic;
	float roughness;
	float ao;

	// for phong shading
	float shininess;
};

class GameObject
{
public:
	glm::vec3 Position;
	float Yaw; // in degrees
	glm::vec3 Scale;

	GameObject(
		const Model& model, 
		glm::vec3 position = glm::vec3(0.0f), 
		float yaw = 0.0f, 
		glm::vec3 scale = glm::vec3(1.0), 
		Material material = Material())
		:model(model), Position(position), Yaw(yaw), Scale(scale), material(material) {}

	void Draw(const Shader& shader, const Camera& camera, const Light& light, float time, bool lineMode = false) const
	{
		shader.Use();

		// set all uniforms before calling model.Draw()
		shader.SetMat4("model", this->GetModelMatrix());
		shader.SetMat4("view", camera.GetViewMatrix());
		shader.SetMat4("projection", camera.GetProjectionMatrix());
		shader.SetVec3("cameraPos", camera.getPosition());
		shader.SetVec3("light.position", light.Position);
		shader.SetVec3("light.color", light.Color);
		shader.SetFloat("light.constant", light.Constant);
		shader.SetFloat("light.linear", light.Linear);
		shader.SetFloat("light.quadratic", light.Quadratic);
		// for PBR shader
		shader.SetVec3("material.albedo", material.albedo);
		shader.SetFloat("material.metallic", material.metallic);
		shader.SetFloat("material.roughness", material.roughness);
		shader.SetFloat("material.ao", material.ao);
		//for phong-blinn shader
		shader.SetFloat("material.ambient", material.ao);
		shader.SetFloat("material.shininess", material.shininess);
		shader.SetFloat("time", time);

		glPolygonMode(GL_FRONT_AND_BACK, (lineMode) ? GL_LINE : GL_FILL);
		model.Draw(shader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

private:
	Model model;
	Material material;
	
	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		model = glm::rotate(model, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, Scale);
		return model;
	}
};

