#pragma once
#include <memory>
#include <numbers>

#include "Sprite.h"
#include "ViewProjection.h"
#include "Input.h"

#include "Enemy.h"

/// <summary>
/// ロックオン
/// </summary>
class LockOn {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies">敵リスト</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ロックオン対象の検索
	/// </summary>
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 範囲外判定
	/// </summary>
	/// <param name="viewProjection"></param>
	bool OutOfRange(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド座標->スクリーン座標
	/// </summary>
	/// <param name="worldPosition">ワールド座標</param>
	/// <returns>スクリーン座標</returns>
	Vector3 WorldToScreen(Vector3 worldPosition, const ViewProjection& viewProjection);

	/// <summary>
	/// ロックオン対象の座標取得
	/// </summary>
	/// <returns>ロックオン対象の中心座標</returns>
	Vector3 GetTargetPosition() const;

	/// <summary>
	/// ターゲットの存在を確認
	/// </summary>
	/// <returns>ターゲットが存在しているか</returns>
	bool ExistTarget() const { return target_ ? true : false; }

private:
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

	// ロックオン対象
	const Enemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 60.0f;
	// 角度範囲
	float angleRange_ = 20.0f * std::numbers::pi_v<float> / 180.0f; // radianに変換
};
