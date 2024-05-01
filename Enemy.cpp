#include "Enemy.h"
#include "TextureManager.h"
#include "imgui.h"
#include <cassert>

#include "LoadFile.h"

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
}

void Enemy::Approach() {
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += approachSpeed_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += leaveSpeed_;
}
