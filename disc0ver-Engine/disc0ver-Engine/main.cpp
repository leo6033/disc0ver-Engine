/*
 * @Description: 
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-09-12
 */

#include "render/shader.h"
#include "render/graph.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "disc0ver", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	float texCoords[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	disc0ver::Shader shader("shader_test.vs", "shader_test.fs");

	disc0ver::rectangleModel rect;
	rect.Init();
	rect.addTexture("wall.jpg");
	rect.addTexture("awesomeface.png");

	shader.use();
	shader.setInt("texture1", (int)0);
	shader.setInt("texture2", (int)1);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		rect.transform.position.y = -0.5f;
		rect.transform.rotation.z = (float)glfwGetTime();

		shader.use();

		unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rect.transform.trans));

		rect.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}