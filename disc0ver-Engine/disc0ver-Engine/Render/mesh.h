/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2021-2-20
 *
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-04-13
 */

#ifndef MESH_H
#define MESH_H

#include <utility>
#include<vector>

#include "texture.h"
#include "../math/DGM.h"
#include "transform.h"
#include "shader.h"

namespace disc0ver
{
	// 顶点
	struct Vertex {
		Position position;	// 顶点位置
		Position normal;	// 顶点法向
		glm::vec2 texCoords;	// 纹理

		Vertex() = default;
		Vertex(float x, float y, float z, float n1, float n2, float n3, float u, float v) : position(x, y, z), normal(n1, n2, n3), texCoords(u, v) {}
		Vertex(Position v, Position vn, glm::vec2 vt) : position(v), normal(vn), texCoords(vt) {}
	};

	class BaseMesh
	{
		/* 基础网格 只包含顶点数据和索引数据 不包含材质、纹理等 */
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		BaseMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :vertices(std::move(vertices)), indices(std::move(indices))
		{
			setupMesh();
		}

		BaseMesh(const BaseMesh& mesh) :vertices(mesh.vertices), indices(mesh.indices)
		{
			setupMesh();
		}

		BaseMesh& operator=(const BaseMesh& mesh)
		{
			vertices = mesh.vertices;
			indices = mesh.indices;
			setupMesh();
		}

		BaseMesh(BaseMesh&& mesh) noexcept :vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), VAO(mesh.VAO), VBO(mesh.VBO), EBO(mesh.EBO)
		{
			mesh.VAO = mesh.VBO = mesh.EBO = 0;
		}

		BaseMesh& operator=(BaseMesh&& mesh)noexcept
		{
			vertices = std::move(mesh.vertices);
			indices = std::move(mesh.indices);
			VAO = mesh.VAO;
			VBO = mesh.VBO;
			EBO = mesh.EBO;
			mesh.VAO = mesh.VBO = mesh.EBO = 0;
		}

		~BaseMesh()
		{
			glDeleteBuffers(1, &EBO);
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		}

		/*
			使用指定的GL_DRAW_MODE 和 指定的纹理数组绘制网格
			比如 GL_TRIANGLES GL_TRIANGLE_FAN等

			这是针对graph.cpp中普通模型(除去stlModel 和 Model)所做的优化
		*/
		void Draw(Shader& shader, std::vector<Texture>& textures, Material& material, unsigned int GL_DRAW_MODE = GL_TRIANGLES);

	protected:
		/*  顶点数组对象 顶点缓冲对象 索引缓冲对象  */
		unsigned int VAO, VBO, EBO;

		// 初始化VAO VBO EBO对象
		void setupMesh();
	};

	class Mesh :public BaseMesh
	{
	public:
		/* 纹理数据 */
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
			BaseMesh(std::move(vertices), std::move(indices)), textures(std::move(textures)) { }

		Mesh(const Mesh& mesh) :BaseMesh(mesh.vertices, mesh.indices), textures(mesh.textures), material(mesh.material) { }

		Mesh& operator=(const Mesh& mesh)
		{
			vertices = mesh.vertices;
			indices = mesh.indices;
			textures = mesh.textures;
			material = mesh.material;
			setupMesh();
		}

		Mesh(Mesh&& mesh)noexcept :BaseMesh(std::move(mesh)), textures(std::move(mesh.textures)), material(std::move(mesh.material)) {}

		Mesh& operator=(Mesh&& mesh)noexcept
		{
			vertices = std::move(mesh.vertices);
			indices = std::move(mesh.indices);
			textures = std::move(mesh.textures);
			material = std::move(mesh.material);
			VAO = mesh.VAO;
			VBO = mesh.VBO;
			EBO = mesh.EBO;
			mesh.VAO = mesh.VBO = mesh.EBO = 0;
		}

		/*
			使用指定的GL_DRAW_MODE 绘制网格
			比如 GL_TRIANGLES GL_TRIANGLE_FAN等
		*/
		void Draw(Shader& shader, unsigned int GL_DRAW_MODE = GL_TRIANGLES);

		// 添加材质
		void addMaterial(Material material);

		// 获取材质
		Material* getMaterial() { return &material; }

	private:
		/* 材质 */
		Material material;
	};
}

#endif
