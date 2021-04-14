/*
 * @Description: 光源类
 * @Author: xiji
 * @Email: wncka@foxmail.com
 * @Date: 2021-04-13
 */
#include "window.h"

using disc0ver::Window;

// 窗口回调函数--当窗口的宽高发生变化时会被调用
void disc0ver::Window::framtebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window& wd = Window::getInstance();
	wd.windowWidth = width;
	wd.windowHeight = height;
	glViewport(0, 0, width, height);
}

// 处理用户输出--如WASD控制摄像机的移动 ESC关闭窗口
void disc0ver::Window::processInput(float deltaTime)
{
	Window& wd = Window::getInstance();
	GLFWwindow* window = wd.glfwWindow;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (wd.camera)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			wd.camera->ProcessKeyboard(disc0ver::CameraMovementDirection::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			wd.camera->ProcessKeyboard(disc0ver::CameraMovementDirection::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			wd.camera->ProcessKeyboard(disc0ver::CameraMovementDirection::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			wd.camera->ProcessKeyboard(disc0ver::CameraMovementDirection::RIGHT, deltaTime);
	}
}

// 鼠标点击回调函数
void disc0ver::Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	// 按下
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
			// 鼠标右键
		case GLFW_MOUSE_BUTTON_RIGHT:
			// 隐藏鼠标 且用户可以通过鼠标移动控制相机的转向
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouse_callback_recordxy);
			break;
		default:
			break;
		}
	}
	else
	{
		Window& wd = Window::getInstance();
		wd.firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, mouse_callback_donothing);
	}
}


// 鼠标移动回调函数--它什么也不做
void disc0ver::Window::mouse_callback_donothing(GLFWwindow* window, double xpos, double ypos)
{

}

// 鼠标移动回调函数--它会记录鼠标上次的位置
void disc0ver::Window::mouse_callback_recordxy(GLFWwindow* window, double xpos, double ypos)
{
	Window& wd = Window::getInstance();
	if (wd.firstMouse)
	{
		wd.mouseLastX = xpos;
		wd.mouseLastY = ypos;
		wd.firstMouse = false;
	}
	float xoffset = xpos - wd.mouseLastX;
	float yoffset = wd.mouseLastY - ypos;
	wd.mouseLastX = xpos;
	wd.mouseLastY = ypos;
	if (wd.camera)
		wd.camera->ProcessMouseMovement(xoffset, yoffset);
}

// 鼠标滚轮回调函数
void disc0ver::Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window& wd = Window::getInstance();
	if (wd.camera)
		wd.camera->ProcessMouseScroll(yoffset);
}
