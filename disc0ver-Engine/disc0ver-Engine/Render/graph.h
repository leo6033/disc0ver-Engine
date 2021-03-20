/*
 * @Description:
 * @Author: ����
 * @Email: long452a@163.com
 * @Date: 2020-09-27
 */

#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include "texture.h"
#include "mesh.h"

namespace disc0ver {

	class IBaseModel {
	public:
		virtual ~IBaseModel() = default;
		virtual void Init() = 0; // ����ģ��
		// virtual void resize() = 0; // 
		virtual void draw(Shader& shader) = 0; // ����ͼ��
		virtual void addTexture(std::string textureName, const GLchar* texturePath) = 0;
		Transform transform;
	};

	class rectangleModel : public IBaseModel {
	public:
		~rectangleModel();
		void Init() override;
		//void resize() override;
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
		//Transform transform;
	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices = {
			//     ---- λ�� ----       ---- ���� ----     - �������� -
			{ 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f},   // ����
			{ 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f},   // ����
			{-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f},   // ����
			{-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f}    // ����
		};
		std::vector<unsigned int> indices = {
			0, 1, 3,
			1, 2, 3
		};
	};

	class cubeModel : public IBaseModel {
	public:
		~cubeModel();
		void Init() override;
		// void resize() override;
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices = {
			//     ---- λ�� ----       ---- ���� ----     - �������� -
			{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
			{ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f },
			{ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f },
			{ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f },
			{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f },

			{ -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f },
			{ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f },
			{ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f },
			{ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f },
			{ -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f },
			{ -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f },

			{ -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f },
			{ -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f },
			{ -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f },
			{ -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f },

			{ 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f },
			{ 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f },
			{ 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f },
			{ 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f },
			{ 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f },
			{ 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f },

			{ -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f },
			{ 0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f },
			{ 0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f },
			{ 0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f },
			{ -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f },
			{ -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f },

			{ -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f },
			{ 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f },
			{ 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f },
			{ 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f },
			{ -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f },
			{ -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f }

		};
		std::vector<unsigned int> indices;
	};

	class Model: public IBaseModel
	{
	public:
		Model(const char* path)
		{
			loadModel(path);
			scale();
		}

		void Init() override;
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
		//Transform transform;
	private:
		std::vector<Mesh> meshes;
		//std::string directory;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void loadModel(const std::string path);
		void scale();
	};
}
#endif