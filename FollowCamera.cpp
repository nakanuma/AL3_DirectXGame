#include "FollowCamera.h"
#include "Input.h"
#include "imgui.h"

// MyClass
#include "MyMath.h"

void FollowCamera::Initialize()
{
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update()
{
	///
	///	追従処理
	/// 

	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 2.0f, -20.0f };

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotationMatrix = MyMath::RotationXYZ(
			viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z
		);

		// オフセットをカメラの回転に合わせて回転させる
		offset = MyMath::TransformNormal(offset, rotationMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = MyMath::Add(target_->translation_, offset);
	}

	///
	///	カメラ旋回
	/// 
	
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ジョイスティックが有効な場合
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 回転速度
		const float speed = 0.000002f;

		// カメラ角度を変更
		viewProjection_.rotation_.y += static_cast<float>(joyState.Gamepad.sThumbRX) * speed;
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();

	// デバッグ表示
	ImGui::Begin("FollowCamera");

	ImGui::Text("rotation : %f, %f, %f", viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z);
	ImGui::Text("translation : %f, %f, %f", viewProjection_.translation_.x, viewProjection_.translation_.y, viewProjection_.translation_.z);

	ImGui::End();
}
