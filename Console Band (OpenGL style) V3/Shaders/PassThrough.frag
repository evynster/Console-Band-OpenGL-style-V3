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
uniform vec3 uViewDir = vec3(0, 0, 1);

uniform bool toonActive;

in vec2 texcoord;

out vec4 outColor;

void pointLight();

vec3 calculatePointLight();

void defaultLight() { pointLight(); }

void spotLight() {
  //vec4 viewDir = mat4(mat3(uView)) * vec4(0, 0, 1, 1);

  //float theta = max(dot(LightDirection, uViewDir.xyz), 0.0);
//if( theta > .1)
    outColor.rgb += calculatePointLight();

}

vec3 calculatePointLight() 
{

  vec3 norm = (texture(uNorm, texcoord) * 2 - 1).rgb;
  vec3 pos = (texture(uPos, texcoord) * 2 - 1).rgb;
  //account for rasterizer interpolating
  vec3 normal = normalize((texture(uNorm, texcoord) * 2 - 1).rgb);
  
  vec3 lightVec =  LightPosition.xyz - (texture(uPos, texcoord) * 2 - 1).rgb;
  float dist = length(lightVec);
  vec3 lightDir = lightVec / dist;
  
  float NdotL = dot(normal, lightDir);
  
  vec3 diffuse, specular;
  if(NdotL > 0.0)
  {
    //The light contributes to this surface
    
    //Calculate attenuation (falloff)
    float attenuation = 1.0 / (Attenuation_Constant + (Attenuation_Linear * dist) + (Attenuation_Quadratic * dist * dist));
    
    //Calculate diffuse contribution
    diffuse = LightDiffuse * NdotL * attenuation;
    
    //Blinn-Phong half vector
    float NdotHV =  max(dot(normal, normalize(lightDir + normalize(-(texture(uPos, texcoord) * 2 - 1).rgb))), 0.0); 
    
    //Calculate specular contribution
    specular += LightSpecular * pow(NdotHV, LightSpecularExponent) * attenuation;
  }

  return diffuse + specular;






//  vec4 normal = texture(uNorm, texcoord) * 2 - 1;
//  normal = normal / normal.w;
//
//  //vec4 viewDir = mat4(mat3(uView)) * vec4(0, 0, 1, 1);
//  vec3 lightDir =
//    normalize(( (texture(uPos, texcoord) * 2 - 1)- LightPosition).xyz );
//
//  float NdotL = max(dot(normal.xyz, lightDir) , 0.0);
//
//  vec3 reflectDir = reflect(-lightDir, normal.xyz);
//  float spec = pow(max(dot(uViewDir.xyz, reflectDir), 0), LightSpecularExponent);
//
//  float dist = length(LightPosition.xyz - (texture(uPos, texcoord).xyz * 2 - 1));
//  float attenuation =
//      1.0 / (Attenuation_Constant + (Attenuation_Linear * dist) +
//             (Attenuation_Quadratic * dist * dist));
//
// //Blinn-Phong half vector
//  float NdotHV =  max(dot(normal.xyz, normalize(lightDir + normalize(-(texture(uPos, texcoord).xyz * 2 - 1)))), 0.0); 
//        
//  vec3 diffuse = LightDiffuse * NdotL * attenuation;
//  vec3 specular = LightSpecular * pow(NdotHV, LightSpecularExponent)  * attenuation;
//
//  //if(!toonActive)
//  //{
//  //  return texture(uRamp, vec2((1 - NdotL),0.5)).rgb/* * (diffuse + specular)*/;
//  //}
//  return (diffuse + specular) * (texture(uRamp, vec2((NdotL),0.5)).rgb * int(toonActive));
}

void pointLight() { outColor.rgb += calculatePointLight(); }

vec3 calculateDirectionalLight() {
  vec3 normal = normalize(texture(uNorm, texcoord).rgb) * 2 - 1;

  vec3 lightDir = normalize(LightDirection);
  
  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(uViewDir, reflectDir), 0), LightSpecularExponent);

  vec3 diffuse = LightDiffuse;
  vec3 specular = LightSpecular;

  
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

  if (LightEnable)
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