#include "Player.h"
#include <cassert>
#include <numbers>

#include "Input.h"
#include "imgui.h"

// MyClass
#include "MyMath.h"
#include "GlobalVariables.h"

void Player::Initialize(const std::vector<Model*>& models)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = { -1.2f,1.8f,0.0f }; // 初期位置を変更
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = { 1.2f,1.8f,0.0f }; // 初期位置を変更
	worldTransformHammer_.Initialize();
	worldTransformHammer_.translation_.y = 2.0f; // 体の中央を基準に設定

	// 全てのパーツ同士の親子関係を結ぶ（Bodyが親になるように設定）
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
	// 腕振りギミック初期化
	InitializeArmSwingGimmick();


	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "Test1", 90);
	globalVariables->SetValue(groupName, "Test2", 60.0f);
	globalVariables->SetValue(groupName, "Test3", Vector3{1.0f, 2.0f, 3.0f});
}

void Player::Update()
{
	// 基底クラスの更新
	BaseCharacter::Update();

	// Behavior遷移
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	// Behaviorの実行
	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		// 攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}


	// デバッグ用
	ImGui::Begin("Player");
	ImGui::Text("Right Stick : Camera Rotate");
	ImGui::Text("Left Stick : Move");
	ImGui::Text("A Button : Attack");
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	// 攻撃状態時のみハンマーを描画
	if (behavior_ == Behavior::kAttack) {
		models_[kModelIndexHammer]->Draw(worldTransformHammer_, viewProjection);
	}
}

void Player::InitializeFloatingGimmick()
{
	// 浮遊ギミックの媒介変数
	floatingParameter_ = 0.0f;
	// デフォルトのサイクル
	period_ = 60;
	// デフォルトの振幅
	floatingAmplitude_ = 0.5f;
}

void Player::UpdateFloatingGimmick()
{
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * std::numbers::pi_v<float> / period_;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * std::numbers::pi_v<float>);
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = 1.0f + std::sin(floatingParameter_) * floatingAmplitude_;
}

void Player::InitializeArmSwingGimmick()
{
	armSwingParameter_ = 0.0f;
}

void Player::UpdateArmSwingGimmick()
{
	// 腕振りのサイクル<frame>
	const uint16_t period = 60;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * std::numbers::pi_v<float> / period;
	// パラメータを1ステップ分加算
	armSwingParameter_ += step;
	// 2πを超えたら0に戻す
	armSwingParameter_ = std::fmod(armSwingParameter_, 2.0f * std::numbers::pi_v<float>);
	// 腕振りの振幅
	const float armSwingAmplitude_ = 0.5f;
	// 回転角に反映
	worldTransformL_arm_.rotation_.x = std::sin(armSwingParameter_) * armSwingAmplitude_;
	worldTransformR_arm_.rotation_.x = std::sin(armSwingParameter_) * armSwingAmplitude_;
}

void Player::BehaviorRootUpdate()
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
			move = MyMath::Normalize(MyMath::Multiply(speed, MyMath::Normalize(move)));

			// 移動ベクトルをカメラの角度だけ回転する
			move = MyMath::Transform(move, MyMath::RotationY(viewProjection_->rotation_.y));
		}

		// 移動方向と自キャラの向きを合わせる（Y軸周り角度）
		worldTransformBody_.rotation_.y = std::atan2(move.x, move.z);

		// 移動
		worldTransformBody_.translation_.x += move.x;
		worldTransformBody_.translation_.z += move.z;

		// ゲームパッドのAボタンが押された際に攻撃状態へ遷移
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	// 浮遊ギミック更新
	UpdateFloatingGimmick();
	// 腕振りギミック更新
	UpdateArmSwingGimmick();

	// 行列を更新
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Player::BehaviorAttackUpdate()
{
	// ハンマーが振り下ろされるまでの更新
	if (worldTransformHammer_.rotation_.x <= std::numbers::pi_v<float> / 2.0f) {
		// ハンマーをX軸周りに回転
		worldTransformHammer_.rotation_.x += 0.1f;
		// 両腕をX軸周りに回転
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;
	} else {
		// 硬直タイマーを増加
		postAttackTimer_++;
	}
	// 硬直時間終了後に通常状態に遷移させる
	if (postAttackTimer_ > kPostAttackCooldown) {
		behaviorRequest_ = Behavior::kRoot;
	}

	// 行列を更新
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::BehaviorRootInitialize()
{

}

void Player::BehaviorAttackInitialize()
{
	// 腕の初期回転角
	worldTransformL_arm_.rotation_.x = std::numbers::pi_v<float>;
	worldTransformR_arm_.rotation_.x = std::numbers::pi_v<float>;
	// ハンマーの初期回転角
	worldTransformHammer_.rotation_.x = 0.0f;
	// 攻撃後の硬直時間
	postAttackTimer_ = 0;
}
