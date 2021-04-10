/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-30
 */

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"

#endif // !STB_IMAGE_IMPLEMENTATION

#include "texture.h"

#include <utility>

disc0ver::Texture::Texture(std::string textureName, const GLchar* texturePath, TextureType textureType, bool flipVertically)
{
	/*
	构造函数
	参数一：该纹理的名称
	参数二：该纹理对应图片的路径
	*/
	this->textureName = std::move(textureName);
	this->textureType = textureType;
	std::cout << "Loading texture......\n" << "Path: " << std::string(texturePath) << "\nType: " << getTypeString() << "\n";
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 缩小
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 放大
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture img
	int width, height, nrChannels;
	// 是否需要翻转图片的y轴 默认为true
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
		{
			//  c++ error stream
			std::cerr << "Texture image channel numbers error.\n\n";
			stbi_image_free(data);
			return;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Loading success!\n\n";
	}
	else {
		std::cout << "Failed to load texture\n\n";
	}
	// 纹理已经设置完成了 记得回收读取图片所消耗的内存
	stbi_image_free(data);
}

void disc0ver::Texture::use(unsigned int ID)
{
	/*
	激活参数ID所对应的纹理单元 并把该纹理对象绑定到上面
	*/
	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(GL_TEXTURE_2D, texture);
}
