/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2021-2-20
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
		Vertex(Position v, Position vn, glm::vec2 vt): position(v), normal(vn), texCoords(vt) {}
	};
	
	class Mesh
	{
    public:
        /*  网格数据: 顶点数据 索引数据(用于索引绘制) 纹理数据*/
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);
		void addMaterial(Material material);
		Material& getMaterial() { return material; }
    private:
        /*  顶点数组对象 顶点缓冲对象 索引缓冲对象  */
        unsigned int VAO, VBO, EBO;
		/* 材质 */
		Material material;
		bool useMaterial = false;

        void setupMesh();
	};
}

#endif
