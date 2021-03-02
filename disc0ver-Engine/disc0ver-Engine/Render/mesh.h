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
	};
	
	class Mesh
	{
    public:
        /*  网格数据  */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        /*  函数  */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader shader);
    private:
        /*  渲染数据  */
        unsigned int VAO, VBO, EBO;
        /*  函数  */
        void setupMesh();
	};
}

#endif
