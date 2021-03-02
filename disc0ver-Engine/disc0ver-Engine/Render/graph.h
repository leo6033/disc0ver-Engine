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

#include "texture.h"
#include "mesh.h"

namespace disc0ver {

	class IBaseModel {
	public:
		unsigned int VAO, VBO, EBO;
		virtual void Init() = 0; // 创建模型
		virtual void resize() = 0; // 
		virtual void draw() = 0; // 绘制图形
		virtual void addTexture(std::string textureName, const GLchar* texturePath) = 0;
	private:
		const std::vector<Vertex> vertices;
		const std::vector<unsigned int> indices;
	};

	class rectangleModel : public IBaseModel {
	public:
		~rectangleModel();
		void Init() override;
		void resize() override;
		void draw() override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
		Transform transform;
	private:
		std::vector<vertex> vertices = {
			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			{ 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f},   // 右上
			{ 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f},   // 右下
			{-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f},   // 左下
			{-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f}    // 左上
		};
		std::vector<unsigned int> indices = {
			0, 1, 3,
			1, 2, 3
		};
	};

	class cubeModel : IBaseModel {
	public:
		~cubeModel();
		void Init() override;
		void resize() override;
		void draw() override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		std::map<std::string, Texture> textures;
		Transform transform;
	private:
		std::vector<Vertex> vertices = {
			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{ 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},

			{-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{ 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},

			{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},

			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{ 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{ 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{ 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},

			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{ 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{ 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{ 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},

			{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
			{ 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{ 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f},
			{-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f},
			{-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f}

		};
		std::vector<unsigned int> indices = {
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23
		};
	};

	class Model: public IBaseModel
	{
	public:
		
	};
}
#endif