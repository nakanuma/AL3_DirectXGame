#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

// MyClass
#include "GlobalVariables.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// レティクルのテクスチャ
	TextureManager::Load("reticle.png");

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルデータの生成
	model_.reset(Model::Create());

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// 自キャラの3Dモデルの生成
	/*modelPlayer_.reset(Model::CreateFromOBJ("player", true));*/

	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelFighterHammer_.reset(Model::CreateFromOBJ("hammer", true));

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	// 自キャラモデル
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(), modelFighterR_arm_.get(), modelFighterHammer_.get()
	};

	// 自キャラの初期化
	player_->Initialize(playerModels);


	// 敵キャラの3Dモデルの生成
	modelEnemyBody_.reset(Model::CreateFromOBJ("enemy_body", true));
	modelEnemyL_spear_.reset(Model::CreateFromOBJ("enemy_L_spear", true));
	modekEnemyR_spear_.reset(Model::CreateFromOBJ("enemy_R_spear", true));
	std::vector<Model*> enemyModels = { 
		modelEnemyBody_.get(), modelEnemyL_spear_.get(), modekEnemyR_spear_.get()
	};
	// 敵キャラの生成
	for (uint32_t i = 0; i < kNumEnemies_; ++i) {
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize(enemyModels);
		enemy->SetPosition({1.0f + (i * 6.0f), 1.0f, 8.0f});
		enemies_.push_back(std::move(enemy));
	}


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
	followCamera_->SetTarget(&player_->GetWorldTransformBody());

	// 自キャラに追従カメラのビュープロジェクションをアドレス渡し
	player_->SetViewProjection(&followCamera_.get()->GetViewProjection());


	// ロックオンの生成とポインタの代入
	lockOn_ = std::make_unique<LockOn>();
	// ロックオンの初期化
	lockOn_->Initialize();

	// 追従カメラにロックオンのポインタを設定
	followCamera_->SetLockOn(lockOn_.get());
	// 自キャラにロックオンのポインタを設定
	player_->SetLockOn(lockOn_.get());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	for (const auto& enemy : enemies_) {
		enemy->Update();
	}

	// ロックオン更新
	lockOn_->Update(enemies_, viewProjection_);

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
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	for (const auto& enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

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

	// ロックオンの描画
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
