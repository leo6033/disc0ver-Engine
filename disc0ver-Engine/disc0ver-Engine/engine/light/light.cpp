/*
 * @Description: 光源类
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-10-16
 *
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-04-13
 */

#include "light.h"

using std::string;
using std::to_string;
using disc0ver::Shader;
using disc0ver::DirLight;
using disc0ver::PointLight;
using disc0ver::SpotLight;


void disc0ver::setLightAllAttribute(const Shader& shader, const string& uniformName, const DirLight* dirLights, int len)
{
	/*
		为glsl中的方向光uniform变量设置所有属性的值：
		direction ambient diffuse specular
	*/
	if (len == 1)
	{
		shader.setVec3(uniformName + ".direction", dirLights->direction);
		shader.setVec3(uniformName + ".ambient", dirLights->ambient);
		shader.setVec3(uniformName + ".diffuse", dirLights->diffuse);
		shader.setVec3(uniformName + ".specular", dirLights->specular);
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			shader.setVec3(uniformName + "[" + to_string(i) + "].direction", dirLights[i].direction);
			shader.setVec3(uniformName + "[" + to_string(i) + "].ambient", dirLights[i].ambient);
			shader.setVec3(uniformName + "[" + to_string(i) + "].diffuse", dirLights[i].diffuse);
			shader.setVec3(uniformName + "[" + to_string(i) + "].specular", dirLights[i].specular);
		}
	}
}

void disc0ver::setLightAllAttribute(const Shader& shader, const string& uniformName, const PointLight* pointLights, int len)
{
	/*
		为glsl中的点光源uniform变量设置所有属性的值:
		position ambient diffuse specular constant linear quadratic
	*/
	if (len == 1)
	{
		shader.setVec3(uniformName + ".position", pointLights->position);
		shader.setVec3(uniformName + ".ambient", pointLights->ambient);
		shader.setVec3(uniformName + ".diffuse", pointLights->diffuse);
		shader.setVec3(uniformName + ".specular", pointLights->specular);
		shader.setFloat(uniformName + ".constant", pointLights->constant);
		shader.setFloat(uniformName + ".linear", pointLights->linear);
		shader.setFloat(uniformName + ".quadratic", pointLights->quadratic);
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			shader.setVec3(uniformName + "[" + to_string(i) + "].position", pointLights[i].position);
			shader.setVec3(uniformName + "[" + to_string(i) + "].ambient", pointLights[i].ambient);
			shader.setVec3(uniformName + "[" + to_string(i) + "].diffuse", pointLights[i].diffuse);
			shader.setVec3(uniformName + "[" + to_string(i) + "].specular", pointLights[i].specular);
			shader.setFloat(uniformName + "[" + to_string(i) + "].constant", pointLights->constant);
			shader.setFloat(uniformName + "[" + to_string(i) + "].linear", pointLights->linear);
			shader.setFloat(uniformName + "[" + to_string(i) + "].quadratic", pointLights->quadratic);
		}
	}
}

void disc0ver::setLightAllAttribute(const Shader& shader, const string& uniformName, const SpotLight* spotLights, int len)
{
	/*
		为glsl中的聚光灯uniform变量设置所有属性的值:
		position direction ambient diffuse specular constant linear quadratic cutOff outerCutOff
	*/
	if (len == 1)
	{
		shader.setVec3(uniformName + ".position", spotLights->position);
		shader.setVec3(uniformName + ".direction", spotLights->direction);
		shader.setVec3(uniformName + ".ambient", spotLights->ambient);
		shader.setVec3(uniformName + ".diffuse", spotLights->diffuse);
		shader.setVec3(uniformName + ".specular", spotLights->specular);
		shader.setFloat(uniformName + ".constant", spotLights->constant);
		shader.setFloat(uniformName + ".linear", spotLights->linear);
		shader.setFloat(uniformName + ".quadratic", spotLights->quadratic);
		shader.setFloat(uniformName + ".cutOff", spotLights->cutOff);
		shader.setFloat(uniformName + ".outerCutOff", spotLights->outerCutOff);
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			shader.setVec3(uniformName + "[" + to_string(i) + "].position", spotLights[i].position);
			shader.setVec3(uniformName + "[" + to_string(i) + "].direction", spotLights[i].direction);
			shader.setVec3(uniformName + "[" + to_string(i) + "].ambient", spotLights[i].ambient);
			shader.setVec3(uniformName + "[" + to_string(i) + "].diffuse", spotLights[i].diffuse);
			shader.setVec3(uniformName + "[" + to_string(i) + "].specular", spotLights[i].specular);
			shader.setFloat(uniformName + "[" + to_string(i) + "].constant", spotLights->constant);
			shader.setFloat(uniformName + "[" + to_string(i) + "].linear", spotLights->linear);
			shader.setFloat(uniformName + "[" + to_string(i) + "].quadratic", spotLights->quadratic);
			shader.setFloat(uniformName + "[" + to_string(i) + "].cutOff", spotLights->cutOff);
			shader.setFloat(uniformName + "[" + to_string(i) + "].outerCutOff", spotLights->outerCutOff);
		}
	}
}
