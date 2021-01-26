#version 420

// enums
const int DEFAULT = 0;
const int POINT = 1;
const int DIRECTIONAL = 2;
const int SPOTLIGHT = 3;

uniform vec4 LightPosition;

// colour
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

// scalar
uniform float LightSpecularExponent;
uniform float Attenuation_Constant;
uniform float Attenuation_Linear;
uniform float Attenuation_Quadratic;

// amount of lights
uniform int LightAmount;

// other
uniform bool LightEnable;
uniform int LightType;
uniform vec3 LightDirection;
uniform float LightAngleConstraint;

// uniform sampler2D uTex;
uniform sampler2D uPos;
uniform sampler2D uNorm;
uniform sampler2D uScene;
uniform sampler2D uRamp;
uniform vec4 uViewPos;

uniform bool toonActive;

in vec2 texcoord;

out vec4 outColor;

void pointLight();
void directionalLight();
vec3 calculatePointLight();

void defaultLight() { directionalLight(); }

void spotLight() 
{
  

}

vec3 blinnPhong(vec3 lightDir, vec3 viewDir )
{
  vec3 norm = (texture(uNorm, texcoord)).rgb;
  vec3 pos  = (texture(uPos, texcoord)).rgb;//frag position
  
  vec3 diffuse, specular;
 
 
  vec3 halfDir    = normalize(lightDir + viewDir);

//Blinn-Phong Lighting here:
  
  //Diffuse   
  diffuse = max(dot(norm, lightDir),0.0) * LightDiffuse;

  //Specular  
  specular = pow(max(dot(norm, halfDir),0.0), LightSpecularExponent) *  LightSpecular;

 
  return (diffuse + specular) * int(LightEnable);
}

vec3 calculatePointLight(){

  
  vec3 pos  = (texture(uPos, texcoord)).rgb;//frag position
  vec3 lightDir   = LightPosition.xyz - pos;
  vec3 viewDir    = uViewPos.xyz - pos;   
  
//Atenuation calculation
  float dist= length(LightPosition.xyz - pos);
  float attenuation = ( 1.0 / (Attenuation_Constant + Attenuation_Linear * dist + Attenuation_Quadratic * (dist * dist)));
 
  return blinnPhong(lightDir, viewDir) * attenuation;

}

void pointLight() {   
 outColor.rgb += calculatePointLight();

 }

vec3 calculateDirectionalLight() {
  vec3 norm = (texture(uNorm, texcoord)).rgb;
  vec3 pos  = (texture(uPos, texcoord)).rgb;
  
  
 // float diff = max(dot(norm, lightDir), 0.0);

 // vec3 reflectDir = reflect(-lightDir, norm);
  vec3 viewDir    = uViewPos.xyz - pos;
  //float spec = pow(max(dot(viewDir, reflectDir), 0), LightSpecularExponent);

  vec3 diffuse = LightDiffuse;
  vec3 specular = LightSpecular;

  //Blinn-Phong Lighting here:
  
  //Diffuse 
  vec3 lightDir = normalize(-LightDirection);
 diffuse = max(dot(norm, lightDir),0.0) * LightDiffuse;

  //Specular
  vec3 halfDir    = normalize(lightDir + viewDir);
  specular = pow(max(dot(viewDir, halfDir),0.0), LightSpecularExponent) *  LightSpecular;


  
  return diffuse + specular;
}

void directionalLight() {
 
  outColor.rgb += calculateDirectionalLight();
}


void main() {

  vec3 colour = texture(uScene, texcoord).rgb;

  // Ambient Light
  float ambientStrength = 1.0;
  outColor.rgb = colour * LightAmbient * ambientStrength;

 
    switch (LightType) {
    case POINT:
      pointLight();
      break;
    case DIRECTIONAL:
      directionalLight();
      break;
    case SPOTLIGHT:
      spotLight();
      break;
    default:
      defaultLight();
    }


  //outColor = normalize(texture(uPos,texcoord));
  //outColor.rgb = abs(texture(uNorm,texcoord).rgb);
  outColor.a = 1;
}