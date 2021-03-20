/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-30
 */

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>

namespace disc0ver {
	class Texture {
	public:
		unsigned int texture;
		Texture() {}
		Texture(std::string textureName, const GLchar* texturePath);
		void use(unsigned int ID);
		std::string getName() const { return textureName; }
	private:
		std::string textureName;
	};

	class Material
	{
	public:
		Material()
		{
			name;
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;
		}

		// Material Name
		std::string name;
		// Ambient Color
		glm::vec3 Ka;
		// Diffuse Color
		glm::vec3 Kd;
		// Specular Color
		glm::vec3 Ks;
		// Specular Exponent
		float Ns;
		// Optical Density
		float Ni;
		// Dissolve
		float d;
		// Illumination
		int illum;
		// Ambient Texture Map
		std::string map_Ka;
		// Diffuse Texture Map
		std::string map_Kd;
		// Specular Texture Map
		std::string map_Ks;
		// Specular Hightlight Map
		std::string map_Ns;
		// Alpha Texture Map
		std::string map_d;
		// Bump Map
		std::string map_bump;
	};
}

#endif // !TEXTURE_H