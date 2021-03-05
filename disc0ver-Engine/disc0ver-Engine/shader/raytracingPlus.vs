#version 420 core  

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

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