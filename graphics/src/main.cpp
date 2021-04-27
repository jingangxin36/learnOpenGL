#include <glad/glad.h> 
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
#include "Model.h"
#include "Camera.h"

float mixValue = 0.f;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -5.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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

	float cubeVertices[] = {
		// positions          // texture Coords
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

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  0.0f,  1.0f
	};

	vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}



		stbi_set_flip_vertically_on_load(true);
		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


		Shader shader("res/Shaders/stencil_testing.shader");
		Shader shaderSingleColor("res/Shaders/stencil_single_color.shader");


		VertexArray va;
		VertexBuffer vb(cubeVertices, 12 * 3 * (3 + 2) * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);//position
		layout.Push<float>(2);//uv
		va.AddBuffer(vb, layout);

		VertexArray va_plane;
		VertexBuffer vb_plane(planeVertices, 6 * (3 + 2) * sizeof(float));
		VertexBufferLayout layout_plane;
		layout_plane.Push<float>(3);//position
		layout_plane.Push<float>(2);//uv
		va_plane.AddBuffer(vb_plane, layout_plane);

		VertexArray va_transparent;
		VertexBuffer vb_transparent(transparentVertices, 6 * (3 + 2) * sizeof(float));
		VertexBufferLayout layout_transparent;
		layout_transparent.Push<float>(3);//position
		layout_transparent.Push<float>(2);//uv
		va_transparent.AddBuffer(vb_transparent, layout_transparent);

		Texture texture_cube("res/Textures/marble.jpg");
		Texture texture_plane("res/Textures/metal.png");
		Texture texture_transparent("res/Textures/window.png");
		texture_plane.Bind(0);
		shader.Bind();
		shader.SetUniform1i("texture1", 0);

		Renderer renderer;
		// render loop
		while (!glfwWindowShouldClose(window))
		{
			//for camera
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			processInput(window);

			glfwSetCursorPosCallback(window, mouse_callback);

			glfwSetScrollCallback(window, scroll_callback);

			// render
			// ------
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			renderer.Clear();

			// set uniforms
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

			shaderSingleColor.Bind();
			shaderSingleColor.SetUniformMat4f("view", view);
			shaderSingleColor.SetUniformMat4f("projection", projection);

			shader.Bind();
			shader.SetUniformMat4f("view", view);
			shader.SetUniformMat4f("projection", projection);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			// draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
			glStencilMask(0x00);
			// floor
			texture_plane.Bind(0);
			shader.SetUniformMat4f("model", glm::mat4(1.0f));
			renderer.DrawArray(va_plane, 6, shader);

			// 1st. render pass, draw objects as normal, writing to the stencil buffer
			// --------------------------------------------------------------------
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);


			// cubes
			texture_cube.Bind(0);

			//cube1
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			shader.SetUniformMat4f("model", model);
			renderer.DrawArray(va, 36, shader);

			//cube2
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			shader.SetUniformMat4f("model", model);
			renderer.DrawArray(va, 36, shader);

			// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
			// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
			// the objects' size differences, making it look like borders.
			// -----------------------------------------------------------------------------------------------------------------------------
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);

			shaderSingleColor.Bind();
			float scale = 1.1;
			// cubes

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			shaderSingleColor.SetUniformMat4f("model", model);
			renderer.DrawArray(va, 36, shaderSingleColor);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			shaderSingleColor.SetUniformMat4f("model", model);
			renderer.DrawArray(va, 36, shaderSingleColor);
			
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glEnable(GL_DEPTH_TEST);

			//glass
			std::map<float, glm::vec3> sorted;
			for (unsigned int i = 0; i < vegetation.size(); i++)
			{
				float distance = glm::length(camera.Position - vegetation[i]);
				sorted[distance] = vegetation[i];
			}
			shader.Bind();
			texture_transparent.Bind(0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, it->second);
				shader.SetUniformMat4f("model", model);
				renderer.DrawArray(va_transparent, 6, shader);
			}

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}


		// glfw: terminate, clearing all previously allocated GLFW resources.
	}

	glfwTerminate();
	std::cout << "press to quit" << std::endl;
	return 0;
	
}
