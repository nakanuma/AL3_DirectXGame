#include "EnemyBullet.h"
#include <cassert>
#include "TextureManager.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った値をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	// Z方向に伸びた形状にする
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	// Y軸周り角度（θy）
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	
	// X軸周り角度（θx）
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

}

void EnemyBullet::Update() {
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
