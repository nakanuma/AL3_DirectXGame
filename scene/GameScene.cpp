#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	// 3Dモデルデータの開放
	delete model_;

	// 自キャラの開放
	delete player_;

	// 敵キャラの開放
	delete enemy_;

	// デバッグカメラの開放
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵キャラの生成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, {16.0f, 2.0f, 30.0f}); // 一旦適当な値を設定

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferWidth());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新（ポインタが有効な場合のみ）
	if (enemy_) {
		enemy_->Update();
	}

	// 衝突判定
	CheckAllCollisions();

	// デバッグカメラの更新
	debugCamera_->Update();

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
		viewProjection_.UpdateMatrix();
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

	// 敵キャラの描画（ポインタが有効な場合のみ）
	if (enemy_) {
		enemy_->Draw(viewProjection_);
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

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離の2乗を求める
		float distance = std::powf(posB.x - posA.x, 2) + std::powf(posB.y - posA.y, 2) + std::powf(posB.z - posA.z, 2);
		// 半径AとBの和の2乗を求める
		float combinedRadius = std::powf(player_->GetRadius() + bullet->GetRadius(), 2);

		// 球と球の交差判定
		if (distance <= combinedRadius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion
	
	#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posB = enemy_->GetWorldPosition();

	// 自弾全てと敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posA = bullet->GetWorldPosition();

		// 座標AとBの距離の2乗を求める
		float distance = std::powf(posB.x - posA.x, 2) + std::powf(posB.y - posA.y, 2) + std::powf(posB.z - posA.z, 2);
		// 半径AとBの和の2乗を求める
		float combinedRadius = std::powf(enemy_->GetRadius() + bullet->GetRadius(), 2);

		// 球と球の交差判定
		if (distance <= combinedRadius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion
	
	#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離の2乗を求める
			float distance = std::powf(posB.x - posA.x, 2) + std::powf(posB.y - posA.y, 2) + std::powf(posB.z - posA.z, 2);
			// 半径AとBの和の2乗を求める
			float combinedRadius = std::powf(playerBullet->GetRadius() + enemyBullet->GetRadius(), 2);

			// 球と球の交差判定
			if (distance <= combinedRadius) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}
