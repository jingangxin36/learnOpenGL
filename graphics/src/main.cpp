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

float mixValue = 0.f;

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
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	Shader shader("res/Shaders/Basic.shader");
	shader.Bind();


	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f 
	};

	unsigned int indices[] = {
		0, 1, 2, // first
		2, 3, 0, // first
	};

	VertexArray va;
	VertexBuffer vb(vertices, 4 * 8 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);//position
	layout.Push<float>(3);//color
	layout.Push<float>(2);//uv
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 3 * 2);

	Texture texture("res/Textures/awesome.png");
	shader.SetUniform1i("ourTexture0", 0);

	Texture texture1("res/Textures/container.jpg");
	shader.SetUniform1i("ourTexture1", 1);
 
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		renderer.Clear();
		texture1.Bind(1);
		texture.Bind();

		shader.Bind();//for set uniform!!
		shader.SetUniform1f("mixValue", mixValue);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.SetUniformMat4f("trans", transform);

		renderer.Draw(va, ib, shader);

		transform = glm::mat4(1.f);;
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
		transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f) * glm::abs(glm::sin((float)glfwGetTime())));
		shader.SetUniformMat4f("trans", transform);


		renderer.Draw(va, ib, shader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;

}
