#include "TitleText.h"
#include <cassert>
#include <cmath>
#include <numbers>

void TitleText::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 初期サイズを変更
	worldTransform_.scale_ = { 10.0f, 10.0f, 10.0f };

	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
}

void TitleText::Update()
{
	// 浮遊ギミック更新
	UpdateFloatingGimmick();

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void TitleText::Draw()
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

void TitleText::InitializeFloatingGimmick()
{
	// 浮遊ギミックの媒介変数
	floatingParameter_ = 0.0f;
	// デフォルトのサイクル
	period_ = 60;
	// デフォルトの振幅
	floatingAmplitude_ = 0.5f;
}

void TitleText::UpdateFloatingGimmick()
{
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * std::numbers::pi_v<float> / period_;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * std::numbers::pi_v<float>);
	// 浮遊を座標に反映
	worldTransform_.translation_.y = 6.0f + std::sin(floatingParameter_) * floatingAmplitude_;
}
