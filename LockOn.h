#pragma once
#include "Sprite.h"

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;
};
