#include "Enemy.h"
#include "TextureManager.h"
#include "imgui.h"
#include <cassert>

#include "LoadFile.h"
#include "Player.h"

Enemy::~Enemy() {
	// bullets_の開放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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

	// 接近時の速度を設定
	approachSpeed_ = LoadFile::Vector3FromCSV("Resources/enemySpeed.csv", "Approach");
	// 離脱時の速度を設定
	leaveSpeed_ = LoadFile::Vector3FromCSV("Resources/enemySpeed.csv", "Leave");

	// 接近フェーズ初期化
	InitializeApproach();
}

void Enemy::Update() {
	///
	///	移動処理
	///
	switch (phase_) {
	case Phase::Approach:
	default:
		// 接近フェーズの更新
		Approach();
		break;
	case Phase::Leave:
		// 離脱フェーズの更新
		Leave();
		break;
	}

	///
	///	攻撃処理
	/// 

	// 弾の更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 行列の更新
	worldTransform_.UpdateMatrix();

	//// デバッグ用
	//ImGui::Begin("Enemy");
	//ImGui::DragFloat3("approachSpeed", &approachSpeed_.x);
	//ImGui::DragFloat3("leaveSpeed", &leaveSpeed_.x);
	//ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach() {
	///
	///	移動
	/// 
	
	worldTransform_.translation_ += approachSpeed_;

	///
	///	攻撃
	/// 

	// 発射タイマーをカウントダウン
	fireTimer_--;
	// 指定時間に達した
	if (fireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}

	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += leaveSpeed_;
}

void Enemy::Fire() {
	// 現在の座標をコピー
	Vector3 position = worldTransform_.translation_;

	// 弾の速度
	const float kBulletSpeed = 0.02f;

	// 敵キャラ->自キャラへの差分ベクトルを求める
	Vector3 direction = player_->GetWorldPosition() - GetWorldPosition();
	// ベクトルを正規化
	MyMath::Normalize(direction);
	// ベクトルの長さを、早さに合わせる
	Vector3 velocity = direction * kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, position, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::InitializeApproach() {
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos; 
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}
