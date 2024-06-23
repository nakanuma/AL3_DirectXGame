#pragma once
#include "Collider.h"
#include <list>

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 全ての当たり判定チェック
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// コライダーをリストに登録
	/// </summary>
	void AddCollider(Collider* collider);

private:
	// コライダー
	std::list<Collider*> colliders_;
};
