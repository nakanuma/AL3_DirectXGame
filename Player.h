#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

// 左右
enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
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
	/// WorldTransformを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// プレイヤーの情報
	// 速度
	static inline const float kAcceleration = 0.01f; // 加速度
	static inline const float kAttenuation = 0.05f; // 速度減衰率
	static inline const float kLimitRunSpeed = 0.5f; // 最大速度
	Vector3 velocity_ = {}; // 速度
	// 回転
	LRDirection lrDirection_ = LRDirection::kRight; // 向き
	float turnFirstRotationY_ = 0.0f; // 旋回開始時の角度
	float turnTimer_ = 0.0f; // 旋回タイマー
	static inline const float kTimeTurn = 0.3f; // 旋回時間<秒>
	// ジャンプ
	bool onGround_ = true; // 接地状態フラグ
	static inline const float kGravityAcceleration = 0.05f; // 重力加速度（下方向）
	static inline const float kLimitFallSpeed = 1.0f; // 最大落下速度（下方向）
	static inline const float kJumpAcceleration = 1.0f; // ジャンプ初速（上方向）
	static inline const float kAttenuationLanding = 0.05f;
};
