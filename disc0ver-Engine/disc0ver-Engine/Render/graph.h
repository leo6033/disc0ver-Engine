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

#include "texture.h"

namespace disc0ver {
	// 顶点
	struct vertex {
		float x, y, z; // 坐标
		float r, g, b; // 颜色
		float u, v;  // 贴图坐标
	};

	class IBaseModel {
	public:
		unsigned int VAO, VBO, EBO;
		virtual void Init() = 0; // 创建模型
		virtual void resize() = 0; // 
		virtual void draw() = 0; // 绘制图形
		virtual void addTexture(const GLchar* texturePath) = 0;
	private:
		const std::vector<vertex> vertices;
		const std::vector<unsigned int> indices;
	};

	class rectangleModel : public IBaseModel {
	public:
		~rectangleModel();
		virtual void Init();
		virtual void resize();
		virtual void draw();
		virtual void addTexture(const GLchar* texturePath);
		std::vector<Texture> textures;
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
}
#endif