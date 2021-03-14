#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec4 ourColor;
uniform float iTime;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourColor = vec4(aColor, 1.0);
   ourColor.x = abs(sin(iTime + aColor.x));
   //ourColor.y = abs(sin(iTime + aColor.y));
   ourColor.z = abs(sin(iTime + aColor.z));
};

#shader fragment
#version 330 core
out vec4 FragColor;
in vec4 ourColor;
void main()
{
   FragColor = ourColor;
};
