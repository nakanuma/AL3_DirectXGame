#include "MyMath.h"
#include <math.h>
#include <cmath>
#include <algorithm>

float MyMath::Lerp(const float& a, const float& b, float t) {
	// aからbまでの差を計算し、tを乗じて補間
	return a + (b - a) * t;
}

Vector3 MyMath::TransformNormal(const Vector3& v, const Matrix4x4& m) { 
	return Vector3{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], 
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], 
		v.x * m.m[0][2] + v.y + m.m[1][2] + v.z * m.m[2][2]
	};
}

float MyMath::Length(const Vector3& v) { 
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2)); 
}

Vector3 MyMath::Normalize(const Vector3& v) { 
	float norm = Length(v);

	return Vector3(
		v.x / norm,
		v.y / norm,
		v.z / norm
	);
}

float MyMath::Dot(const Vector3& v1, const Vector3& v2) { 
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

Vector3 MyMath::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	// ベクトルの差を計算
	Vector3 diff = v2 - v1;
	// tを乗じて補間を行う
	diff *= t;
	// ベクトルv1に結果を加えて補間されたベクトルを計算
	return v1 + diff;
}

Vector3 MyMath::Slerp(const Vector3& v1, const Vector3& v2, float t) {
	// それぞれのベクトルを正規化
	Vector3 normalizedV1 = Normalize(v1);
	Vector3 normalizedV2 = Normalize(v2);

	// 内積を求める
	float dot = Dot(normalizedV1, normalizedV2);

	// 誤差により1.0fを超えるのを防ぐ
	dot = std::clamp(dot, -1.0f, 1.0f);

	// θの角度を求める
	float theta = std::acos(dot);
	// サインθを求める
	float sinTheta = std::sin(theta);

	// サイン(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	// サインθtを求める
	float sinThetaTo = std::sin(t * theta);

	// 球面線形補間したベクトル（単位ベクトル）
	Vector3 slerpVector;
	// ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5) {
		slerpVector = normalizedV1;
	} else {
		// 球面線形補間したベクトル（単位ベクトル）
		slerpVector = (normalizedV1 * sinThetaFrom + normalizedV2 * sinThetaTo) / sinTheta;
	}

	// ベクトルの長さはv1とv2の長さを線形補間
	float length1 = Length(v1);
	float length2 = Length(v2);
	// Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);

	// 長さを反映
	return slerpVector * length;
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
