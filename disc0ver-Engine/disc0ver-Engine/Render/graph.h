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

	void scale(std::vector<Vertex>& vertices);

	class IBaseModel {
	public:
		virtual ~IBaseModel() = default;
		virtual void draw(Shader& shader) = 0; // 绘制图形
		virtual void addTexture(std::string textureName, const GLchar* texturePath) = 0;
		Transform transform;
	};

	// 矩形模型
	class rectangleModel : public IBaseModel {
	public:
		rectangleModel() { meshes.emplace_back(move(vertices), move(indices), std::vector<Texture>()); }
		~rectangleModel() {};
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material& getMaterial() { return meshes[0].getMaterial(); }
		std::map<std::string, Texture> textures;
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

	// 立方体模型
	class cubeModel : public IBaseModel {
	public:
		cubeModel() 
		{ 
			indices.resize(vertices.size());
			for (int i = 0; i < indices.size(); i++)
				indices[i] = i;
			meshes.emplace_back(move(vertices), move(indices), std::vector<Texture>());
		}
		~cubeModel() {};
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material& getMaterial() { return meshes[0].getMaterial(); }
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

	//点光源模型
	//todo:目前是立方体形状 考虑之后改为球形
	using pointLightModel = cubeModel;

	// github skyline model
	// https://skyline.github.com/
	class STLModel: public IBaseModel
	{
	public:
		friend void scale(std::vector<Vertex>& vertices);
		STLModel(const char* path)
		{
			// 从指定路径读取模型文件生成vertices数组
			loadModel(path);
			// 适度缩放模型
			scale(vertices);
			indices.resize(vertices.size());
			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = i;
			}
			meshes.emplace_back(move(vertices), move(indices), std::vector<Texture>());
		}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void loadModel(const std::string path);
	};

	// Marry.obj 模型
	class Model: public IBaseModel
	{
	public:
		friend void scale(std::vector<Vertex>& vertices);
		Model(const char* path)
		{
			loadModel(path);
			scale(vertices);
		}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void loadModel(const std::string path);
		void createMesh(std::string materialName, std::vector<Material>& materials);
		void loadMaterial(std::vector<Material> &materials, std::string path);
		
	};

}
#endif