#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw()
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, nullptr);
}
