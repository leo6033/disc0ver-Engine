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
#include <time.h>

const unsigned int SCR_WIDTH_raytracing = 800;
const unsigned int SCR_HEIGHT_raytracing = 600;

disc0ver::FPSCamera rayCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX_raytracing = SCR_WIDTH_raytracing / 2.0f;
float lastY_raytracing = SCR_HEIGHT_raytracing / 2.0f;
bool firstMouse_raytracing = true;

float deltaTime_raytracing = 0.0f;
float lastFrame_raytracing = 0.0f;

void framebuffer_size_callback_raytracing(GLFWwindow* window, int width, int height);
void mouse_button_callback_raytracing(GLFWwindow* window, int button, int action, int mods);
void mouse_callback_raytracing(GLFWwindow* window, double xpos, double ypos);
void mouse_callback1_raytracing(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_raytracing(GLFWwindow* window, double xoffset, double yoffset);
void processInput_raytracing(GLFWwindow* window);

int test_ray_tracing_main() {

#pragma region init gl

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "disc0ver", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_raytracing);
	glfwSetMouseButtonCallback(window, mouse_button_callback_raytracing);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback_raytracing);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

#pragma endregion

#pragma region init imgui

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420 core");
	ImGui::StyleColorsClassic();

#pragma endregion

	glm::ivec2 appSize(SCR_WIDTH_raytracing, SCR_HEIGHT_raytracing);

	disc0ver::Shader shader("shader/raytracingPlus.vs", "shader/raytracingPlus.fs");

	//  perspective projection matrix
	auto perspectiveMat = glm::perspective(glm::radians(rayCamera.zoom_), 
		(float)SCR_WIDTH_raytracing / (float)SCR_HEIGHT_raytracing, 0.1f, 100.0f);

	disc0ver::rectangleModel canvas;

	canvas.Init();
	canvas.transform.position = { 0.0f, 0.0f, 0.0f };
	canvas.transform.scale = { 4.0f,3.0f,1.0f };
	canvas.transform.use();
	shader.use();

	//  transform
	shader.setMat4("_view", rayCamera.GetViewMatrix());
	shader.setMat4("_projection", perspectiveMat);
	shader.setMat4("_model", canvas.transform.trans);

	glm::vec3 leftButtom(0.0f), horizontal(0.0f), vertical(0.0f);
	float theta = glm::radians(rayCamera.zoom_);
	float aspect = (float)appSize.x / (float)appSize.y;
	float height = 2 * tan(theta / 2);
	float width = height * aspect;
	leftButtom = rayCamera.position_ + rayCamera.forward_- width / 2 * rayCamera.right_ - height / 2 * rayCamera.up_;
	horizontal = width * rayCamera.right_;
	vertical = height * rayCamera.up_;
	shader.setVec3("_camera.lookFrom", rayCamera.position_);
	shader.setVec3("_camera.left_buttom", leftButtom);
	shader.setVec3("_camera.horizontal", horizontal);
	shader.setVec3("_camera.vertical", vertical);
	//  get screen size
	shader.setVec2("_screen_size", appSize);

	bool show_demo_window = true;

	glEnable(GL_DEPTH_TEST);
	
	srand((unsigned int)time(NULL));
	float rdSeed[4];
	int SamplerTimes = 3;
	float sssTime = 0.0f;
	
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Raytracing Properties", &show_demo_window);
			//--Raytracing UI
			ImGui::SliderInt("Sampler Times Per Second", &SamplerTimes, 3, 40);
			
			ImGui::End();
		}

		ImGui::Render();

		float currentFrame = glfwGetTime();
		deltaTime_raytracing = currentFrame - lastFrame_raytracing;
		lastFrame_raytracing = currentFrame;

		glfwGetWindowSize(window, &(appSize.x), &appSize.y);

		processInput_raytracing(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setVec2("_screen_size", appSize);
		for (int i = 0; i < 4; i++)rdSeed[i] = static_cast<float>((rand() % 10000)) * 0.0001f;
		glUniform1fv(glGetUniformLocation(shader.ID, "_rdSeed"), 4, rdSeed);
		shader.setInt("_maxSampeler", SamplerTimes);

		theta = glm::radians(rayCamera.zoom_);
		aspect = (float)appSize.x / (float)appSize.y;
		height = 2 * tan(theta / 2);
		width = height * aspect;
		leftButtom = rayCamera.position_ + rayCamera.forward_ - width / 2 * rayCamera.right_ - height / 2 * rayCamera.up_;
		horizontal = width * rayCamera.right_;
		vertical = height * rayCamera.up_;
		shader.setVec3("_camera.lookFrom", rayCamera.position_);
		shader.setVec3("_camera.left_buttom", leftButtom);
		shader.setVec3("_camera.horizontal", horizontal);
		shader.setVec3("_camera.vertical", vertical);
		canvas.draw(shader);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback_raytracing(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput_raytracing(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		rayCamera.ProcessKeyboard(disc0ver::FORWARD, deltaTime_raytracing);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		rayCamera.ProcessKeyboard(disc0ver::BACKWARD, deltaTime_raytracing);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rayCamera.ProcessKeyboard(disc0ver::LEFT, deltaTime_raytracing);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rayCamera.ProcessKeyboard(disc0ver::RIGHT, deltaTime_raytracing);
}

void mouse_button_callback_raytracing(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback_raytracing);
		break;
	default:
		return;
	}
	if (action != GLFW_PRESS) {
		firstMouse_raytracing = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, mouse_callback1_raytracing);
	}

	return;
}

void mouse_callback_raytracing(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse_raytracing)
	{
		lastX_raytracing = xpos;
		lastY_raytracing = ypos;
		firstMouse_raytracing = false;
	}

	float xoffset = xpos - lastX_raytracing;
	float yoffset = lastY_raytracing - ypos;

	lastX_raytracing = xpos;
	lastY_raytracing = ypos;

	rayCamera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_callback1_raytracing(GLFWwindow* window, double xpos, double ypos)
{
}

void scroll_callback_raytracing(GLFWwindow* window, double xoffset, double yoffset)
{
	rayCamera.ProcessMouseScroll(yoffset);
}