#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include "imgui.h"
#include <fstream>
#include <iostream>
#include <string>

GameScene::GameScene() {
}

GameScene::~GameScene() {
	// 3Dモデルデータの開放
	delete model_;

	// 自キャラの開放
	delete player_;

	// 敵キャラの開放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	// 敵弾の開放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	// 天球の開放
	delete skydome_;

	// 天球の3Dモデルの開放
	delete modelSkydome_;

	// デバッグカメラの開放
	delete debugCamera_;

	// レールカメラの開放
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// レティクルのテクスチャ
	TextureManager::Load("reticle.png");

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize(viewProjection_.translation_, viewProjection_.rotation_);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPositon = {0.0f, 0.0f, 30.0f};
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, playerPositon);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//// 敵キャラの生成
	//for (int32_t i = 0; i < kEnemyNum_; ++i) {
	//	Enemy* newEnemy = new Enemy();
	//	Vector3 enemyPosition = {16.0f + (i * 10.0f), 2.0f, 30.0f}; // 一旦適当な値を設定
	//	newEnemy->Initialize(model_, enemyPosition);
	//	newEnemy->SetPlayer(player_); // 敵キャラに自キャラのアドレスを渡す
	//	newEnemy->SetGameScene(this); // 敵キャラにゲームシーンを渡す

	//	enemys_.push_back(newEnemy);
	//}

	// 敵の情報をファイルから読み込む
	LoadEnemyPopData();

	// 天球の3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferWidth());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//// 敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	// レールカメラの更新
	railCamera_->Update();

	// 自キャラの更新
	player_->Update(viewProjection_);

	///
	///	↓ ここから敵の更新処理
	/// 

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		// ポインタが有効な場合のみ
		if (enemy) {
			enemy->Update();
		}
	}

	// デスフラグの立った敵をリストから削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->GetDeathFlag()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 敵弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// 削除フラグの立った敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	///
	///	↑ ここまで敵の更新処理
	/// 

	// 衝突判定
	CheckAllCollisions();

	// 天球の更新
	skydome_->Update();

	// デバッグカメラの更新
	debugCamera_->Update();

	// レールカメラのビュー行列とプロジェクション行列をコピー
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	//// デバッグ用表示
	//ImGui::Begin("debug");
	//ImGui::End();

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

	///
	///	↓ ここから敵の描画処理
	/// 
	
	// 敵キャラの描画
	for (Enemy* enemy : enemies_) {
		// ポインタが有効な場合のみ
		if (enemy) {
			enemy->Draw(viewProjection_);
		}
	}
	
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	///
	///	↑ ここまで敵の描画処理
	/// 

	// 天球の描画
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// プレイヤーのUIを描画
	player_->DrawUI();

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
	/*const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();*/

	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		posB = enemy->GetWorldPosition();

		// 自弾全てと敵キャラの当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posA = bullet->GetWorldPosition();

			// 座標AとBの距離の2乗を求める
			float distance = std::powf(posB.x - posA.x, 2) + std::powf(posB.y - posA.y, 2) + std::powf(posB.z - posA.z, 2);
			// 半径AとBの和の2乗を求める
			float combinedRadius = std::powf(enemy->GetRadius() + bullet->GetRadius(), 2);

			// 球と球の交差判定
			if (distance <= combinedRadius) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	#pragma endregion
	
	#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			// 待機完了
			isWaiting_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyPop(Vector3(x, y, z));

		// WAITコマンド
		} else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyPop(Vector3 position) {
	// 新しい敵を生成
	Enemy* newEnemy = new Enemy();
	// 指定された位置に敵を初期化
	newEnemy->Initialize(model_, position);
	// 自キャラのポインタを渡す
	newEnemy->SetPlayer(player_);
	// ゲームシーンのポインタを渡す
	newEnemy->SetGameScene(this);

	// 新しい敵を敵リストに追加
	enemies_.push_back(newEnemy);
}
