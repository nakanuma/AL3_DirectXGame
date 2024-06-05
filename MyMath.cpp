#include "MyMath.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>

Vector3 MyMath::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 MyMath::Multiply(const float& scalar, const Vector3& v) {
	Vector3 result;

	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;

	return result;
}

Vector3 MyMath::Multiply(const Vector3& v, const Matrix4x4& m) {
	Vector3 result;

	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3];
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3];
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3];

	return result;
}

Vector3 MyMath::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	return Vector3{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], 
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], 
		v.x * m.m[0][2] + v.y + m.m[1][2] + v.z * m.m[2][2]
	};
}

float MyMath::Length(const Vector3& v) { return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2)); }

Vector3 MyMath::Normalize(const Vector3& v) {
	float norm = Length(v);

	return Vector3(v.x / norm, v.y / norm, v.z / norm);
}

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

Matrix4x4 MyMath::Multiply(const Matrix4x4& m1, const Matrix4x4 m2) {
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

Matrix4x4 MyMath::Inverse(Matrix4x4 matrix) {
	Matrix4x4 result;
	float temp[4][8] = {};

	float a;

	// 一時行列にコピー
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			temp[i][j] = matrix.m[i][j];

			if (i == j)
				temp[i][4 + j] = 1;
		}
	}

	for (int32_t k = 0; k < 4; k++) {
		a = 1 / temp[k][k];

		for (int32_t j = 0; j < 8; j++) {
			temp[k][j] *= a;
		}

		for (int32_t i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			a = -temp[i][k];

			for (int32_t j = 0; j < 8; j++) {
				temp[i][j] += temp[k][j] * a;
			}
		}
	}

	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			result.m[i][j] = temp[i][4 + j];
		}
	}
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

Matrix4x4 MyMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = Identity();

	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0.0f;

	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;

	return result;
}

float MyMath::DegToRad(float deg)
{
	return static_cast<float>(deg * (M_PI / 180));
}
