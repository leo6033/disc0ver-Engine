/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-10-16
 */

#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include "../../render/transform.h"
#include "../../render/shader.h"

namespace disc0ver {

	using std::string;

	//基础光源 有三个颜色分量:环境光 漫反射光 镜面光
	class BaseLight
	{
	public:
		Rgb ambient;
		Rgb diffuse;
		Rgb specular;

		BaseLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :ambient(ambient), diffuse(diffuse), specular(specular) {}
		BaseLight(float ambient_r = 0, float ambient_g = 0, float ambient_b = 0, float diffuse_r = 0, float diffuse_g = 0, float diffuse_b = 0,
			float specular_r = 0, float specular_g = 0, float specular_b = 0) :ambient(ambient_r, ambient_g, ambient_b),
			diffuse(diffuse_r, diffuse_g, diffuse_b), specular(specular_r, specular_g, specular_b) {}
	};

	//可衰减型光源 attenuation = 1.0/( c + l * dis + q * dis * dis) dis为片段(顶点)距光源的距离
	class AttenuationLight :public BaseLight
	{
	public:
		float constant;
		float linear;
		float quadratic;
		AttenuationLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
			float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f) :BaseLight(ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}
	};

	//方向光 不需要考虑衰减
	class DirLight :public BaseLight
	{
	public:
		glm::vec3 direction;

		DirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :BaseLight(ambient, diffuse, specular), direction(direction) {}
	};

	//点光源 需要考虑衰减
	class PointLight :public AttenuationLight
	{
	public:
		glm::vec3 position;

		PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
			float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f) :AttenuationLight(ambient, diffuse, specular, constant, linear, quadratic), position(position) {}
	};

	//聚光灯 需要考虑衰减
	class SpotLight :public AttenuationLight
	{
	public:
		glm::vec3 position;
		glm::vec3 direction;
		//聚光灯内角cos值
		float cutOff;
		//聚光灯外角cos值
		float outerCutOff;

		SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
			float constant = 1.0f, float linear = 0.7f, float quadratic = 1.8f, float cutOff = cos(glm::radians(12.5f)), float outerCutOff = cos(glm::radians(17.5f))) :
			AttenuationLight(ambient, diffuse, specular, constant, linear, quadratic), position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}
	};


	void setLightAllAttribute(const Shader& shader, const string& uniformName, const DirLight* dirLights, int len = 1);
	void setLightAllAttribute(const Shader& shader, const string& uniformName, const PointLight* pointLights, int len = 1);
	void setLightAllAttribute(const Shader& shader, const string& uniformName, const SpotLight* spotLights, int len = 1);
}

#endif // !LIGHT_H