#include "Hammer.h"
#include <cassert>
#include "MyMath.h"
#include "CollisionTypeIdDef.h"
#include "Enemy.h"
#include "imgui.h"

void Hammer::Initialize(Model* model)
{
	Collider::Initialize();

	assert(model);
	// 引数で受け取ったモデルを設定
	model_ = model;
	// ワールド行列を初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f; // 体の中央を基準に設定

	// 種別IDを設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
}

void Hammer::Update()
{
	Collider::UpdateWorldTransform();

	// ワールド行列の更新
	worldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection)
{
	// モデルを描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Hammer::GetCenterPosition() const
{
	// 見た目上の中心点オフセット（モデル座標系）
	const Vector3 offset = { 0.0f, 8.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = MyMath::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Hammer::OnCollision(Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->getTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);
		// 敵の位置にエフェクトを発生
		enemy;
		ImGui::Begin("hit");
		ImGui::Text("hitting");
		ImGui::End();
	}
}
