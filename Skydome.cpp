#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	// 引数で受け取ったデータをメンバ変数に記録
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 天球のサイズを変更
	worldTransform_.scale_ = {500.0f, 500.0f, 500.0f};
}

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
