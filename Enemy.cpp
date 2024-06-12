#include "Enemy.h"
#include <numbers>

void Enemy::Initialize(const std::vector<Model*>& models)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.translation_.z = 8.0f;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
}

void Enemy::Update()
{
	// 基底クラスの更新
	BaseCharacter::Update();

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// 3Dモデルを描画
	models_[0]->Draw(worldTransform_, viewProjection);
}
