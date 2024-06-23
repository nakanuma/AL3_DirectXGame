#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models)
{
	// 引数で受け取ったモデルをメンバ変数にコピー
	models_ = models;
	// ワールド変換データの初期化
	worldTransform_.Initialize();

	// コライダーの初期化
	Collider::Initialize();
}

void BaseCharacter::Update()
{
	// ワールド変換データの行列更新
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection)
{
	// モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

Vector3 BaseCharacter::GetCenterPosition() const { 
	// ワールド座標をいれる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
