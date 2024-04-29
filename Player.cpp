#define NOMINMAX
#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>

#include <Input.h>
#include <imgui.h>
#include <DebugText.h>

#include "MyMath.h"
#include "MapChipField.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標を代入
	worldTransform_.translation_ = position;
	// 初期回転角を指定
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
	worldTransform_.scale_ = {0.8f, 0.8f, 0.8f};
}

void Player::Update() {
	// 移動入力
	MoveInput();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.moveAmount = velocity_;

	// マップ衝突チェック
	MapCollisionDetection(collisionMapInfo);

	// 判定結果を反映して移動させる
	moveBasedOnJudgment(collisionMapInfo);

	// 天井に接触している場合の処理（デバッグ用）
	CollisionCeiling(collisionMapInfo);

	// 接地状態の切り替え
	CollisionGround(collisionMapInfo);

	// 旋回制御
	TurningControl();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// デバッグ用
	ImGui::Begin("Window");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::DragFloat3("velocity", &velocity_.x);
	ImGui::DragFloat3("moveAmount", &collisionMapInfo.moveAmount.x);
	if (onGround_) {
		ImGui::Text("onGround");
	}
	if (collisionMapInfo.landing) {
		ImGui::Text("landing");
	}
	ImGui::End();
}

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

void Player::MoveInput() {
	// 移動入力
	// 接地状態
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};

			// 右入力時
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				acceleration.x += kAcceleration;
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				// 右向きにする
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
				// 左入力時
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleration;
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

				// 左向きにする
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
				}
			}
			// 加速 / 減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		// ジャンプ入力
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

		
		// 空中
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::TurningControl() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= (1.0f / 60.0f);                         // 旋回タイマーのカウントダウン
		float turnProgress = 1.0f - (turnTimer_ / kTimeTurn); // 旋回タイマーを0から1の範囲で変化させる

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * MyMath::EaseInOutQuad(turnProgress);
	}
}

void Player::MapCollisionDetection(CollisionMapInfo& info) {
	MapCollisionDetectionUp(info);
	MapCollisionDetectionDown(info);
	/*MapCollisionDetectionRight(info);
	MapCollisionDetectionLeft(info);*/
}

void Player::MapCollisionDetectionUp(CollisionMapInfo& info) {
	// プレイヤーが上昇していない場合スキップ
	if (info.moveAmount.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標を計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒットした場合の処理
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.moveAmount + Vector3{0.0f, kHeight / 2.0f, 0.0f});
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// Y移動量を求める
		float actualVelocityY = rect.bottom - kBlank - (kHeight / 2.0f) - worldTransform_.translation_.y;
		info.moveAmount.y = std::max(0.0f, actualVelocityY);
		// 天井に当たったことを記録する
		info.isCeilingCollision = true;
	}
}

void Player::MapCollisionDetectionDown(CollisionMapInfo& info) {
	// プレイヤーが下降していない場合スキップ
	if (info.moveAmount.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標を計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒットした場合の処理
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.moveAmount - Vector3{0.0f, kHeight / 2.0f, 0.0f});
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		//// Y移動量を求める
		float actualVelocityY = rect.top + kBlank + (kHeight / 2.0f) - worldTransform_.translation_.y;
		info.moveAmount.y = std::min(0.0f, actualVelocityY);
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

//void Player::MapCollisionDetectionRight(CollisionMapInfo& info) {}
//
//void Player::MapCollisionDetectionLeft(CollisionMapInfo& info) {}

void Player::CollisionCeiling(const CollisionMapInfo& info) {
	// 天井に当たっている場合、ログを出力してY方向の速度を0に設定
	if (info.isCeilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::CollisionGround(const CollisionMapInfo& info) {
	// 自キャラが接地状態の場合
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		// 着地フラグ
		if (info.landing) {
			// 着地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}

	/*自キャラの下側がブロックに接触しているかを判定*/
	// 移動後の4つの角の座標を計算
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0.0f, -kBlank, 0.0f));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0.0f, -kBlank, 0.0f));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 落下開始
	if (!hit) {
		// 空中状態に切り替える
		onGround_ = false;
	}
}

void Player::moveBasedOnJudgment(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.moveAmount;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
