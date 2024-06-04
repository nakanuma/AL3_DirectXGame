#include "Player.h"
#include <cassert>
#include "Input.h"
#include "imgui.h"

// MyClass
#include "MyMath.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	///
	///	移動処理
	/// 

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
			static_cast<float>(joyState.Gamepad.sThumbLX), 
			0.0f, 
			static_cast<float>(joyState.Gamepad.sThumbLY)
		};

		// ゼロ除算や無限大への演算を避ける
		if (move.x != 0.0f || move.z != 0.0f) {
			// 移動量に速さを反映
			move = MyMath::Multiply(speed, MyMath::Normalize(move));

			// 移動
			worldTransform_.translation_.x += move.x;
			worldTransform_.translation_.z += move.z;
		}

		// 移動
		worldTransform_.translation_.x += move.x;
		worldTransform_.translation_.z += move.z;
	}

	// 行列を更新
	worldTransform_.UpdateMatrix();

	// デバッグ用
	ImGui::Begin("Player");
	ImGui::Text("%.f, %.f, %.f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
}

void Player::Draw()
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, nullptr);
}
