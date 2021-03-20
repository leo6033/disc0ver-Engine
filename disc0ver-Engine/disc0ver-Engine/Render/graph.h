/*
 * @Description:
 * @Author: 妄想
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
		virtual void Init() = 0; // 创建模型
		// virtual void resize() = 0; // 
		virtual void draw(Shader& shader) = 0; // 绘制图形
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
			//     ---- 位置 ----       ---- 法向 ----     - 纹理坐标 -
			{ 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f},   // 右上
			{ 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f},   // 右下
			{-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f},   // 左下
			{-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f}    // 左上
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
			//     ---- 位置 ----       ---- 法向 ----     - 纹理坐标 -
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

	class STLModel: public IBaseModel
	{
	public:
		STLModel(const char* path)
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
		void createMesh(std::string materialName, std::vector<Material>& materials);
		void loadMaterial(std::vector<Material> &materials, std::string path);
		
	};

}
#endif