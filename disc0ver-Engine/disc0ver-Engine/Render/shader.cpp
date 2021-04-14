/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-24
 */

#include "shader.h"

disc0ver::Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	/*
	着色器程序构造函数
	参数一：顶点着色器代码文件路径
	参数二：片段着色器代码文件路径
	*/
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		read(vertexPath, vShaderFile, vertexCode);
		read(fragmentPath, fShaderFile, fragmentCode);
	}
	catch (std::ifstream::failure e) {
		std::cout << "SHADER.CPP ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader;

	// 编译 Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	errorCheck(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	errorCheck(fragmentShader, "FRAGMENT");

	// 链接
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	errorCheck(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void disc0ver::Shader::use() {
	/* 激活程序对象 */
	glUseProgram(ID);
}

void disc0ver::Shader::read(const GLchar* path, std::ifstream& shaderFile, std::string& code) const
{
	/* 读取着色器文件 */
	shaderFile.open(path);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	code = shaderStream.str();
}

void disc0ver::Shader::errorCheck(unsigned int shader, std::string type) const {
	/* 判断是否有编译、连接错误 */
	int success;
	char infoLog[512];
	if (type == "PROGRAM") {
		glLinkProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "SHADER.CPP ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}
