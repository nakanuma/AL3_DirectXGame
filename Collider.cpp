#include "Collider.h"

void Collider::Initialize() {
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Collider::SetRadius(float radius) {
	// 実際の半径を設定
	radius_ = radius;
	// 見た目上の半径を設定
	worldTransform_.scale_ = {radius, radius, radius};
}

void Collider::UpdateWorldTransform() {
	// ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCenterPosition();
	// ワールド行列を再計算して定数バッファに書き込む
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	// モデルの描画
	model->Draw(worldTransform_, viewProjection);
}
