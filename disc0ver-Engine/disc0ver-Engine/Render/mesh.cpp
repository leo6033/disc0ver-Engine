#include "mesh.h"

#include <utility>
/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2021-2-20
 */

disc0ver::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = std::move(vertices);
	this->indices = std::move(indices);
	this->textures = std::move(textures);

	setupMesh();
}

void disc0ver::Mesh::Draw(Shader &shader)
{
    /* 绘制网格 */
    bool useTextureDiffuse = false;
    bool useTextureSpecular = false;
	for(unsigned int i = 0; i < textures.size(); i ++)
	{
        textures[i].use(i);
        if (textures[i].getType() == TextureType::DIFFUSE)
        {
            useTextureDiffuse = true;
            shader.setInt("material.texture_diffuse", static_cast<int>(i));
        }
        else if (textures[i].getType() == TextureType::SPECULAR)
        {
            useTextureSpecular = true;
            shader.setInt("material.texture_specular", static_cast<int>(i));
        }
	}
    if (useTextureDiffuse)
        shader.setBool("material.use_texture_diffuse", true);
    else
    {
        shader.setBool("material.use_texture_diffuse", false);
        shader.setVec3("material.ambient_color", material.Ka);
        shader.setVec3("material.diffuse_color", material.Kd);
    }                                                                                                                           
    if (useTextureSpecular)                                                                                                     
        shader.setBool("material.use_texture_specular", true);                                                                  
    else
    {
        shader.setBool("material.use_texture_specular", false);
        shader.setVec3("material.specular_color", material.Ks);
    }
    shader.setFloat("material.shininess", material.Ns / 1000 * 256);
	
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	
    glActiveTexture(GL_TEXTURE0);
}

void disc0ver::Mesh::setupMesh()
{
    // 初始化VAO VBO EBO对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

    glBindVertexArray(0);
}

void disc0ver::Mesh::addMaterial(Material material)
{
    /* 添加材质 */
    this->material = std::move(material);
    // 此处纹理名称可以随意填写 
	if(!this->material.map_Kd.empty())
	{
        Texture texture(this->material.map_Kd, this->material.map_Kd.c_str());
        textures.push_back(texture);
	}
    if (!this->material.map_Ks.empty())
    {
        Texture texture("material.texture_specular", this->material.map_Ks.c_str(), TextureType::SPECULAR);
        textures.push_back(texture);
    }
}
