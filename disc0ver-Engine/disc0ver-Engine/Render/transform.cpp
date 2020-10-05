/*
 * @Description:
 * @Author: НэПл
 * @Email: long452a@163.com
 * @Date: 2020-10-02
 */

#include "transform.h"

void disc0ver::Transform::use()
{
	trans = glm::mat4(1.0f);
	
	trans = glm::translate(trans, glm::vec3(position.x, position.y, position.z));

	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	trans = glm::scale(trans, glm::vec3(scale.x, scale.y, scale.z));

}
