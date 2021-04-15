/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-30
 *
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-04-13
 */

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"

#endif // !STB_IMAGE_IMPLEMENTATION

#include "texture.h"

#include <utility>

std::unordered_map<std::string, disc0ver::Texture> disc0ver::Texture::textureHashTable;

disc0ver::Texture::Texture(std::string textureName, const GLchar* texturePath, TextureType textureType, bool flipVertically)
{
	/*
	构造函数
	参数一：该纹理的名称
	参数二：该纹理对应图片的路径
	参数三：该纹理对应的类型
	参数四：是否翻转图片y轴
	*/
	std::cout << "Loading texture......\n" << "Path: " << texturePath << "\nType: " << getTypeString(textureType) << "\n";

	std::string path(texturePath);
	std::unordered_map<std::string, Texture>::iterator it = textureHashTable.find(path);
	// 第一次加载该纹理
	if (it == textureHashTable.end())
	{
		this->textureName = std::move(textureName);
		this->textureType = textureType;
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
				glDeleteTextures(1, &texture);
				stbi_image_free(data);
				return;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "Loading success!\n\n";
		}
		else {
			std::cout << "Failed to load texture...\n\n";
			glDeleteTextures(1, &texture);
			return;
		}
		// 纹理已经设置完成了 记得回收读取图片所消耗的内存
		stbi_image_free(data);
		textureHashTable[path] = *this;
	}
	else
	{
		(*this) = it->second;
		std::cout << "Loading success!\n\n";
	}
}

