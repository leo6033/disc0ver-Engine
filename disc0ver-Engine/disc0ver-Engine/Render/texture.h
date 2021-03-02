/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-09-30
 */

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
}

#endif // !TEXTURE_H