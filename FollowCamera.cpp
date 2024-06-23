#include "FollowCamera.h"
#include "Input.h"
#include "imgui.h"

// MyClass
#include "LockOn.h"
#include "MyMath.h"

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	///
	///	カメラ旋回
	///

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ロックオン中
	if (lockOn_ != nullptr && lockOn_->ExistTarget()) { // ロックオンのポインタにnullptr以外が入っているかつ、ロックオンが存在している
		// カメラをロックオン対象の方に向ける処理
		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 追従対象からロックオン座標へのベクトル
		Vector3 sub = lockOnPosition - target_->translation_;

		// Y軸周り角度
		viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);

		// スティック入力で角度を変更する処理
	} else {
		// ジョイスティックが有効な場合
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// 回転速度
			const float speed = 0.05f;
			// カメラ角度を変更
			viewProjection_.rotation_.y += static_cast<float>(joyState.Gamepad.sThumbRX) * speed / 32767.0f;
		}
	}

	///
	///	追従処理
	///

	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -25.0f};

		// カメラの回転行列を計算
		Matrix4x4 rotationMatrix = MyMath::RotationY(viewProjection_.rotation_.y);

		// オフセットをカメラの回転に合わせて回転させる
		offset = MyMath::TransformNormal(offset, rotationMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = 
			MyMath::Add({target_->translation_.x, 0.0f, target_->translation_.z}, {offset.x, offset.y + 2.0f, offset.z - 2.0f});
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
