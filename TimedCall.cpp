#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> func, uint32_t time) {
	// 引数で受けとった関数を設定
	callback_ = func;
	time_ = time;
}

void TimedCall::Update() { 
	// 完了なら
	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		//　コールバック呼び出し
		callback_();
	}
}
