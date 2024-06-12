#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "BaseCharacter.h"

class Player : public BaseCharacter
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
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	/// <summary>
	/// カメラのビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void SetViewProjection(const ViewProjection* viewProjection) { cameraViewProjection_ = viewProjection; }

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕振りギミック初期化
	/// </summary>
	void InitializeArmSwingGimmick();

	/// <summary>
	/// 腕振りギミック更新
	/// </summary>
	void UpdateArmSwingGimmick();

private:
	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	// モデル番号
	const uint32_t kModelIndexBody = 0;
	const uint32_t kModelIndexHead = 1;
	const uint32_t kModelIndexL_arm = 2;
	const uint32_t kModelIndexR_arm = 3;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 腕振りギミックの媒介変数
	float armSwingParameter_ = 0.0f;

	// 浮遊移動のサイクル<frame>
	uint16_t period_;
	// 浮遊の振幅
	float floatingAmplitude_;
};

