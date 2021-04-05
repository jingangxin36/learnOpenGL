﻿#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float mixValue = 0.f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01f;
		if (mixValue >= 1.f)
		{
			mixValue = 0.f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01f;
		if (mixValue <= 0.f)
		{
			mixValue = 1.f;
		}
	}

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//auto y = cameraPos.y;//fps
	
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
	//cameraPos.y = y;//fps
}

float lastX = 400, lastY = 300;
float yaw = 0, pitch = 0;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

float fov = 45.f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up)
{
	auto zAxis = eye - target;
	auto xAxis = glm::cross(up, zAxis);
	auto yAxis = glm::cross(zAxis, xAxis);

	yAxis = glm::normalize(yAxis);
	zAxis = glm::normalize(zAxis);
	xAxis = glm::normalize(xAxis);
	
	glm::mat4 rotation(1.0f);
	rotation[0][0] = xAxis.x;
	rotation[1][0] = xAxis.y;
	rotation[2][0] = xAxis.z;

	rotation[0][1] = yAxis.x;
	rotation[1][1] = yAxis.y;
	rotation[2][1] = yAxis.z;

	rotation[0][2] = zAxis.x;
	rotation[1][2] = zAxis.y;
	rotation[2][2] = zAxis.z;

	glm::mat4 translation(1.0f);
	translation[3][0] = -eye.x;
	translation[3][1] = -eye.y;
	translation[3][2] = -eye.z;
	
	return rotation * translation;
}

int main(int argc, char* argv[])
{
	std::cout << "hello world" << std::endl;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glViewport(0, 0, 800, 600);

		Shader shader("res/Shaders/Basic.shader");
		shader.Bind();


		glEnable(GL_DEPTH_TEST);
		// set up vertex data (and buffer(s)) and configure vertex attributes
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2, // first
			2, 3, 0, // first
		};

		VertexArray va;
		VertexBuffer vb(vertices, 12 * 3 * (2 + 3) * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);//position
		//layout.Push<float>(3);//color
		layout.Push<float>(2);//uv
		va.AddBuffer(vb, layout);

		//IndexBuffer ib(indices, 3 * 2);

		//Texture texture("res/Textures/awesome.png");
		//shader.SetUniform1i("ourTexture0", 0);

		//Texture texture1("res/Textures/container.jpg");
		//shader.SetUniform1i("ourTexture1", 1);

		va.Unbind();
		vb.Unbind();
		//ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		// render loop
		while (!glfwWindowShouldClose(window))
		{
			// input
			processInput(window);

			glfwSetCursorPosCallback(window, mouse_callback);

			glfwSetScrollCallback(window, scroll_callback);

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

			renderer.Clear();
			//texture1.Bind(1);
			//texture.Bind();

			shader.Bind();//for set uniform!!
			//shader.SetUniform1f("mixValue", mixValue);

			glm::mat4 model(1.0f);
			shader.SetUniformMat4f("model", model);

			glm::mat4 view;
			view = myLookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			shader.SetUniformMat4f("view", view);

			const glm::mat4 projection = glm::perspective(glm::radians(fov), float(800) / float(600), 0.1f, 100.f);
			shader.SetUniformMat4f("projection", projection);


			renderer.DrawArray(va, 36, shader);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwPollEvents();
			glfwSwapBuffers(window);
		}


		// glfw: terminate, clearing all previously allocated GLFW resources.
	}

	glfwTerminate();
	std::cout << "press to quit" << std::endl;
	return 0;
	
}
