#define NOMINMAX
#include "CameraController.h"

#include "Player.h"
#include "MyMath.h"

void CameraController::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + target_->GetVelocity() * kVelocityBias;

	// 座標補間によりゆったり追従
	viewProjection_.translation_ = MyMath::Lerp(viewProjection_.translation_, targetPosition_, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x + kMargin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.x + kMargin.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.y + kMargin.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.x, target_->GetWorldTransform().translation_.y + kMargin.top);

	// 移動範囲制限
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);

	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}