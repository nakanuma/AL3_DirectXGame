#include "MyMath.h"
#include <math.h>
#include <cmath>
#include <cassert>

Vector3 MyMath::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MyMath::Multiply(const Matrix4x4& m1, const Matrix4x4 m2){
	Matrix4x4 result = Identity();

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;
}

Matrix4x4 MyMath::Identity() {
	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MyMath::Scaling(Vector3 scale) {
	Matrix4x4 result = Identity();

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

Matrix4x4 MyMath::Translation(Vector3 translation) {
	Matrix4x4 result = Identity();

	result.m[3][0] = translation.x;
	result.m[3][1] = translation.y;
	result.m[3][2] = translation.z;

	return result;
}

Matrix4x4 MyMath::RotationX(float rad) {
	Matrix4x4 result = Identity();

	result.m[1][1] = cosf(rad);
	result.m[2][1] = -sinf(rad);
	result.m[1][2] = sinf(rad);
	result.m[2][2] = cosf(rad);

	return result;
}

Matrix4x4 MyMath::RotationY(float rad) {
	Matrix4x4 result = Identity();

	result.m[0][0] = cosf(rad);
	result.m[0][2] = -sinf(rad);
	result.m[2][0] = sinf(rad);
	result.m[2][2] = cosf(rad);

	return result;
}

Matrix4x4 MyMath::RotationZ(float rad) {
	Matrix4x4 result = Identity();

	result.m[0][0] = cosf(rad);
	result.m[1][0] = -sinf(rad);
	result.m[0][1] = sinf(rad);
	result.m[1][1] = cosf(rad);

	return result;
}

Matrix4x4 MyMath::RotationXYZ(float rotateX, float rotateY, float rotateZ) { 
	Matrix4x4 result = Identity();

	result = Multiply(result, RotationX(rotateX));
	result = Multiply(result, RotationY(rotateY));
	result = Multiply(result, RotationZ(rotateZ));

	return result;
}

Matrix4x4 MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) { 
	Matrix4x4 result = Identity();

	result = Multiply(result, Scaling(scale));
	result = Multiply(result, RotationXYZ(rotate.x, rotate.y, rotate.z));
	result = Multiply(result, Translation(translate));

	return result;
}



Vector3 MyMath::Lerp(Vector3 a, Vector3 b, float t) { 
	// tが0~1の範囲に収まるようにクランプする
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	return Vector3{
		a.x + t * (b.x - a.x),
		a.y + t * (b.y - a.y), 
		a.z + t * (b.z - a.z)
	};
}

float MyMath::EaseInOutQuad(float t) { 
	return t < 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f; 
}

bool MyMath::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x < aabb2.max.x && aabb1.max.x > aabb2.min.x) &&  // x軸
		(aabb1.min.y < aabb2.max.y && aabb1.max.y > aabb2.min.y) &&  // y軸
		(aabb1.min.z < aabb2.max.z && aabb1.max.z > aabb2.min.z)) {  // z軸
		return true;
	}

	return false;
}

