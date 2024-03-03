#version 450 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform bool bUseTexture;

uniform vec3 color;
uniform sampler2D tex;

void main() {

	FragColor = bUseTexture? texture(tex, TexCoords) : vec4(color, 1.0f);
}