void disc0ver::Texture::use(unsigned int ID)
{
	/*
	激活参数ID所对应的纹理单元 并把该纹理对象绑定到上面
	*/
	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void disc0ver::Material::setMaterial(DefaultMaterialType materialType)
{
	/*
	将材质修改为指定材质
	仅修改Ka Kd Ks Ns
	尽量不要在循环中使用这个函数 它可能会很慢
	*/
	switch (materialType)
	{
	case DefaultMaterialType::emerald:
		Ka = Rgb(0.0215, 0.1745, 0.0215);
		Kd = Rgb(0.07568, 0.61424, 0.07568);
		Ks = Rgb(0.633, 0.727811, 0.633);
		Ns = 0.6 * 128;
		break;
	case DefaultMaterialType::jade:
		Ka = Rgb(0.135, 0.2225, 0.1575);
		Kd = Rgb(0.54, 0.89, 0.63);
		Ks = Rgb(0.316228, 0.316228, 0.316228);
		Ns = 0.1 * 128;
		break;
	case DefaultMaterialType::obsidian:
		Ka = Rgb(0.05375, 0.05, 0.06625);
		Kd = Rgb(0.18275, 0.17, 0.22525);
		Ks = Rgb(0.332741, 0.328634, 0.346435);
		Ns = 0.3 * 128;
		break;
	case DefaultMaterialType::pearl:
		Ka = Rgb(0.25, 0.20725, 0.20725);
		Kd = Rgb(1, 0.829, 0.829);
		Ks = Rgb(0.296648, 0.296648, 0.296648);
		Ns = 0.088 * 128;
		break;
	case DefaultMaterialType::ruby:
		Ka = Rgb(0.1745, 0.01175, 0.01175);
		Kd = Rgb(0.61424, 0.04136, 0.04136);
		Ks = Rgb(0.727811, 0.626959, 0.626959);
		Ns = 0.6 * 128;
		break;
	case DefaultMaterialType::turquoise:
		Ka = Rgb(0.1, 0.18725, 0.1745);
		Kd = Rgb(0.396, 0.74151, 0.69102);
		Ks = Rgb(0.297254, 0.30829, 0.306678);
		Ns = 0.1 * 128;
		break;
	case DefaultMaterialType::brass:
		Ka = Rgb(0.329412, 0.223529, 0.027451);
		Kd = Rgb(0.780392, 0.568627, 0.113725);
		Ks = Rgb(0.992157, 0.941176, 0.807843);
		Ns = 0.217949 * 128;
		break;
	case DefaultMaterialType::bronze:
		Ka = Rgb(0.2125f, 0.1275f, 0.054f);
		Kd = Rgb(0.714f, 0.4284f, 0.18144f);
		Ks = Rgb(0.393548f, 0.271906f, 0.166721f);
		Ns = 0.2 * 128;
		break;
	case DefaultMaterialType::chrome:
		Ka = Rgb(0.25, 0.25, 0.25);
		Kd = Rgb(0.4, 0.4, 0.4);
		Ks = Rgb(0.774597, 0.774597, 0.774597);
		Ns = 0.6 * 128;
		break;
	case DefaultMaterialType::copper:
		Ka = Rgb(0.19125, 0.0735, 0.0225);
		Kd = Rgb(0.7038, 0.27048, 0.0828);
		Ks = Rgb(0.256777, 0.137622, 0.086014);
		Ns = 0.1 * 128;
		break;
	case DefaultMaterialType::gold:
		Ka = Rgb(0.24725, 0.1995, 0.0745);
		Kd = Rgb(0.75164, 0.60648, 0.22648);
		Ks = Rgb(0.628281, 0.555802, 0.366065);
		Ns = 0.4 * 128;
		break;
	case DefaultMaterialType::silver:
		Ka = Rgb(0.19225, 0.19225, 0.19225);
		Kd = Rgb(0.50754, 0.50754, 0.50754);
		Ks = Rgb(0.508273, 0.508273, 0.508273);
		Ns = 0.4 * 128;
		break;
	case DefaultMaterialType::black_plastic:
		Ka = Rgb(0, 0, 0);
		Kd = Rgb(0.01, 0.01, 0.01);
		Ks = Rgb(0.5, 0.5, 0.5);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::cyan_plastic:
		Ka = Rgb(0, 0.1, 0.06);
		Kd = Rgb(0, 0.509804, 0.509804);
		Ks = Rgb(0.501961, 0.501961, 0.501961);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::green_plastic:
		Ka = Rgb(0, 0, 0);
		Kd = Rgb(0.1, 0.35, 0.1);
		Ks = Rgb(0.45, 0.55, 0.45);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::red_plastic:
		Ka = Rgb(0, 0, 0);
		Kd = Rgb(0.5, 0, 0);
		Ks = Rgb(0.7, 0.6, 0.6);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::white_plastic:
		Ka = Rgb(0, 0, 0);
		Kd = Rgb(0.55, 0.55, 0.55);
		Ks = Rgb(0.7, 0.7, 0.7);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::yellow_plastic:
		Ka = Rgb(0, 0, 0);
		Kd = Rgb(0.5, 0.5, 0);
		Ks = Rgb(0.6, 0.6, 0.5);
		Ns = 0.25 * 128;
		break;
	case DefaultMaterialType::black_rubber:
		Ka = Rgb(0.02, 0.02, 0.02);
		Kd = Rgb(0.01, 0.01, 0.01);
		Ks = Rgb(0.4, 0.4, 0.4);
		Ns = 0.078125 * 128;
		break;
	case DefaultMaterialType::cyan_rubber:
		Ka = Rgb(0, 0.05, 0.05);
		Kd = Rgb(0.4, 0.5, 0.5);
		Ks = Rgb(0.04, 0.7, 0.7);
		Ns = 0.078125 * 128;
		break;
	case DefaultMaterialType::green_rubber:
		Ka = Rgb(0, 0.05, 0);
		Kd = Rgb(0.4, 0.5, 0.4);
		Ks = Rgb(0.04, 0.7, 0.04);
		Ns = 0.078125 * 128;
		break;
	case DefaultMaterialType::red_rubber:
		Ka = Rgb(0.05, 0, 0);
		Kd = Rgb(0.5, 0.4, 0.4);
		Ks = Rgb(0.7, 0.04, 0.04);
		Ns = 0.078125 * 128;
		break;
	case DefaultMaterialType::white_rubber:
		Ka = Rgb(0.05, 0.05, 0.05);
		Kd = Rgb(0.5, 0.5, 0.5);
		Ks = Rgb(0.7, 0.7, 0.7);
		Ns = 0.078125 * 128;
		break;
	case DefaultMaterialType::yellow_rubber:
		Ka = Rgb(0.05, 0.05, 0);
		Kd = Rgb(0.5, 0.5, 0.4);
		Ks = Rgb(0.7, 0.7, 0.04);
		Ns = 0.078125 * 128;
		break;
	}
}
