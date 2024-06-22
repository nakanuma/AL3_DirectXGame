#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	// 加算
	Matrix4x4 operator+(const Matrix4x4& rhs) const {
		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m[i][j] + rhs.m[i][j];
			}
		}
		return result;
	}

	// 減算
	Matrix4x4 operator-(const Matrix4x4& rhs) const {
		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m[i][j] - rhs.m[i][j];
			}
		}
		return result;
	}

	// 行列乗算
	Matrix4x4 operator*(const Matrix4x4& rhs) const {
		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m[i][k] * rhs.m[k][j];
				}
			}
		}
		return result;
	}

	// スカラー乗算
	Matrix4x4 operator*(float scalar) const {
		Matrix4x4 result = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m[i][j] * scalar;
			}
		}
		return result;
	}
};
