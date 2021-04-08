#include "test_total.h"

#include "../render/shader.h"
#include "../render/graph.h"
#include "../render/camera.h"
#include "../engine/light/light.h"
#include "../engine/window/window.h"

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

//FPS相机
disc0ver::FPSCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//记录时间帧间隔
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int test_master_main() {
	
	//glfw初始化
	glfwInit();
	//创建窗口
	disc0ver::Window& window = disc0ver::Window::getInstance(800, 600, "disc0ver", &camera);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//开启深度测试
	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window.getGlfwWindowObject(), true);
	ImGui_ImplOpenGL3_Init("#version 420 core");
	ImGui::StyleColorsClassic();

	disc0ver::Shader shader("shader/phong.vs", "shader/phong.fs");
	disc0ver::Shader lightShader("shader/light_shader.vs", "shader/light_shader.fs");

	std::vector<std::shared_ptr<disc0ver::IBaseModel>> models;
	
	std::shared_ptr<disc0ver::IBaseModel> cube(new disc0ver::cubeModel());
	std::shared_ptr<disc0ver::IBaseModel> rect(new disc0ver::rectangleModel());
	std::shared_ptr<disc0ver::pointLightModel> pointLight(new disc0ver::pointLightModel());
	cube->transform.position = disc0ver::Position(2.0f, 0.0f, 0.0f);
	rect->transform.position = disc0ver::Position(-2.0f, 0.0f, 0.0f);
	pointLight->transform.position = disc0ver::Position(0.0f, 0.0f, 3.0f);
	models.push_back(cube);
	models.push_back(rect);
	try
	{
		std::shared_ptr<disc0ver::IBaseModel> model1(new disc0ver::STLModel("models/stlModels/github-skyline/leo6033-2020.stl"));
		std::shared_ptr<disc0ver::IBaseModel> model2(new disc0ver::Model("models/objModels/Marry/Marry.obj"));
		models.push_back(model1);
		models.push_back(model2);
	}catch (const char* msg)
	{
		std::cerr << msg << std::endl;
	}

	shader.use();

	shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	bool show_demo_window = true;

	disc0ver::DirLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	disc0ver::setLightAllAttribute(shader, "dirLight", &dirLight);

	while (!window.shouldClose()) {

		window.pollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Another Window", &show_demo_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				//glfwSetWindowShouldClose(window, true);
				glfwSetWindowShouldClose(window.getGlfwWindowObject(), true);
			if (ImGui::Button("new window"))
			{
				GLFWwindow* newwindow = glfwCreateWindow(800, 600, "new window", NULL, NULL);

			}
			ImGui::End();
		}
		ImGui::Render();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.processInput(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (float)window.getWindowWidth() / (float)window.getWindowHeight(), 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.position_);
		//shader.setVec3("lightPos", std::sin((float)glfwGetTime() * 6) * 100.0f, std::cos((float)glfwGetTime() * 4) * 150.0f, std::cos((float)glfwGetTime() * 2) * 100.0f);
		shader.setVec3("lightPos", glm::vec3(100.0f, 100.0f, 200.0f));

		for (auto& model : models)
		{
			shader.setMat4("model", model->transform.trans);
			model->draw(shader);
		}

		lightShader.use();
		lightShader.setMat4("model", pointLight->transform.trans);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		pointLight->draw(lightShader);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.swapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}