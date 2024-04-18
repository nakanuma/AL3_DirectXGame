#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

class MyMath {
public:
	///
	///	4x4行列
	///

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4 m2);

	// 単位行列を書き込む
	Matrix4x4 Identity();

	Matrix4x4 Scaling(Vector3 scale);

	Matrix4x4 Translation(Vector3 translation);

	Matrix4x4 RotationX(float rad);

	Matrix4x4 RotationY(float rad);

	Matrix4x4 RotationZ(float rad);

	Matrix4x4 RotationXYZ(float rotateX, float rotateY, float rotateZ);

	// アフィン変換行列の作成
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
};