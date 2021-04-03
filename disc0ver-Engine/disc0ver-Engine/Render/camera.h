/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-10-07
 */

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace disc0ver {

	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum CameraProjection {
		PERSEPCTIVE,
		ORTHO
	};

	//欧拉角-偏转角 
	const float YAW = -90.0f;
	//欧拉角-俯仰角
	const float PITCH = 0.0f;
	//相机移动速度
	const float SPEED = 2.5f;
	//转向敏感度
	const float SENSITIVITY = 0.1f;
	//fov
	const float ZOOM = 45.0f;

	class IBaseCamera {
	public:
		// 相机属性
		glm::vec3 position_;
		glm::vec3 forward_;
		glm::vec3 up_;
		glm::vec3 right_;
		glm::vec3 world_up_;
		// 欧拉角
		float yaw_;
		float pitch_;
		float roll_;
		// 移动速度 转向敏感度 fov
		float movement_speed_;
		float mouse_sensitivity_;
		float zoom_;

	public:
		IBaseCamera() : forward_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(SPEED),
			mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM) {}

		// 得到view矩阵
		glm::mat4 GetViewMatrix() const;
		// 处理键盘输入—WASD控制相机移动
		virtual void ProcessKeyboard() {}
		// 处理鼠标输入—控制相机转向
		virtual void ProcessMouseMovement() {}
		// 处理滚轮输入—放大/缩小
		virtual void ProcessMouseScroll() {}
	protected:
		void UpdateCameraVectors();
	};

	class FPSCamera : public IBaseCamera {
	public:
		FPSCamera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH
		);

		FPSCamera(
			float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float yaw,
			float pitch
		);
	public:
		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

	};
}

#endif // !CAMERA_H