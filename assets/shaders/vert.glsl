#version 460 core

// Positions
layout (location = 0) in vec3 v_pos;
// Colors
layout (location = 1) in vec3 v_colour;
// Texture Coordinates
layout (location = 2) in vec2 v_texCoord;
// Texture Index
layout (location = 3) in float v_texIndex;
 


out vec3 f_colour;
out vec2 f_texCoord;
out float f_texIndex;



void main()
{
	gl_Position = vec4(v_pos.x, v_pos.y, v_pos.z, 1.0);
    
	f_colour = v_colour;

	f_texCoord = v_texCoord;

    f_texIndex = v_texIndex;
}
