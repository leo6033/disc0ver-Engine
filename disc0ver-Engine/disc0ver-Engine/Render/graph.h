/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-27
 *
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-04-13
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
#include <limits>
#include <cmath>

#include "texture.h"
#include "mesh.h"

namespace disc0ver {

	void scale(const std::vector<Mesh>& meshes, Transform& trans);

	class IBaseModel {
	public:
		virtual ~IBaseModel() = default;
		// 绘制模型
		virtual void draw(Shader& shader) = 0;
		// 添加纹理--适用于普通(简单)模型
		virtual void addTexture(std::string textureName, const GLchar* texturePath) = 0;
		void addTexture(std::string textureName, std::string texturePath)
		{
			addTexture(textureName, texturePath.c_str());
		}
		// 获取模型材质--适用于普通(简单)模型
		virtual Material* getMaterial() = 0;
		// 模型的变换
		Transform transform;
	};

	//==================================================普通模型===================================================
	// 普通模型的特点是初始化时不需要读取文件 所以任意类的普通模型初始化时的顶点数组和索引数组都是相等的
	// 而且它们只有1个网格组成
	// 所以可以认为它们都是相等的 区别仅在于 变换和纹理 也就是 transform textures material
	// 那么我们可以做一个优化 使所有模型使用同一个顶点数组和索引数组 但是有自己的transform textures 和 material
	// 这样可以降低程序的时空复杂度
	// 所以使用BaseMesh

