#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;


in vec3 f_colour;
in vec2 f_texCoord;
in float f_texIndex;

uniform sampler2D u_textures[2];


void main()
{
    int index = int(f_texIndex);
	FragColor = texture(u_textures[index], f_texCoord);
}
