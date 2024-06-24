#pragma once
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"

class Hammer final : public Collider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 回転角を取得
	/// </summary>
	const Vector3& GetRotation() const { return worldTransform_.rotation_; }

	/// <summary>
	/// 回転角を設定
	/// </summary>
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// 親を設定
	/// </summary>
	void SetParent(const WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};

