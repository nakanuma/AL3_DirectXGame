#include "TitleScene.h"
#include <cassert>
#include <numbers>

#include "MyMath.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	// タイトル文字の開放
	delete titleText_;
	// 3Dモデル（タイトル文字）の開放
	delete modelTitleText_;

	// 3Dモデル（プレイヤー）の開放
	delete modelPlayer_;
}

void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 3Dモデル（タイトル文字）の生成
	modelTitleText_ = Model::CreateFromOBJ("titleText", true);

	// タイトル文字の生成
	titleText_ = new TitleText();
	// タイトル文字の初期化
	titleText_->Initialize(modelTitleText_, &viewProjection_);


	// 3Dモデル（プレイヤー）の生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	// プレイヤーのワールド変換データの初期化
	worldTransformPlayer_.Initialize();
	// プレイヤーの初期位置を変更
	worldTransformPlayer_.translation_ = { 0.0f, -6.0f, 0.0f };
	// プレイヤーの初期サイズを変更
	worldTransformPlayer_.scale_ = { 3.0f, 3.0f, 3.0f };
}

void TitleScene::Update()
{
	// タイトル文字の更新
	titleText_->Update();
	// プレイヤーを回転
	worldTransformPlayer_.rotation_.y -= 0.1f;
	// プレイヤーの行列を更新
	worldTransformPlayer_.UpdateMatrix();

	// タイトルシーンの終了
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// タイトル文字の描画
	titleText_->Draw();

	// プレイヤーの描画
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
