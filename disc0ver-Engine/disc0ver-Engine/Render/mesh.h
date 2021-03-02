/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2021-2-20
 */

#ifndef MESH_H
#define MESH_H

#include<vector>

#include "texture.h"
#include "../math/DGM.h"
#include "transform.h"
#include "shader.h"

namespace disc0ver
{
	// 顶点
	struct Vertex {
		Position position;	// 坐标
		vec3<TRANSFORM> normal;	// 顶点法向
		glm::vec2 texCoords;

		Vertex(float x, float y, float z, float n1, float n2, float n3, float u, float v) : position(x, y, z), normal(n1, n2, n3), texCoords(u, v) {}
	};
	
	class Mesh
	{
    public:
        /*  网格数据  */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);
    private:
        /*  渲染数据  */
        unsigned int VAO, VBO, EBO;

        void setupMesh();
	};
}

#endif
