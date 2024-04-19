#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数で受け取ったデータをメンバ関数に記録
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update()
{
}

void Skydome::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_);
}
