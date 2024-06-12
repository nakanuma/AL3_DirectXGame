#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models)
{
	// 引数で受け取ったモデルをメンバ変数にコピー
	models_ = models;
	// ワールド変換データの初期化
	worldTransform_.Initialize();
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
