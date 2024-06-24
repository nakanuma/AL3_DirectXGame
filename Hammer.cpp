#include "Hammer.h"
#include <cassert>
#include "MyMath.h"

void Hammer::Initialize(Model* model)
{
	assert(model);
	// 引数で受け取ったモデルを設定
	model_ = model;
	// ワールド行列を初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 2.0f; // 体の中央を基準に設定
}

void Hammer::Update()
{
	// ワールド行列の更新
	worldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection)
{
	// モデルを描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Hammer::GetCenterPosition() const
{
	// 見た目上の中心点オフセット（モデル座標系）
	const Vector3 offset = { 0.0f, 2.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = MyMath::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}
