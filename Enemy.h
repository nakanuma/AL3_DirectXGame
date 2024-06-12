#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
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

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
};

