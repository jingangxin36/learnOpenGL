#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(-aPos.x, aPos.y, aPos.z, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float mixValue;

void main()
{
	vec4 color0 = texture(ourTexture0, TexCoord);
	vec4 color1 = texture(ourTexture1, TexCoord);
	if (color0.a < 0.1)
		discard;
	if (color1.a < 0.1)
		discard;
	vec4 texColor = mix(color0, color1, mixValue);
	FragColor = texColor;
}