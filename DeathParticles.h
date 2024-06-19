#pragma once
#include <array>
#include <algorithm>
#include <numbers>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class DeathParticles
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了フラグのgetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

private:
	// モデル
	Model* model_;

	// ビュープロジェクション
	ViewProjection* viewprojection_;

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	// 存続時間（消滅までの時間）<秒>
	static inline const float kDuration = 2.0f;
	// 移動の速さ
	static inline const float kSpeed = 0.2f;
	// 分割した1個分の角度
	static inline const float kAngleUnit = std::numbers::pi_v<float> *2.0f / kNumParticles;

	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;

	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
};

