/*
 * @Description: ��ɫ����
 * @Author: ����
 * @Email: long452a@163.com
 * @Date: 2020-09-24
 */
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

namespace disc0ver {
	class Shader {
	public:
		unsigned int ID;
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		void use();
		template<typename T>
		void setValue(const std::string& name, T value) const;
	private:
		void read(const GLchar* path, std::ifstream& shaderFile, std::string& code) const;
		void errorCheck(unsigned int shader, std::string type) const;
	};
}

#endif