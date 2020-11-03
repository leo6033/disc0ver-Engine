/*
 * @Description: 
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-09-12
 */

#include "render/shader.h"
#include "render/graph.h"
#include "render/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <algorithm>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback1(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

disc0ver::FPSCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "disc0ver", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

    glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420 core");
	ImGui::StyleColorsClassic();

	disc0ver::Shader shader("shader_test.vs", "shader_test.fs");

	disc0ver::rectangleModel rect;
	disc0ver::cubeModel cube;
	//rect.Init();
	//rect.addTexture("wall.jpg");
	//rect.addTexture("awesomeface.png");

	cube.Init();
	cube.addTexture("texture1", "wall.jpg");
	cube.addTexture("texture2", "awesomeface.png");

	shader.use();
	shader.setInt("texture1", (int)0);
	shader.setInt("texture2", (int)1);

	bool show_demo_window = true;

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Another Window", &show_demo_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				glfwSetWindowShouldClose(window, true);
			ImGui::End();
		}
		ImGui::Render();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//rect.transform.position.y = -0.5f;
		//rect.transform.rotation.z = (float)glfwGetTime();

		//cube.transform.position.y = -0.5f;
		cube.transform.rotation = {cube.transform.rotation.x(), cube.transform.rotation.y(), (float)glfwGetTime() };

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		unsigned int transformLoc = glGetUniformLocation(shader.ID, "model");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rect.transform.trans));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cube.transform.trans));

		//rect.draw();
		cube.draw();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(disc0ver::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(disc0ver::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(disc0ver::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(disc0ver::RIGHT, deltaTime);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		break;
	default:
		return;
	}
	if (action != GLFW_PRESS) {
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, mouse_callback1);
	}
		
	return;
}

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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_callback1(GLFWwindow* window, double xpos, double ypos)
{
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}