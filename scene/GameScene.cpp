#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "imgui.h"

// MyClass
#include "MyMath.h"

GameScene::GameScene() {} 

GameScene::~GameScene() {
	// 3Dモデルデータの開放
	delete modelBlock_;

	// ワールドトランスフォーム（ブロック用）のポインタのメモリを開放し、コンテナをクリア
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// デバッグカメラの開放
	delete debugCamera_;

	// 天球の開放
	delete skydome_;
	// 3Dモデル（天球）の開放
	delete modelSkydome_;

	// 自キャラの開放
	delete player_;
	// 3Dモデル（自キャラ）の開放
	delete modelPlayer_;

	// 敵キャラの開放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// 3Dモデル（敵キャラ）の開放
	delete modelEnemy_;

	// マップチップフィールドの開放
	delete mapChipField_;

	// カメラコントローラの開放
	delete cameraController_;

	
	// 死亡時パーティクルの開放
	if (deathParticles_) {
		delete deathParticles_;
	}
	// 3Dモデル（死亡時パーティクル）の開放
	delete modelDeathParticle_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルデータの生成
	modelBlock_ = Model::Create();


	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.translation_ = {4.0f, 3.0f, -30.0f}; // カメラの初期座標を変更


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);


	// 3Dモデル（天球）の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);


	// マップチップフィールドの生成とファイル読み込み
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 表示ブロックの生成
	GenerateBlocks();


	// 3Dモデル（自キャラ）の生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	// 自キャラの生成
	player_ = new Player();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(4, 16);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	// 自キャラのマップチップフィールド情報をセット
	player_->SetMapChipField(mapChipField_);

	// 3Dモデル（敵キャラ）の生成
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	// 敵キャラの生成
	for (int32_t i = 0; i < kEnemyNum_; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(16 + (i * 4), 18); // 一体ずつ異なる座標をセット
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// 3Dモデル（死亡時パーティクル）の生成
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	// 仮の生成処理
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, player_->GetWorldTransform().translation_);

	// カメラコントローラの初期化
	// 生成
	cameraController_ = new CameraController;
	// 初期化
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット（瞬間合わせ）
	cameraController_->Reset();
	// 移動範囲をセット
	cameraController_->SetMovableArea(movableArea_);
}

void GameScene::Update() {
	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// nullptrだった場合にはスキップ
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}

	// 天球の更新
	skydome_->Update();

	// 自キャラの更新
	player_->Update();

	// 死亡時パーティクルの更新
	if (deathParticles_) {
		deathParticles_->Update();
	}

	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		if (enemy != nullptr) {
			enemy->Update();
		}
	}

	// 全ての当たり判定を行う
	CheckAllCollisions();

	// デバッグカメラの更新
	debugCamera_->Update();

	// カメラコントローラの更新
	viewProjection_.translation_ = cameraController_->GetTranslation();
	cameraController_->Update();

	// デバッグ用
	/*ImGui::Begin("Window");
	ImGui::DragFloat3("viewProjection", &viewProjection_.translation_.x);
	ImGui::End();*/

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		// デバッグカメラ有効フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// デバッグカメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
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

	// ブロックの描画処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// nullptrだった場合にはスキップ
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 天球の描画
	skydome_->Draw();

	// 自キャラの描画
	player_->Draw();

	// 死亡時パーティクルの描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 敵キャラの描画
	for (Enemy* enemy : enemies_) {
		if (enemy != nullptr) {
			enemy->Draw();
		}
	}

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

void GameScene::GenerateBlocks()
{
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions()
{
#pragma region 自キャラと敵キャラの当たり判定
	// 自キャラと敵キャラの当たり判定

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵キャラ全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (MyMath::IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			// 敵キャラの衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}

#pragma endregion

#pragma region 自キャラとアイテムの当たり判定
	// 自キャラとアイテムの当たり判定

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自弾と敵キャラの当たり判定

#pragma endregion
}
