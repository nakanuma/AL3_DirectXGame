#include "CollisionManager.h"
#include "MyMath.h"
#include "GlobalVariables.h"

void CollisionManager::Initialize() {
	model_.reset(Model::CreateFromOBJ("sphere", true));
	assert(model_);

	// ImGui表示
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Collision";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Visible", visible_);
}

void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) { 
	// コライダーAの座標を取得
	Vector3 positionA = colliderA->GetCenterPosition(); 
	// コライダーBの座標を取得
	Vector3 positionB = colliderB->GetCenterPosition();
	// 座標の差分ベクトル
	Vector3 subtract = positionB - positionA;
	// 座標AとBの距離を求める
	float distance = MyMath::Length(subtract);

	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision(colliderB);
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision(colliderA);
	}
}

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当り
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider) { 
	colliders_.push_back(collider); 
}

void CollisionManager::UpdateWorldTransform() {
	// グローバル変数からvisible_の最新値を取得
	visible_ = GlobalVariables::GetInstance()->GetBoolValue("Collision", "Visible");

	// 非表示なら抜ける
	if (!visible_) {
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {
		// 更新
		collider->UpdateWorldTransform();
	}
}

void CollisionManager::Draw(const ViewProjection& viewProjection) {
	// 非表示なら抜ける
	if (!visible_) {
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {
		// 描画
		collider->Draw(model_.get(), viewProjection);
	}
}
