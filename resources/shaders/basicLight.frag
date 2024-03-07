#version 450 core

// ���յ���ɫ���
out vec4 FragColor;

// ������յģ������⡢�����䡢�߹���
in vec3 Normal; // ����
in vec3 FragPos; // Ƭ��λ��
in vec2 TexCoords;

// ��������Ĳ��ʽṹ��
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

// �����Դ�Ĳ�ͬ������Ӱ��
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

uniform vec3 viewPos; // �۲���λ��

uniform bool bUseTexture;

uniform int lightMethod;

void main()
{
	vec3 diffuseColor  = texture(diffuseTex, TexCoords).rgb;
	vec3 specularColor = texture(specularTex, TexCoords).rgb;

	// ����������
	vec3 norm     = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); // ��������Ҫ�Ĺ��߷���ΪƬ��ָ���Դλ��
	float diff    = max(dot(norm, lightDir), 0.0);

	// ���㾵�淴��
	vec3 viewDir    = normalize(viewPos - FragPos);
	vec3 halfVec	= normalize(lightDir + viewDir);

	vec3 reflectDir = reflect(-lightDir, norm); // ���淴����Ҫ�Ĺ��߷���Ϊ��Դָ��Ƭ��λ��

	float Phong = dot(reflectDir, viewDir);
	float BlinnPhong = dot(halfVec, norm);
	float useLightMethod = (lightMethod==0)? Phong : BlinnPhong;

	float spec = pow(max(useLightMethod, 0.0), material.shininess);
	
	// ������+������+�߹�
	vec3 ambient  = light.ambient;
	vec3 diffuse  = light.diffuse * diff;
	vec3 specular = light.specular * spec;

	ambient  *= (bUseTexture)? diffuseColor : material.ambient;
	diffuse  *= (bUseTexture)? diffuseColor : material.diffuse;
	//specular *= (bUseTexture)? specularColor : material.specular;

	// ����������ɫ
	vec3 finalColor = ambient + diffuse + specular;
	FragColor = vec4(finalColor, 1.0);
}