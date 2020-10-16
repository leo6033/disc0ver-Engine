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

namespace disc0ver {

	class Light {
	protected:
		Light() = default;
		virtual ~Light() = default;
		Ubpa::rgbf color;
		
	public:

	};
}

#endif // !LIGHT_H