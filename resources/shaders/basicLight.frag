#version 450 core

// 最终的颜色结果
out vec4 FragColor;

// 计算光照的：环境光、漫反射、高光项
in vec3 Normal; // 法线
in vec3 FragPos; // 片段位置
in vec2 TexCoords;

// 定义物体的材质结构体
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

// 定义光源的不同分量的影响
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

uniform vec3 viewPos; // 观察者位置

uniform bool bUseTexture;

uniform int lightMethod;

void main()
{
	vec3 diffuseColor  = texture(diffuseTex, TexCoords).rgb;
	vec3 specularColor = texture(specularTex, TexCoords).rgb;

	// 计算漫反射
	vec3 norm     = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); // 漫反射需要的光线方向为片段指向光源位置
	float diff    = max(dot(norm, lightDir), 0.0);

	// 计算镜面反射
	vec3 viewDir    = normalize(viewPos - FragPos);
	vec3 halfVec	= normalize(lightDir + viewDir);

	vec3 reflectDir = reflect(-lightDir, norm); // 镜面反射需要的光线方向为光源指向片段位置

	float Phong = dot(reflectDir, viewDir);
	float BlinnPhong = dot(halfVec, norm);
	float useLightMethod = (lightMethod==0)? Phong : BlinnPhong;

	float spec = pow(max(useLightMethod, 0.0), material.shininess);
	
	// 环境光+漫反射+高光
	vec3 ambient  = light.ambient;
	vec3 diffuse  = light.diffuse * diff;
	vec3 specular = light.specular * spec;

	ambient  *= (bUseTexture)? diffuseColor : material.ambient;
	diffuse  *= (bUseTexture)? diffuseColor : material.diffuse;
	//specular *= (bUseTexture)? specularColor : material.specular;

	// 计算最终颜色
	vec3 finalColor = ambient + diffuse + specular;
	FragColor = vec4(finalColor, 1.0);
}