	// 三角形模型
	class triangleModel :public IBaseModel {
	public:
		// 三角形模型
		triangleModel()
		{
			if (meshes.empty())
			{
				std::vector<Vertex> vertices = {
					//     ---- 位置 ----       ---- 法向 ----     - 纹理坐标 -
					{ -0.5f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f},	// 左下
					{  0.5f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f},   // 右下
					{  0.0f,   0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f}	// 中上
				};
				std::vector<unsigned int> indices = {
					0, 1, 2
				};
				meshes.emplace_back(std::move(vertices), std::move(indices));
			}
		}
		~triangleModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 矩形模型
	class rectangleModel : public IBaseModel {
	public:
		// 矩形模型
		rectangleModel()
		{
			if (meshes.empty())
			{
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
				meshes.emplace_back(std::move(vertices), std::move(indices));
			}
		}
		~rectangleModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 圆形模型
	class circleModel :public IBaseModel {
	public:
		// 圆形模型
		circleModel()
		{
			if (meshes.empty())
			{
				std::vector<Vertex> vertices;
				std::vector<unsigned int>indices;
				// 用120个三角形构成圆形
				int num = 120;
				float r = 0.5f, pi = 3.1415926f;
				float x, y, radians;
				// 圆心
				vertices.emplace_back(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f);
				indices.emplace_back(0);
				for (int i = 0; i <= num; i++)
				{
					radians = (i * 1.0) / num * 2 * pi;
					x = r * cos(radians);
					y = r * sin(radians);
					vertices.emplace_back(x, y, 0.0f, 0.0f, 0.0f, 1.0f, x + 0.5f, y + 0.5f);
					indices.emplace_back(i + 1);
				}
				meshes.emplace_back(std::move(vertices), std::move(indices));
			}
		}
		~circleModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 空心圆模型
	class hollowCircleModel :public IBaseModel {
	public:
		// 空心圆模型
		hollowCircleModel(float outerRadius = 0.8f, float innerRadius = 0.65f) :r1(outerRadius), r2(innerRadius)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int>indices;
			// 用120个三角形构成圆形
			int num = 120;
			float pi = 3.1415926f;
			float x, z, u, radians;
			for (int i = 0; i <= num; i++)
			{
				u = (i * 1.0) / num;
				radians = (i * 1.0) / num * 2 * pi;
				x = r1 * cos(radians);
				z = -r1 * sin(radians);
				// 我们认为贴图是矩形的 而不是环形的
				vertices.emplace_back(x, 0.0f, z, 0.0f, 1.0f, 0.0f, u, 0.0f);
				indices.emplace_back(indices.size());
				x = r2 * cos(radians);
				z = -r2 * sin(radians);
				vertices.emplace_back(x, 0.0f, z, 0.0f, 1.0f, 0.0f, u, 1.0f);
				indices.emplace_back(indices.size());
			}
			meshes.emplace_back(std::move(vertices), std::move(indices));
		}
		~hollowCircleModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		// r1>r2
		float r1, r2;
		// 空心圆支持自定义内外半径 所以不使用static
		std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 立方体模型
	class cubeModel : public IBaseModel {
	public:
		// 立方体模型
		cubeModel()
		{
			if (meshes.empty())
			{
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
				indices.resize(vertices.size());
				for (int i = 0; i < indices.size(); i++)
					indices[i] = i;
				meshes.emplace_back(std::move(vertices), std::move(indices));
			}
		}
		~cubeModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }

	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 圆柱模型 感觉目前的实现不是很优雅- -
	class cylinderModel :public IBaseModel
	{
	public:
		// 圆柱模型
		cylinderModel()
		{
			if (meshes.empty())
			{
				std::vector<Vertex> upCircleVertices, downCircleVertices, midRectVertices;
				std::vector<unsigned int> upCircleIndices, downCircleIndices, midRectIndices;
				int num = 120;
				float r = 0.5f, pi = 3.1415926f;
				float x, y, z, u, radians;
				// 圆心
				upCircleVertices.emplace_back(0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				upCircleIndices.emplace_back(0);
				downCircleVertices.emplace_back(0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
				downCircleIndices.emplace_back(0);
				for (int i = 0; i <= num; i++)
				{
					u = (i * 1.0) / num;
					radians = (i * 1.0) / num * 2 * pi;
					x = r * cos(radians);
					z = -r * sin(radians);
					upCircleVertices.emplace_back(x, 0.5f, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
					upCircleIndices.emplace_back(i + 1);
					downCircleVertices.emplace_back(x, -0.5f, z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
					downCircleIndices.emplace_back(i + 1);
					midRectVertices.emplace_back(x, 0.5f, z, x, 0.0f, z, u, 1.0f);
					midRectIndices.emplace_back(midRectIndices.size());
					midRectVertices.emplace_back(x, -0.5f, z, x, 0.0f, z, u, 0.0f);
					midRectIndices.emplace_back(midRectIndices.size());
				}
				meshes.emplace_back(std::move(upCircleVertices), std::move(upCircleIndices));
				meshes.emplace_back(std::move(downCircleVertices), std::move(downCircleIndices));
				meshes.emplace_back(std::move(midRectVertices), std::move(midRectIndices));
			}
		}
		~cylinderModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 球体模型
	class sphereModel :public IBaseModel
	{
	public:
		// 球体模型
		sphereModel()
		{
			if (meshes.empty())
			{
				std::vector<Vertex> vertices;
				std::vector<unsigned int> indices;
				// 用50*50的网格模拟球体
				int xNum = 50, yNum = 50;
				float r = 0.5f, pi = 3.1415926f;
				float x, y, z, u, v, phi, theta;
				for (int i = 0; i <= yNum; i++)
				{
					// [0,pi] 即0到180° 对应竖直方向的角度变化——从y轴正半轴开始顺时针增加
					phi = (i * 1.0) / yNum * pi;
					y = r * cos(phi);
					v = 1 - (i * 1.0) / yNum;
					// 计算在该2D平面(圆形)——圆的半径
					float tmpr = r * sin(phi);
					for (int j = 0; j <= xNum; j++)
					{
						// [0,2*pi] 即0到360° 对应2D平面即圆面的角度变化——从x轴正半轴开始顺时针增加
						theta = (j * 1.0) / xNum * 2 * pi;
						x = tmpr * cos(theta);
						z = tmpr * sin(theta);
						u = 1 - (j * 1.0) / xNum;
						vertices.emplace_back(x, y, z, x, y, z, u, v);
						if (i != yNum && j != xNum)
						{
							// 计算网格的四个顶点的索引
							// 右上
							int rightUp = i * (xNum + 1) + j;
							// 左上
							int leftUp = rightUp + 1;
							// 右下
							int rightDown = rightUp + xNum + 1;
							// 左下
							int leftDown = rightDown + 1;
							// 将该网格拆分为2个三角形
							// 右上-右下-左下
							indices.emplace_back(rightUp);
							indices.emplace_back(rightDown);
							indices.emplace_back(leftDown);
							// 右上-左下-左上
							indices.emplace_back(rightUp);
							indices.emplace_back(leftDown);
							indices.emplace_back(leftUp);
						}
					}
				}
				meshes.emplace_back(std::move(vertices), std::move(indices));
			}
		}
		~sphereModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		static std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	// 圆环模型
	class ringModel :public IBaseModel {
	public:
		// 圆环模型
		ringModel(float outerRadius = 0.8f, float innerRadius = 0.65f) :r1(outerRadius), r2(innerRadius)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			float pi = 3.1415926f, r = (r1 - r2) / 2;
			// 用80*60的网格模拟球体
			int xNum = 80, yNum = 60;
			float x, y, z, u, v, phi, theta;
			for (int i = 0; i <= yNum; i++)
			{
				// [0,2*pi] 即0到360° 对应竖直方向的角度变化——从x轴负半轴开始顺时针增加
				phi = (i * 1.0) / yNum * 2 * pi;
				y = r * sin(phi);
				v = (i * 1.0) / yNum;
				// 计算在该2D平面(圆形)——圆的半径
				float tmpr = r * cos(phi);
				for (int j = 0; j <= xNum; j++)
				{
					// [0,2*pi] 即0到360° 对应2D平面即圆面的角度变化——从x轴正半轴开始逆时针
					theta = (j * 1.0) / xNum * 2 * pi;
					x = (r2 + r + tmpr) * cos(theta);
					z = -(r2 + r + tmpr) * sin(theta);
					u = (j * 1.0) / xNum;
					vertices.emplace_back(x, y, z, x, y, z, u, v);
					if (i != yNum && j != xNum)
					{
						// 计算网格的四个顶点的索引
						// 左下
						int leftDown = i * (xNum + 1) + j;
						// 右下
						int rightDown = leftDown + 1;
						// 左上
						int leftUp = leftDown + xNum + 1;
						// 右上
						int rightUp = leftUp + 1;
						// 将该网格拆分为2个三角形
						// 右上-右下-左下
						indices.emplace_back(rightUp);
						indices.emplace_back(rightDown);
						indices.emplace_back(leftDown);
						// 右上-左下-左上
						indices.emplace_back(rightUp);
						indices.emplace_back(leftDown);
						indices.emplace_back(leftUp);
					}
				}
			}
			meshes.emplace_back(std::move(vertices), std::move(indices));
		}
		~ringModel() {}
		void draw(Shader& shader) override;
		void addTexture(std::string textureName, const GLchar* texturePath) override;
		Material* getMaterial() override { return &material; }
	private:
		// r1>r2
		float r1, r2;
		// 圆环支持自定义内外半径 所以不使用static
		std::vector<BaseMesh> meshes;
		std::vector<Texture> textures;
		Material material;
	};

	//==================================================特殊模型(需要读取文件 比如.stl .obj)===================================================
	//需要使用Mesh

	// .stl 模型
	// 比如 github skyline model https://skyline.github.com/
	class STLModel : public IBaseModel
	{
	public:
		friend void scale(const std::vector<Mesh>& meshes, Transform& trans);
		STLModel(const char* path)
		{
			// 从指定路径读取模型文件生成vertices数组
			loadModel(path);
			indices.resize(vertices.size());
			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = i;
			}
			meshes.emplace_back(move(vertices), move(indices), std::vector<Texture>());
			// 适度缩放模型
			scale(meshes, transform);
		}
		void draw(Shader& shader) override;

		// 我建议不要使用这个函数 因为模型文件有其对应的纹理(如果有 则相关信息可以在.mtl文件中找到) 添加不匹配的纹理可能造成奇怪的结果 这也是为什么它什么都不做
		void addTexture(std::string textureName, const GLchar* texturePath) override { }
		// 这个函数同理
		Material* getMaterial() override { return nullptr; }

	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void loadModel(const std::string path);

	};

	// .obj 模型
	class Model : public IBaseModel
	{
	public:
		friend void scale(const std::vector<Mesh>& meshes, Transform& trans);
		Model(const char* path)
		{
			// 从指定路径读取obj文件 生成该模型的网格
			loadModel(path);
			scale(meshes, transform);
		}
		void draw(Shader& shader) override;

		// 我建议不要使用这个函数 因为模型文件有其对应的纹理(如果有 则相关信息可以在.mtl文件中找到) 添加不匹配的纹理可能造成奇怪的结果 这也是为什么它什么都不做
		void addTexture(std::string textureName, const GLchar* texturePath) override { }
		// 这个函数同理
		Material* getMaterial() override { return nullptr; }

	private:
		std::vector<Mesh> meshes;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void loadModel(const std::string path);
		void createMesh(const std::string& materialName, std::vector<Material>& materials);
		void loadMaterial(std::vector<Material>& materials, std::string path);

	};

}
#endif
