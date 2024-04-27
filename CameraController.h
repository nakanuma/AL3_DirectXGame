#pragma once
#include "ViewProjection.h"

// 前方宣言
class Player;

// 矩形
struct Rect {
	float left = 0.0f; // 左端
	float right = 1.0f; // 右端
	float bottom = 0.0f; // 下端
	float top = 1.0f; // 上橋
};

class CameraController {
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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 外部からポインタをセット
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }

	/// <summary>
	/// viewProjectionのTranslationを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTranslation() { return viewProjection_.translation_; }

	/// <summary>
	/// 移動可能範囲をセット
	/// </summary>
	/// <param name="area"></param>
	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	ViewProjection viewProjection_;

	// 追従対象のポインタ
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -30.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	Vector3 targetPosition_;
	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f; // 1フレームで目標に近づく割合
	// 速度掛け率
	static inline const float kVelocityBias = 40.0f;
	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect kMargin = {-15, 15, -10, 10};
};
