#include "Enemy.h"
#include <cassert>
#include "TextureManager.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	///
	///	移動処理
	/// 
	const float kEnemySpeed = -0.4f;
	Vector3 velocity(0, 0, kEnemySpeed);

	// 座標に速度を加算して移動
	worldTransform_.translation_ += velocity;

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
