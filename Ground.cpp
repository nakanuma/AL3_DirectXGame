#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数で受け取ったデータをメンバ変数に記録
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 地面のサイズを変更
	worldTransform_.scale_ = { 500.0f, 500.0, 500.0f };
}

void Ground::Update()
{
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Ground::Draw()
{
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
