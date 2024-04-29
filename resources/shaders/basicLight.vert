#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 scaleFactor;

uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec4 FragPosLightSpace;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Œ∆¿Ì”≥…‰µƒÀı∑≈
	vec2 scale = vec2(1.0f);
	if(abs(aNormal) == vec3(0,0,1)) scale = scaleFactor.xy;
	else if(abs(aNormal) == vec3(0,1,0)) scale = scaleFactor.xz;
	if(abs(aNormal) == vec3(1,0,0)) scale = scaleFactor.yz;
	TexCoords = scale * aTex;

	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));

	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}

