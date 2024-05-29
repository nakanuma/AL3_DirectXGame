#pragma once
#include <WorldTransform.h>
#include <Model.h>

// MyClass
#include "MyMath.h"

class Player; // プレイヤークラスの前方宣言

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// AABBを取得
	/// </summary>
	AABB GetAABB();

	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(const Player* player);

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

	///
	///	その他
	/// 
	
	// 当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
};
