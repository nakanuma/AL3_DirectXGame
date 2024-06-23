#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

// 前方宣言
class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 追従対象のワールドトランスフォームをセット
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ロックオンのsetter
	/// </summary>
	void SetLockOn(LockOn* lockOn);

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	const WorldTransform* target_ = nullptr;

	// ロックオン
	const LockOn* lockOn_ = nullptr;
};

