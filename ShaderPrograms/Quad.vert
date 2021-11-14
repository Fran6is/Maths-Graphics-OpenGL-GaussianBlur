#version 430 

layout (location = 0) in vec2 aPos; 
layout (location = 1) in vec2 aTexCoord; 

out vec2 TexCoord;
flat out int InstanceID;

void main(void)
{
	vec2 Offset = vec2(0);
	Offset.x 	= gl_InstanceID == 0 ? -0.5 : 0.5;

	TexCoord = aTexCoord;

	gl_Position = vec4(aPos + Offset, 0.f, 1.f);

	InstanceID = gl_InstanceID;
}