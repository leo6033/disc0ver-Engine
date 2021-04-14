/*
 * @Description: 窗口类
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-4-14
 */

#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "..\..\Render\camera.h"

namespace disc0ver
{
	class Window
	{
	public:
		// 单例
		static Window& getInstance(int width = 800, int height = 600, const char* title = "disc0ver", IBaseCamera* camera = nullptr)
		{
			static Window window(width, height, title, camera);
			return window;
		}

		Window(const Window&) = delete;

		Window& operator = (const Window&) = delete;

		// 窗口回调函数--当窗口的宽高发生变化时会被调用
		static void framtebuffer_size_callback(GLFWwindow* window, int width, int height);

		// 处理用户输出--如WASD控制摄像机的移动 ESC关闭窗口
		static void processInput(float deltaTime);

		// 鼠标点击回调函数
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

		// 鼠标移动回调函数--它什么也不做
		static void mouse_callback_donothing(GLFWwindow* window, double xpos, double ypos);

		// 鼠标移动回调函数--它会记录鼠标上次的位置
		static void mouse_callback_recordxy(GLFWwindow* window, double xpos, double ypos);

		// 鼠标滚轮回调函数
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		// 窗口宽度
		int getWindowWidth() { return windowWidth; }

		// 窗口高度
		int getWindowHeight() { return windowHeight; }

		// 窗口对象
		GLFWwindow* getGlfwWindowObject() { return glfwWindow; }

		// 设置窗口的主相机
		void setWindowCamera(IBaseCamera* camera) { this->camera = camera; }

		// 窗口是否需要关闭
		int shouldClose() { return glfwWindowShouldClose(glfwWindow); }

		// 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
		void pollEvents() { glfwPollEvents(); }

		// 交换颜色缓冲
		void swapBuffers() { glfwSwapBuffers(glfwWindow); }

	private:
		int windowWidth;
		int windowHeight;
		GLFWwindow* glfwWindow;

		IBaseCamera* camera;

		// 记录鼠标坐标
		bool firstMouse;
		float mouseLastX;
		float mouseLastY;

		// 构造函数
		Window(int width, int height, const char* title, IBaseCamera* camera) :windowWidth(width), windowHeight(height)
		{
			glfwWindow = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
			if (glfwWindow == NULL)
			{
				throw "Fail to create window";
			}
			firstMouse = true;
			mouseLastX = windowWidth / 2.0f;
			mouseLastY = windowHeight / 2.0f;
			this->camera = camera;
			glfwMakeContextCurrent(glfwWindow);
			glfwSwapInterval(1);
			glfwSetFramebufferSizeCallback(glfwWindow, framtebuffer_size_callback);
			glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
			glfwSetScrollCallback(glfwWindow, scroll_callback);
		}
	};

}
#endif // !WINDOW_H
