#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	virtual ~Collider() = default;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision() {}

	/// <summary>
	/// 半径を取得
	/// </summary>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 半径を設定
	/// </summary>
	void SetRadius(float radius);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	virtual Vector3 GetCenterPosition() const = 0;

	/// <summary>
	/// ワールドトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Model* model, const ViewProjection& viewProjection);

private:
	// 衝突半径
	float radius_ = 1.5f;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
};
