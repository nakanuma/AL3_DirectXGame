#pragma once
#include <list>
#include "Collider.h"
#include "Model.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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

	/// <summary>
	/// ワールドトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	// コライダー
	std::list<Collider*> colliders_;

	// デバッグ表示用モデル
	std::unique_ptr<Model> model_;

	// コライダー表示
	bool visible_ = true;
};
