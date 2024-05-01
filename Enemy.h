#pragma once
#include "Model.h"
#include "WorldTransform.h"

// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_">モデル</param>
	/// <param name="position">初期化</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 敵が接近してくる際の更新処理
	/// </summary>
	void Approach();

	/// <summary>
	/// 敵が離脱する際の更新処理
	/// </summary>
	void Leave();

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデルのポインタ
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 初期フェーズ
	Phase phase_ = Phase::Approach;

	// 接近時の速度
	Vector3 approachSpeed_;
	// 離脱時の速度
	Vector3 leaveSpeed_;
};
