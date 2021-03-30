#version 420 core  

layout(location = 0) in vec3 aPos;      //本地——顶点坐标
layout(location = 1) in vec3 aNormal;   //本地——法线坐标
layout(location = 2) in vec2 aTexCoord; //纹理坐标

out vec3 FragPos;   //世界——顶点坐标
out vec3 Normal;    //世界——法线坐标
out vec2 TexCoord;  //纹理坐标

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _projection;

/*main*/
void main()
{  
    FragPos = vec3(_model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(_model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position =_projection * _view * _model * vec4(aPos, 1.0); 
}