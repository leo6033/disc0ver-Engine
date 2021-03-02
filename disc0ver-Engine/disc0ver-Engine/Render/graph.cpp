/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-09-27
 */

#include "graph.h"

disc0ver::rectangleModel::~rectangleModel()
{
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
}

void disc0ver::rectangleModel::Init() {
	std::vector<Texture> tmp;
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		tmp.push_back(it->second);
	}
	Mesh mesh(vertices, indices, tmp);
	meshes.push_back(mesh);
}

//void disc0ver::rectangleModel::resize()
//{
//}

void disc0ver::rectangleModel::draw(Shader &shader)
{
	transform.use();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void disc0ver::rectangleModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

disc0ver::cubeModel::~cubeModel()
{
}

void disc0ver::cubeModel::Init()
{
	std::vector<Texture> tmp;
	for(auto it = textures.begin();it !=textures.end();++it)
	{
		tmp.push_back(it->second);
	}
	Mesh mesh(vertices, indices, tmp);
	meshes.push_back(mesh);
}

//void disc0ver::cubeModel::resize()
//{
//}

void disc0ver::cubeModel::draw(Shader &shader)
{
	transform.use();
	for(int i = 0; i < meshes.size(); i ++)
	{
		meshes[i].Draw(shader);
	}
}

void disc0ver::cubeModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}