#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// カメラのビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void SetViewProjection(const ViewProjection* viewProjection) { cameraViewProjection_ = viewProjection; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;
};

