#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルデータの生成
	model_.reset(Model::Create());

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// 自キャラの3Dモデルの生成
	modelPlayer_.reset(Model::CreateFromOBJ("player", true));
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_.get(), &viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferWidth());

	// 天球の3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get(), &viewProjection_);

	// 地面の3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化
	ground_->Initialize(modelGround_.get(), &viewProjection_);

	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 自キャラに追従カメラのビュープロジェクションをアドレス渡し
	player_->SetViewProjection(&followCamera_.get()->GetViewProjection());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// デバッグカメラの更新
	debugCamera_->Update();

	// 追従カメラの更新
	followCamera_->Update();
	// 追従カメラの行列をゲームシーンのビュープロジェクションにコピー
	viewProjection_.matView = followCamera_.get()->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_.get()->GetViewProjection().matProjection;

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		// デバッグカメラ有効フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	// デバッグカメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
#endif
}

void GameScene::Draw() {

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

	// 自キャラの描画
	player_->Draw();

	// 天球の描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

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
