#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleText
{
public:
	void Initialize(Model* model, ViewProjection* viewProjection);

	void Update();

	void Draw();

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_;

	// タイトルテキストの上下移動
	float floatingParameter_ = 0.0f;
	uint16_t period_;
	float floatingAmplitude_;

};

