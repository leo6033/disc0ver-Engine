/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-10-02
 */

#include "transform.h"

void disc0ver::Transform::use()
{
	/*
		计算变换矩阵
	*/
	// 实际的变换顺序应该与阅读顺序相反
	trans = glm::mat4(1.0f);
	// 最后平移
	trans = glm::translate(trans, glm::vec3(position.x, position.y, position.z));
	// 再旋转
	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 先缩放
	trans = glm::scale(trans, glm::vec3(scale.x, scale.y, scale.z));

}
