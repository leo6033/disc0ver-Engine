#include "test_total.h"

#include "../render/shader.h"
#include "../render/graph.h"
#include "../render/camera.h"
#include "../engine/light/light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <algorithm>

#include "../stb_image.h"

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

int test_master_main() {
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

	disc0ver::Shader shader("shader/phong.vs", "shader/phong.fs");

	disc0ver::Light light();
	//disc0ver::rectangleModel rect;
	std::vector<std::shared_ptr<disc0ver::IBaseModel>> models;
	
	std::shared_ptr<disc0ver::IBaseModel> cube(new disc0ver::cubeModel());
	std::shared_ptr<disc0ver::IBaseModel> rect(new disc0ver::rectangleModel());
	//models.push_back(cube);
	//models.push_back(rect);
	//std::cout << models[1]->vertices.size() << std::endl;
	try
	{
		//std::shared_ptr<disc0ver::IBaseModel> model(new disc0ver::STLModel("leo6033-2020.stl"));
		std::shared_ptr<disc0ver::IBaseModel> model(new disc0ver::Model("models/Marry.obj"));
		models.push_back(model);
	}catch (const char* msg)
	{
		std::cerr << msg << std::endl;
	}
	
	//rect.Init();
	//rect.addTexture("texture1", "wall.jpg");
	//rect.addTexture("texture2", "awesomeface.png");

	/*cube.Init();
	cube.addTexture("texture1", "wall.jpg");
	cube.addTexture("texture2", "awesomeface.png");*/

	for(auto& model : models)
	{
		model->Init();
	}
	

	shader.use();

	shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	bool show_demo_window = true;
	int width, height;

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
			if (ImGui::Button("new window"))
			{
				GLFWwindow* newwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "new window", NULL, NULL);

			}
			ImGui::End();
		}
		ImGui::Render();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//rect.transform.position.y = -0.5f;
		//rect.transform.rotation.z = (float)glfwGetTime();

		//cube.transform.position.y = -0.5f;
		//cube.transform.rotation.z = (float)glfwGetTime();

		for(auto& model : models)
		{
			model->transform.position.y = -0.5f;
			//model->transform.rotation.z = (float)glfwGetTime();
			//model->transform.rotation.x = -90;
		}
		

		shader.use();
		glfwGetFramebufferSize(window, &width, &height);
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (float)width / (float)height, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.position_);
		shader.setVec3("lightPos", std::sin((float)glfwGetTime() * 6) * 100.0f, std::cos((float)glfwGetTime() * 4) * 150.0f, std::cos((float)glfwGetTime() * 2) * 100.0f);
		unsigned int transformLoc = glGetUniformLocation(shader.ID, "model");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rect.transform.trans));
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cube.transform.trans));

		//rect.draw(shader);
		//cube.draw(shader);
		for (auto& model : models)
		{
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model->transform.trans));
			model->draw(shader);
		}
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