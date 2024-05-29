#include "RailCamera.h"
#include "imgui.h"

#include "MyMath.h"

void RailCamera::Initialize(Vector3 WorldPos, Vector3 radian)
{
	worldTransform_.Initialize();
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = WorldPos;
	worldTransform_.rotation_ = radian;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	// ワールドトランスフォームの座標の数値を加算
	worldTransform_.translation_.z -= 0.1f;
	// ワールドトランスフォームの角度の数値を加算
	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = MyMath::Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat4("viewProjection", &viewProjection_.matView.m[0][0], 0.01f);
	ImGui::DragFloat4("viewProjection", &viewProjection_.matView.m[1][0], 0.01f);
	ImGui::DragFloat4("viewProjection", &viewProjection_.matView.m[2][0], 0.01f);
	ImGui::DragFloat4("viewProjection", &viewProjection_.matView.m[3][0], 0.01f);
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

