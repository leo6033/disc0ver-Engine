/*
 * @Description:
 * @Author: ����
 * @Email: long452a@163.com
 * @Date: 2020-10-07
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "camera.h"


#endif // !CAMERA_H

glm::mat4 disc0ver::IBaseCamera::GetViewMatrix() const
{
	return glm::lookAt(position_, position_ + forward_, world_up_);
}

void disc0ver::IBaseCamera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

	forward_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(forward_, world_up_));  
	up_ = glm::normalize(glm::cross(right_, forward_));
}

/*************************************** FPSCamera ***************************************************/
disc0ver::FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : IBaseCamera()
{
	position_ = position;
	world_up_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

disc0ver::FPSCamera::FPSCamera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) : IBaseCamera()
{
	position_ = glm::vec3(pos_x, pos_y, pos_z);
	world_up_ = glm::vec3(up_x, up_y, up_z);
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

void disc0ver::FPSCamera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movement_speed_ * deltaTime;
	if (direction == FORWARD)
		position_ += forward_ * velocity;
	if (direction == BACKWARD)
		position_ -= forward_ * velocity;
	if (direction == LEFT)
		position_ -= right_ * velocity;
	if (direction == RIGHT)
		position_ += right_ * velocity;
}

void disc0ver::FPSCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouse_sensitivity_;
	yoffset *= mouse_sensitivity_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	if (constrainPitch) {
		pitch_ = pitch_ > 89.0f ? 89.0f : pitch_;
		pitch_ = pitch_ < -89.0f ? -89.0f : pitch_;
	}

	UpdateCameraVectors();
}

void disc0ver::FPSCamera::ProcessMouseScroll(float yoffset)
{
	zoom_ -= yoffset;
	zoom_ = zoom_ < 1.0f ? 1.0f : zoom_;
	zoom_ = zoom_ > 45.0f ? 45.0f : zoom_;
}

/*************************************** XXXCamera ***************************************************/