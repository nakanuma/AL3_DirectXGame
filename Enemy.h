#pragma once
#include <WorldTransform.h>
#include <Model.h>

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	///
	///	モデル情報
	/// 

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	///
	///	移動
	/// 
	
	// 歩行の早さ
	static inline const float kWalkSpeed = 0.05f;
	// 速度
	Vector3 velocity_ = {};

	///
	/// アニメーション
	/// 
	
	// 最初の角度
	static inline const float kWalkMotionAngleStart = -15.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 25.0f;
	// アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.0f;

	// 経過時間
	float walkTimer_ = 0.0f;
};
