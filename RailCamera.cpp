#include "RailCamera.h"
#include "imgui.h"

#include "MyMath.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation)
{
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;
	// ビュープロジェクションの初期化
	// ビュープロジェクションのfarZを適切な値にする
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	// ワールドトランスフォームの座標の数値を加算したりする（移動）
	worldTransform_.translation_.z += 0.05f;
	// ワールドトランスフォームの角度の数値を加算したりする（回転）
	/*worldTransform_.rotation_.y += 0.002f;*/
	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = MyMath::Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

