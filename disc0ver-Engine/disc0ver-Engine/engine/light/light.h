/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-10-16
 */

#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <../../UGM/rgb.h>
#include "../../render/transform.h"

namespace disc0ver {

	class Light {
	protected:
		Light() = default;
		Light(Position position = { 0.0f, 0.0f, 0.0f }, Rotation rotation = { 0.0f, 0.0f, 0.0f }, Scale scale = { 1.0f, 1.0f, 1.0f }, Ubpa::rgbf color = Ubpa::rgbf()) : transform(rotation, position, scale), color(color) {}
		virtual ~Light() = default;
	public:
		Ubpa::rgbf color;
		Transform transform;
	};
}

#endif // !LIGHT_H