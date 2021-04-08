/*
#version 420 core
out vec4 FragColor;

struct Material
{
  // 漫反射(环境光)贴图 
	sampler2D texture_diffuse;
  // 镜面光贴图
	sampler2D texture_specular;
  // 是否使用上述贴图
	bool use_texture_diffuse;
  bool use_texture_specular;
  // 环境光、漫反射、镜面光 颜色分量(如果不使用贴图 将使用这些值进行计算)
	vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  // 镜面高光
  float shininess;
};

uniform Material material;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

uniform int uTextureSample;
uniform sampler2D texture1;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uLightIntensity;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void) {
  vec3 color;
  if (uTextureSample == 1) {
    color = pow(texture(texture1, TexCoord).rgb, vec3(2.2));
  } else {
    color = uKd;
  }
  
  vec3 ambient = 0.05 * color;

  vec3 lightDir = normalize(lightPos - FragPos);
  vec3 normal = normalize(Normal);
  float diff = max(dot(lightDir, normal), 0.0);
  float light_atten_coff = 10.0 / length(lightPos - FragPos);
  vec3 diffuse =  diff * color * light_atten_coff;

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular = lightColor * spec * light_atten_coff;  
  
  FragColor = vec4(pow((ambient + diffuse + specular), vec3(1.0/2.2)), 1.0);
  FragColor = texture(texture1, TexCoord);
  FragColor = texture(material.texture_diffuse, TexCoord);
}
*/


#version 420 core

// 材质
struct Material
{
  // 漫反射(环境光)贴图 
	sampler2D texture_diffuse;
  // 镜面光贴图
	sampler2D texture_specular;
  // 是否使用上述贴图
	bool use_texture_diffuse;
  bool use_texture_specular;
  // 环境光、漫反射、镜面光 颜色分量(如果不使用贴图 将使用这些值进行计算)
	vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  // 镜面高光
  float shininess;
};

// 方向光
struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// 点光源
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

  // 光强衰减
	float constant;
	float linear;
	float quadratic;
};

// 聚光-手电筒
struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 内圆锥余弦值
	float cutOff;
	// 外圆锥余弦值
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

// 片段法向量(世界坐标)
in vec3 Normal;
// 片段顶点位置(世界坐标)
in vec3 FragPos;
// uv纹理坐标
in vec2 TexCoord;
// 输出片段颜色
out vec4 FragColor;  
// 相机观察位置(世界坐标)
uniform vec3 viewPos;
// 物体材质
uniform Material material;
// 方向光
uniform DirLight dirLight;
// 点光源个数
#define NR_POINT_LIGHTS 2
// 点光源数组
uniform PointLight pointLights[NR_POINT_LIGHTS];
// 聚光灯
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  // 标准化法向量
	vec3 norm = normalize(Normal);
  // 从片段位置指向观察位置的向量
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// 定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 点光源
	//for(int i = 0; i < NR_POINT_LIGHTS; i++)
		//result += CalPointLight(pointLights[i], norm, FragPos, viewDir);
	// 聚光
	//result += CalSpotLight(spotLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0);
}

// 计算某个方向光源对该片段颜色的贡献
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  // 不要忘记标准化向量~
	vec3 lightDir = normalize(-light.direction);
  // 环境光着色
  vec3 ambient;
  if(material.use_texture_diffuse)
    ambient = light.ambient * texture(material.texture_diffuse, TexCoord).rgb;
  else
    ambient = light.ambient * material.ambient_color;
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
  vec3 diffuse;
  if(material.use_texture_diffuse)
    diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
  else
    diffuse = light.diffuse * diff * material.diffuse_color;
	// 镜面光着色
  // 计算反射光线 注意第一个参数是光线的入射方向 所以需要取反
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
  vec3 specular;
  if(material.use_texture_specular)
    specular = light.specular * spec * texture(material.texture_specular, TexCoord).rgb;
  else
    specular = light.specular * spec * material.specular_color;
	// 合并
	return ambient + diffuse + specular;
}

// 计算某个点光源对该片段颜色的贡献
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
  // 环境光着色
  vec3 ambient;
  if(material.use_texture_diffuse)
    ambient = light.ambient * texture(material.texture_diffuse, TexCoord).rgb;
  else
    ambient = light.ambient * material.ambient_color;
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
  vec3 diffuse;
  if(material.use_texture_diffuse)
    diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
  else
    diffuse = light.diffuse * diff * material.diffuse_color;
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
  vec3 specular;
  if(material.use_texture_specular)
    specular = light.specular * spec * texture(material.texture_specular, TexCoord).rgb;
  else
    specular = light.specular * spec * material.specular_color;
	// 距离
	float dis = length(light.position - fragPos);
	// 衰减
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis *dis);
	attenuation = clamp(attenuation, 0.0, 1.0);
	// 合并
	return (ambient + diffuse + specular) * attenuation;
}

// 计算某个聚光灯源对该片段颜色的贡献
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
  // 环境光着色
  vec3 ambient;
  if(material.use_texture_diffuse)
    ambient = light.ambient * texture(material.texture_diffuse, TexCoord).rgb;
  else
    ambient = light.ambient * material.ambient_color;
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
  vec3 diffuse;
  if(material.use_texture_diffuse)
    diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
  else
    diffuse = light.diffuse * diff * material.diffuse_color;
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
  vec3 specular;
  if(material.use_texture_specular)
    specular = light.specular * spec * texture(material.texture_specular, TexCoord).rgb;
  else
    specular = light.specular * spec * material.specular_color;
	// 距离
	float dis = length(light.position - fragPos);
	// 衰减
	float attenuation = 1.0 /(light.constant + light.linear * dis + light.quadratic * dis * dis);
	attenuation = clamp(attenuation, 0.0, 1.0);
	// 内外光切角插值 实现平滑过度效果
	float cosTheta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((cosTheta  - light.outerCutOff) / epsilon, 0.0, 1.0);
	// 合并
	return (ambient + diffuse + specular) * attenuation * intensity;
}
