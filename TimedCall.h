#pragma once
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TimedCall(std::function<void()> func, uint32_t time);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 完了ならtrueを返す
	/// </summary>
	/// <returns>完了フラグ</returns>
	bool IsFinished() { return isFinish_; }

private:
	// コールバック
	std::function<void()> callback_;

	// 残り時間
	uint32_t time_;

	// 完了フラグ
	bool isFinish_ = false;
};
