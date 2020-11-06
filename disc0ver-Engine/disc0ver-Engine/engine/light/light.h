/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-10-16
 */

#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "../../render/transform.h"

namespace disc0ver {

	class Light {
	protected:
		Light() = default;
		Light(Position position = { 0.0f, 0.0f, 0.0f }, Rotation rotation = { 0.0f, 0.0f, 0.0f }, Scale scale = { 1.0f, 1.0f, 1.0f }, rgb color = { 1.0f,1.0f,1.0f }) : transform(rotation, position, scale), color(color) {}
		virtual ~Light() = default;
	public:
		rgb color;
		Transform transform;
	};
}

#endif // !LIGHT_H