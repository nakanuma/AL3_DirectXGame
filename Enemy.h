#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 生成時に座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position) { worldTransformBody_.translation_ = position; }

private:
	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_spear_;
	WorldTransform worldTransformR_spear_;

	// モデル
	Model* modelBody_ = nullptr;
	Model* modelL_spear_ = nullptr;
	Model* modelR_spear_ = nullptr;

	// モデル番号
	const uint32_t kModelIndexBody = 0;
	const uint32_t kModelIndexL_spear = 1;
	const uint32_t kModelIndexR_spear = 2;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 速度
	const float kSpeed = 0.3f;
};

