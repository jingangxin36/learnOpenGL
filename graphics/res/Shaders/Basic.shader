#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float mixValue;

void main()
{
	//vec4 color0 = texture(ourTexture0, TexCoord);
	vec4 color1 = texture(ourTexture1, TexCoord);
	//if (color0.a < 0.1)
	//	discard;
	//if (color1.a < 0.1)
	//	discard;
	FragColor = color1;
}