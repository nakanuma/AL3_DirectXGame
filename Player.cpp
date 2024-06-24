#include "Player.h"
#include <cassert>
#include <numbers>

#include "Input.h"
#include "imgui.h"

// MyClass
#include "MyMath.h"
#include "GlobalVariables.h"
#include "LockOn.h"
#include "Hammer.h"

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
	//worldTransformHammer_.Initialize();
	//worldTransformHammer_.translation_.y = 2.0f; // 体の中央を基準に設定

	// 全てのパーツ同士の親子関係を結ぶ（Bodyが親になるように設定）
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	/*worldTransformHammer_.parent_ = &worldTransformBody_;*/

	// 浮遊ギミック初期化
	InitializeFloatingGimmick(); 
	// 腕振りギミック初期化
	InitializeArmSwingGimmick();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", period_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
	globalVariables->AddItem(groupName, "idleArmAngleMax", idleArmAngleMax_);
}

void Player::Update()
{
	// 基底クラスの更新
	BaseCharacter::Update();

	// 調整項目の適用
	ApplyGlobalVariables();

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
		case Behavior::kJump:
			BehaviorJumpInitialize();
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
		// ジャンプ行動
	case Behavior::kJump:
		BehaviorJumpUpdate();
	}

	// デバッグ用
	ImGui::Begin("Player");
	ImGui::Text("Right Stick : Camera Rotate");
	ImGui::Text("Left Stick : Move");
	ImGui::Text("A Button : Attack");
	ImGui::Text("B Button : Jump");
	ImGui::Text("Left Trigger : LockOn");
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
		hammer_->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransformBody_.matWorld_.m[3][0];
	worldPos.y = worldTransformBody_.matWorld_.m[3][1];
	worldPos.z = worldTransformBody_.matWorld_.m[3][2];

	return worldPos;
}

void Player::InitializeFloatingGimmick() {
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
	idleArmAngleMax_ = std::numbers::pi_v<float> / 2.0f; // π/2を最大角度として設定
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
	worldTransformL_arm_.rotation_.x = std::sin(armSwingParameter_) * armSwingAmplitude_ * idleArmAngleMax_;
	worldTransformR_arm_.rotation_.x = std::sin(armSwingParameter_) * armSwingAmplitude_ * idleArmAngleMax_;
}

void Player::BehaviorRootUpdate()
{
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	///
	///	移動処理
	/// 

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// ゲームパッドのAボタンが押された際に攻撃状態へ遷移
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kAttack;
			// 攻撃中にする
			isAttacking_ = true;
		}
		// ゲームパッドのBボタンが押された際にジャンプ行動へ遷移
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kJump;
		}

		float lx = static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX;
		float ly = static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX;

		// デッドゾーンの設定
		const float deadZone = 0.2f;
		if (std::abs(lx) < deadZone)
			lx = 0.0f;
		if (std::abs(ly) < deadZone)
			ly = 0.0f;

		// スティックによる移動入力がある場合
		if (lx != 0.0f || ly != 0.0f) {
			// 速さ
			const float speed = 0.3f;
			// 速度を更新
			velocity_ = {static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0.0f, static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX};

			// ゼロ除算や無限大への演算を避ける
			if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
				// 移動量に速さを反映
				velocity_ = MyMath::Normalize(MyMath::Multiply(speed, MyMath::Normalize(velocity_)));

				// 移動ベクトルをカメラの角度だけ回転する
				velocity_ = MyMath::Transform(velocity_, MyMath::RotationY(viewProjection_->rotation_.y));
			}

			// 移動方向と自キャラの向きを合わせる（Y軸周り角度）
			worldTransformBody_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

			// 移動
			worldTransformBody_.translation_.x += velocity_.x;
			worldTransformBody_.translation_.z += velocity_.z;


			// 移動入力が無い場合、自キャラをロックオン対象の方向に向ける処理
		} else if (lockOn_ != nullptr && lockOn_->ExistTarget()) {
			// ロックオン対象
			Vector3 lockOnPosition = lockOn_->GetTargetPosition();
			// 追従対象からロックオン対象へのベクトル
			Vector3 sub = lockOnPosition - worldTransformBody_.translation_;

			// Y軸周り角度
			worldTransformBody_.rotation_.y = std::atan2(sub.x, sub.z);
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
	// ロックオン中
	if (lockOn_ != nullptr && lockOn_->ExistTarget()) {
		// 自キャラをロックオン対象の方向に向ける処理
		// ロックオン対象
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - worldTransformBody_.translation_;

		//距離
		float distance = MyMath::Length(sub);
		// 距離しきい値
		const float threshold = 0.2f;

		// しきい値より離れているときのみ
		if (distance > threshold) {
			// Y軸周り角度
			worldTransformBody_.rotation_.y = std::atan2(sub.x, sub.z);

			// しきい値を超える速さなら補正する
			if (moveSpeed_ > distance - threshold) {
				// ロックオン対象へのめりこみ防止
				moveSpeed_ = distance - threshold;
			}
		}
	}

	// ハンマーが振り下ろされるまでの更新
	if (hammer_->GetRotation().x <= std::numbers::pi_v<float> / 2.0f) {
		// ハンマーをX軸周りに回転
		/*worldTransformHammer_.rotation_.x += 0.1f;*/
		rotateHammer_ += 0.1f;
		hammer_->SetRotation({ rotateHammer_, 0.0f, 0.0f });
		// 両腕をX軸周りに回転
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;
		// プレイヤーを前方へ移動させる処理
		float moveDirection = worldTransformBody_.rotation_.y;
		float moveDistance = 1.0f;
		Vector3 moveVector = {moveDistance * std::sin(moveDirection), 0.0f, moveDistance * std::cos(moveDirection)};

		worldTransformBody_.translation_ += moveVector * moveSpeed_;
	} else {
		// 硬直タイマーを増加
		postAttackTimer_++;
	}
	// 硬直時間終了後に通常状態に遷移させる
	if (postAttackTimer_ > kPostAttackCooldown) {
		behaviorRequest_ = Behavior::kRoot;
		// 攻撃終了
		isAttacking_ = false;
	}

	// 行列を更新
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	hammer_->Update();
}

void Player::BehaviorJumpUpdate() 
{
	// 移動
	worldTransformBody_.translation_ += velocity_;
	// 重力加速度
	const float kGravityAcceleration = 0.05f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0.0f, -kGravityAcceleration, 0.0f};
	// 加速する
	velocity_ += accelerationVector;

	// 着地
	if (worldTransformBody_.translation_.y <= 0.0f) {
		worldTransformBody_.translation_.y = 0.0f;
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}

	// 行列を更新
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	hammer_->Update();
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
	hammer_->SetRotation({ 0.0f, 0.0f, 0.0f });
	rotateHammer_ = 0.0f;
	// 攻撃後の硬直時間
	postAttackTimer_ = 0;
}

void Player::BehaviorJumpInitialize() 
{ 
	worldTransformBody_.translation_.y = 0.0f; 
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;

	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	// ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;
}

void Player::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
	period_ = globalVariables->GetIntValue(groupName, "floatingCycle");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
	idleArmAngleMax_ = globalVariables->GetFloatValue(groupName, "idleArmAngleMax");
}

void Player::SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

Vector3 Player::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 2.0f, 0.0f}; // モデルの真ん中当たりを指定
	// ワールド座標に変換
	Vector3 worldPos = MyMath::Transform(offset, worldTransformBody_.matWorld_);
	return worldPos;
}

void Player::OnCollision([[maybe_unused]] Collider* other) {
	// ジャンプリクエスト
	/*behaviorRequest_ = Behavior::kJump;*/
}
