#include "Enemy.h"
#include <numbers>

#include "MyMath.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransformBody_.Initialize();
	worldTransformBody_.rotation_.y = std::numbers::pi_v<float>;
	worldTransformL_spear_.Initialize();
	worldTransformL_spear_.translation_.x = -1.6f;
	worldTransformR_spear_.Initialize();
	worldTransformR_spear_.translation_.x = 1.6f;

	// 親子関係を結ぶ
	worldTransformL_spear_.parent_ = &worldTransformBody_;
	worldTransformR_spear_.parent_ = &worldTransformBody_;
}

void Enemy::Update()
{
	// 基底クラスの更新
	BaseCharacter::Update();

	// Y軸周りに回転
	worldTransformBody_.rotation_.y += 0.05f;
	// 向いている方向へ移動
	Vector3 velocity{ 0.0f, 0.0f, kSpeed };
	velocity = MyMath::TransformNormal(velocity, worldTransformBody_.matWorld_);
	worldTransformBody_.translation_ = MyMath::Add(worldTransformBody_.translation_, velocity);

	// 行列を更新
	worldTransformBody_.UpdateMatrix();
	worldTransformL_spear_.UpdateMatrix();
	worldTransformR_spear_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexL_spear]->Draw(worldTransformL_spear_, viewProjection);
	models_[kModelIndexR_spear]->Draw(worldTransformR_spear_, viewProjection);
}

Vector3 Enemy::GetCenterPosition() const{
	// 見た目上の中心点オフセット（モデル座標系）
	const Vector3 offset = {0.0f, 0.0f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = MyMath::Transform(offset, worldTransformBody_.matWorld_);
	return worldPos;
}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransformBody_.matWorld_.m[3][0];
	worldPos.y = worldTransformBody_.matWorld_.m[3][1];
	worldPos.z = worldTransformBody_.matWorld_.m[3][2];

	return worldPos;
}
