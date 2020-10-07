/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-10-07
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace disc0ver {
	
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
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
		// 相机选项
		float movement_speed_;
		float mouse_sensitivity_;
		float zoom_;

		glm::mat4 GetViewMatrix() const;
		virtual void ProcessKeyboard() {};
		virtual void ProcessMouseMovement() {};
		virtual void ProcessMouseScroll() {};
	protected:
		void UpdateCameraVectors();
	};

	class FPSCamera : public IBaseCamera {
	public:
		FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		FPSCamera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);
		void ProcessKeyboard(CameraMovement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(float yoffset);
	private:
		void Init() {
			forward_ = glm::vec3(0.0f, 0.0f, -1.0f);
			movement_speed_ = SPEED;
			mouse_sensitivity_ = SENSITIVITY;
			zoom_ = ZOOM;
		}
	};
}