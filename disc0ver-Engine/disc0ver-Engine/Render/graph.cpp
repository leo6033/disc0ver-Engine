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
	/* 矩形模型——初始化 */
	Mesh mesh(vertices, indices, std::vector<Texture>());
	meshes.push_back(mesh);
}

//void disc0ver::rectangleModel::resize()
//{
//}

void disc0ver::rectangleModel::draw(Shader &shader)
{
	/* 矩形模型——绘制 */
	transform.use();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void disc0ver::rectangleModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	/* 矩形模型——添加纹理 */
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

disc0ver::cubeModel::~cubeModel()
{
}

void disc0ver::cubeModel::Init()
{
	/* 立方体模型——初始化 */
	for(int i = 0; i < vertices.size(); i ++)
	{
		indices.push_back(i);
	}
	Mesh mesh(vertices, indices, std::vector<Texture>());
	meshes.push_back(mesh);
}

//void disc0ver::cubeModel::resize()
//{
//}

void disc0ver::cubeModel::draw(Shader &shader)
{
	/* 立方体模型——绘制 */
	transform.use();
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void disc0ver::cubeModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	/* 立方体模型——添加纹理 */
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

void disc0ver::STLModel::Init()
{
	/* Github-SkyLine Model 初始化 */
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

void disc0ver::STLModel::draw(Shader& shader)
{
	/* Github-SkyLine Model 绘制 */
	transform.use();
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void disc0ver::STLModel::addTexture(std::string textureName, const GLchar* texturePath)
{
	/* Github-SkyLine Model 添加纹理 */
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

void disc0ver::STLModel::loadModel(const std::string path)
{
	/* 
		Github-SkyLine Model 从指定路径加载模型文件(.stl) 
		其格式大致为:

		solid stlmesh
		facet normal floatx floaty floatz
			outer loop
				vertex floatx floaty floatz
				vertex floatx floaty floatz
				vertex floatx floaty floatz
			endloop
		end facet
		facet......
		......
		end facet
		endsolid stlmesh

	*/
	std::cout << "load model " << path << std::endl;
	std::ifstream infile;
	std::string tmp_str;
	infile.open(path);

	if(!infile.is_open())
	{
		throw "model not found";
	}

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
			// 法线坐标
			face >> tmp_str >> n1 >> n2 >> n3;
			// outer loop
			infile.getline(line, sizeof(line));
			// 三个顶点坐标
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

void disc0ver::STLModel::scale()
{
	/* 依据模型的坐标范围自动进行缩放 */
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
	m_scale = m_scale < (1.0 / (max_x - min_x)) ? m_scale : (1.0 / (max_x - min_x));
	m_scale = m_scale < (1.0 / (max_y - min_y)) ? m_scale : (1.0 / (max_y - min_y));
	m_scale = m_scale < (1.0 / (max_z - min_z)) ? m_scale : (1.0 / (max_z - min_z));

	for (auto& vertice : vertices)
	{
		vertice.position *= m_scale;
	}
	// you can also set transform.scale
	// transform.scale = { m_scale, m_scale, m_scale };
}

void disc0ver::Model::Init()
{

}

void disc0ver::Model::draw(Shader& shader)
{
	/* Marry.obj 模型绘制 */
	transform.use();
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void disc0ver::Model::addTexture(std::string textureName, const GLchar* texturePath)
{
	/* Marry.obj 添加纹理 */
	Texture texture(textureName, texturePath);
	textures[textureName] = texture;
	meshes[0].textures.push_back(texture);
}

void disc0ver::Model::loadModel(const std::string path)
{
	/* Marry.obj 加载模型文件 */
	std::cout << "load model " << path << std::endl;
	std::ifstream infile;
	std::string tmp_str;
	infile.open(path);

	if (!infile.is_open())
	{
		throw "model file not found";
	}

	char line[256];
	std::vector<vec3<TRANSFORM>> v, vn;
	std::vector<glm::vec2> vt;
	std::vector<Material> materials;
	unsigned int Index[9];
	int flag = 0;
	std::string materialName;
	
	while (!infile.eof())
	{
		infile.getline(line, sizeof(line));
		float n1, n2, n3;
		float uvx, uvy;
		float x, y, z;
		std::istringstream ss(line);

		ss >> tmp_str;
		// 顶点位置坐标
		if (tmp_str == "v")
		{
			ss >> x >> y >> z;
			v.emplace_back(x, y, z);
		}
		// 顶点法线坐标
		else if(tmp_str == "vn")
		{
			ss >> n1 >> n2 >> n3;
			vn.emplace_back(n1, n2, n3);
		}
		// 顶点纹理坐标
		else if (tmp_str == "vt")
		{
			ss >> uvx >> uvy;
			vt.emplace_back(uvx, uvy);
		}
		else if(tmp_str == "f")
		{
			// TODO: 不同形式的 f 参数实现
			std::string s;
			int index = 0;
			while(ss >> s)
			{
				s = s + "/";
				while (s.find('/') != std::string::npos) {
					int pos = s.find('/');
					std::istringstream i(s.substr(0, pos));
					i >> Index[index++];
					s = s.substr(pos + 1, s.size());
				}
			}
			for(int i = 0; i < index;i += 3)
			{
				vertices.emplace_back(v[Index[i] - 1], vn[Index[i + 2] - 1], vt[Index[i + 1] - 1]);
			}
		}
		else if(tmp_str == "mtllib")
		{
			std::string mtPath;
			ss >> mtPath;
			if(path.find('/') != std::string::npos)
				mtPath = path.substr(0, path.find_last_of('/') + 1) + mtPath;
			loadMaterial(materials, mtPath);
		}
		else if(tmp_str == "usemtl")
		{
			ss >> materialName;
		}
		else if(tmp_str == "s")
		{
			flag = 1;
		}
		else if(tmp_str == "o")
		{
			if(flag == 1)
			{
				createMesh(materialName, materials);
				materialName.clear();
				flag = 0;
			}
		}
	}
	if (flag == 1)
		createMesh(materialName, materials);
}

void disc0ver::Model::scale()
{
	/* 依据模型的坐标范围自动进行缩放 */
	float max_x = vertices[0].position.x;
	float max_y = vertices[0].position.y;
	float max_z = vertices[0].position.z;

	float min_x = max_x, min_y = max_y, min_z = max_z;

	for (int i = 1; i < vertices.size(); i++)
	{
		if (vertices[i].position.x > max_x) max_x = vertices[i].position.x;
		else if (vertices[i].position.x < min_x) min_x = vertices[i].position.x;

		if (vertices[i].position.y > max_y) max_y = vertices[i].position.y;
		else if (vertices[i].position.y < min_y) min_y = vertices[i].position.y;

		if (vertices[i].position.z > max_z) max_z = vertices[i].position.z;
		else if (vertices[i].position.z < min_z) min_z = vertices[i].position.z;
	}

	float  m_scale = 100;
	m_scale = m_scale < (1.0 / (max_x - min_x)) ? m_scale : (1.0 / (max_x - min_x));
	m_scale = m_scale < (1.0 / (max_y - min_y)) ? m_scale : (1.0 / (max_y - min_y));
	m_scale = m_scale < (1.0 / (max_z - min_z)) ? m_scale : (1.0 / (max_z - min_z));

	for (auto& vertice : vertices)
	{
		vertice.position *= m_scale;
	}
}

void disc0ver::Model::createMesh(std::string materialName, std::vector<Material>& materials)
{
	int start = 0;
	for(auto mesh: meshes)
	{
		start += mesh.indices.size();
	}
	std::vector<Texture> tmp;
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		tmp.push_back(it->second);
	}

	for (int i = start; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	Mesh mesh(vertices, indices, tmp);
	
	if(!materialName.empty())
	{
		for (auto material : materials)
		{
			if(material.name == materialName)
			{
				mesh.addMaterial(material);
				break;
			}
		}
	}
	meshes.push_back(mesh);
	indices.clear();
}

void disc0ver::Model::loadMaterial(std::vector<Material>& materials, std::string path)
{
	std::ifstream infile;
	std::string tmp_str;
	infile.open(path);

	if (!infile.is_open())
	{
		std::cout << "load materials fail " << path << std::endl;
		return;
	}

	char line[256];
	Material tempMaterial;
	bool listening = false;

	while (!infile.eof())
	{
		infile.getline(line, sizeof(line));
		std::istringstream ss(line);

		ss >> tmp_str;
		if (tmp_str == "newmtl")
		{
			if(!listening)
			{
				ss >> tempMaterial.name;
				listening = true;
			}
			else
			{
				materials.push_back(tempMaterial);
				
				tempMaterial = Material();
				ss >> tempMaterial.name;
			}
		}
		else if(tmp_str == "Ns")
		{
			ss >> tempMaterial.Ns;
		}
		else if(tmp_str == "Ni")
		{
			ss >> tempMaterial.Ni;
		}
		else if(tmp_str == "d")
		{
			ss >> tempMaterial.d;
		}
		else if(tmp_str == "illum")
		{
			ss >> tempMaterial.illum;
		}
		else if(tmp_str == "map_Kd")
		{
			std::string mapPath;
			ss >> mapPath;
			if (path.find('/') != std::string::npos)
				mapPath = path.substr(0, path.find_last_of('/') + 1) + mapPath;
			tempMaterial.map_Kd = mapPath;
		}
		else if(tmp_str == "Ka")
		{
			ss >> tempMaterial.Ka[0] >> tempMaterial.Ka[1] >> tempMaterial.Ka[2];
		}
		else if(tmp_str == "Kd")
		{
			ss >> tempMaterial.Kd[0] >> tempMaterial.Kd[1] >> tempMaterial.Kd[2];
		}
		else if(tmp_str == "Ks")
		{
			ss >> tempMaterial.Ks[0] >> tempMaterial.Ks[1] >> tempMaterial.Ks[2];
		}
		tmp_str = "";
	}
	if (listening)
		materials.push_back(tempMaterial);
}
