#pragma once
#include "Vec3.h"

namespace Sonic {

	struct Mat4
	{
		float data[16];

		Mat4()
			: data{ 0 }
		{
		}

		Mat4(float value)
			: data{ value, value, value, value,
					value, value, value, value,
					value, value, value, value,
					value, value, value, value, }
		{
		}

		Mat4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: data{ m00, m01, m02, m03, 
					m10, m11, m12, m13, 
					m20, m21, m22, m23, 
					m30, m31, m32, m33 }
		{
		}

		Mat4& Translate(const Vec3& v);
		Mat4& Rotate(float angle, const Vec3& mask);
		Mat4& Inverse();

		Mat4 operator*(const Mat4& other);

		static Mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	};

}
