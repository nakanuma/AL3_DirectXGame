#include "Player.h"
#include <cassert>
#include <numbers>

#include "Input.h"
#include "imgui.h"

// MyClass
#include "MyMath.h"

void Player::Initialize(const std::vector<Model*>& models)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// NULLポインタチェック
	/*assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);*/

	// 引数として受け取ったデータをメンバ変数に記録する
	/*modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;*/

	/*viewProjection_ = viewProjection;*/

	// ワールド変換の初期化
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = { -1.2f,1.8f,0.0f }; // 初期位置を変更
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = { 1.2f,1.8f,0.0f }; // 初期位置を変更

	// 全てのパーツ同士の親子関係を結ぶ（Bodyが親になるように設定）
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
	// 腕振りギミック初期化
	InitializeArmSwingGimmick();
}

void Player::Update()
{
	// 基底クラスの更新
	BaseCharacter::Update();

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

	// デバッグ用
	ImGui::Begin("Player");
	ImGui::Text("worldTransformBody : %.f, %.f, %.f", 
		worldTransformBody_.translation_.x, worldTransformBody_.translation_.y, worldTransformBody_.translation_.z
	);
	ImGui::Text("worldTransformHead : %.f, %.f, %.f",
		worldTransformHead_.translation_.x, worldTransformHead_.translation_.y, worldTransformHead_.translation_.z
	);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
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

	ImGui::Begin("Player");
	ImGui::DragFloat3("Head Translation", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragInt("Period", reinterpret_cast<int*>(&period_), 1);
	ImGui::DragFloat("floatingAmplitude", &floatingAmplitude_, 0.1f);
	ImGui::End();
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
