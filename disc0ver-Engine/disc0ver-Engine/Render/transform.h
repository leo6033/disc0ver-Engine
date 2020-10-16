/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-10-02
 */

#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace disc0ver {
	struct Rotation {
		float x, y, z;
	};

	struct Position {
		float x, y, z;
	};

	struct Scale {
		float x, y, z;
	};

	class Transform {
	public:
		glm::mat4 trans;
		void use();
	//private:
		Rotation rotation = { 0.0f, 0.0f, 0.0f };
		Position position = { 0.0f, 0.0f, 0.0f };
		Scale scale = { 1.0f, 1.0f, 1.0f };
	};
}

#endif // !TRANSFORM_H