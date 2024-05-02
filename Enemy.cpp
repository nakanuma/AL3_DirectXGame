#include "Enemy.h"
#include "TextureManager.h"
#include "imgui.h"
#include <cassert>

#include "LoadFile.h"

Enemy::~Enemy() {
	// bullets_の開放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	// timedCall_の開放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
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

	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	// リストの全要素の更新を呼ぶ
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	// 行列の更新
	worldTransform_.UpdateMatrix();

	//// デバッグ用
	// ImGui::Begin("Enemy");
	// ImGui::DragFloat3("approachSpeed", &approachSpeed_.x);
	// ImGui::DragFloat3("leaveSpeed", &leaveSpeed_.x);
	// ImGui::End();
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

	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
		// 登録済みの時限発動イベントリストをクリア
		timedCalls_.clear();
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
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, position, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::InitializeApproach() {
	// 発射タイマーをセットする
	FireAndTimerReset();
}

void Enemy::FireAndTimerReset() {
	// 弾を発射する
	Fire();

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireAndTimerReset, this), kFireInterval));
}
