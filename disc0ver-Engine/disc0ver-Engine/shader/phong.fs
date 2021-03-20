#version 420 core
out vec4 FragColor;

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
  // float light_atten_coff = uLightIntensity / length(lightPos - FragPos);
  vec3 diffuse =  diff * color;

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow (max(dot(viewDir, reflectDir), 0.0), 35.0);
  vec3 specular = lightColor * spec;  
  
  gl_FragColor = vec4(pow((ambient + diffuse + specular), vec3(1.0/2.2)), 1.0);

}