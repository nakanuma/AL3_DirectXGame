#include "Enemy.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	// モデルの設定
	model_ = model;
	// ビュープロジェクションの設定
	viewProjection_ = viewProjection;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;
	// 初期回転角を指定
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 2.0f;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	// 経過時間の初期化
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	///
	/// 移動処理
	/// 
	worldTransform_.translation_ += velocity_;

	///
	///	アニメーション
	/// 
	
	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.z = radian * (std::numbers::pi_v<float> / 180.0f);

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	// モデルの描画
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標をいれる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB()
{
	// ワールド座標を取得
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };

	return aabb;
}

void Enemy::OnCollision(const Player* player)
{
	(void)player;
}
