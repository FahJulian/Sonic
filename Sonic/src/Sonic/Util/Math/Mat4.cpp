#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Mat4.h"

using namespace Sonic;


Mat4& Mat4::Translate(const Vec3& v)
{
	glm::mat4 result = glm::translate(*((glm::mat4*)this), *((glm::vec3*)&v));
	*this = *((Mat4*)&result);
	return *this;
}

Mat4& Mat4::Rotate(float angle, const Vec3& mask)
{
	glm::mat4 result = glm::rotate(*((glm::mat4*)this), angle, *((glm::vec3*)&mask));
	*this = *((Mat4*)&result);
	return *this;
}

Mat4& Mat4::Inverse()
{
	glm::mat4 result = glm::inverse(*((glm::mat4*)this));
	*this = *((Mat4*)&result);
	return *this;
}

Mat4 Mat4::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glm::mat4 result = glm::ortho(left, right, bottom, top, zNear, zFar);
	return *((Mat4*)&result);
}

Mat4 Mat4::operator*(const Mat4& other)
{
	glm::mat4 result = *((glm::mat4*)this) * *((glm::mat4*)&other);
	return *((Mat4*)&result);
}
