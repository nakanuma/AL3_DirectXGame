#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

class MyMath {
public:
	///
	///	3次元ベクトル
	/// 
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// 長さ（ノルム）
	static float Length(const Vector3& v);
	// 正規化
	static Vector3 Normalize(const Vector3& v);

	///
	///	4x4行列
	///

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4 m2);

	// 単位行列を書き込む
	static Matrix4x4 Identity();

	static Matrix4x4 Inverse(Matrix4x4 matrix);

	static Matrix4x4 Scaling(Vector3 scale);

	static Matrix4x4 Translation(Vector3 translation);

	static Matrix4x4 RotationX(float rad);

	static Matrix4x4 RotationY(float rad);

	static Matrix4x4 RotationZ(float rad);

	static Matrix4x4 RotationXYZ(float rotateX, float rotateY, float rotateZ);

	// アフィン変換行列の作成
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
};