/*
 * @Description:
 * @Author: 妄想
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
	for(int i = 0; i < vertices.size(); i ++)
	{
		indices.push_back(i);
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
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void disc0ver::cubeModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

void disc0ver::Model::Init()
{
	std::vector<Texture> tmp;
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		tmp.push_back(it->second);
	}
	for (int i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	Mesh mesh(vertices, indices, tmp);
	meshes.push_back(mesh);
}

void disc0ver::Model::draw(Shader& shader)
{
	transform.use();
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void disc0ver::Model::addTexture(std::string textureName, const GLchar* texturePath)
{
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

void disc0ver::Model::loadModel(const std::string path)
{
	std::cout << "load model " << path << std::endl;
	std::ifstream infile;
	std::string tmp_str;
	infile.open(path);

	// 读取头 solid filename
	char line[256];
	infile.getline(line, sizeof(line));
	std::istringstream solid(line);
	solid >> tmp_str;
	assert(tmp_str == "solid");

	while(!infile.eof())
	{
		infile.getline(line, sizeof(line));
		float n1, n2, n3;
		float x, y, z;
		std::istringstream face(line);

		face >> tmp_str;
		if(tmp_str == "facet")
		{
			face >> tmp_str >> n1 >> n2 >> n3;
			// outer loop
			infile.getline(line, sizeof(line));
			for(int i = 0; i < 3;i ++)
			{
				infile.getline(line, sizeof(line));
				std::istringstream vertex(line);
				vertex >> tmp_str >> x >> y >> z;
				vertices.emplace_back(x, y, z, n1, n2, n3, 0.0f, 0.0f);
			}
			// end loop
			infile.getline(line, sizeof(line));
		}
		// end facet
		infile.getline(line, sizeof(line));
	}
}

void disc0ver::Model::scale()
{
	float max_x = vertices[0].position.x;
	float max_y = vertices[0].position.y;
	float max_z = vertices[0].position.z;

	float min_x = max_x, min_y = max_y, min_z = max_z;
	
	for(int i = 1; i < vertices.size(); i ++)
	{
		if (vertices[i].position.x > max_x) max_x = vertices[i].position.x;
		else if (vertices[i].position.x < min_x) min_x = vertices[i].position.x;

		if (vertices[i].position.y > max_y) max_y = vertices[i].position.y;
		else if (vertices[i].position.y < min_y) min_y = vertices[i].position.y;

		if (vertices[i].position.z > max_z) max_z = vertices[i].position.z;
		else if (vertices[i].position.z < min_z) min_z = vertices[i].position.z;
	}

	float  m_scale = 100;
	m_scale = m_scale < (1.0 / (max_x - min_x)) ? m_scale : (1.0 / (max_x - min_x));//比较长缩放倍数 ，记录小的倍数值
	m_scale = m_scale < (1.0 / (max_y - min_y)) ? m_scale : (1.0 / (max_y - min_y));//比较宽缩放倍数 ，记录小的倍数值
	m_scale = m_scale < (1.0 / (max_z - min_z)) ? m_scale : (1.0 / (max_z - min_z));//比较高缩放倍数 ，记录小的倍数值

	for(int i = 0; i < vertices.size();i ++)
	{
		vertices[i].position *= m_scale;
	}
}
