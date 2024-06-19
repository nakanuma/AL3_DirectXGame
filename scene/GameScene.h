#pragma once
#include <vector>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

// MyClass
#include "MyMath.h"
#include "Skydome.h"
#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "DeathParticles.h"

// ゲームのフェーズ（型）
enum class Phase {
	kPlay, // ゲームプレイ
	kDeath, // デス演出
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// ゲームプレイフェーズの処理
	/// </summary>
	void PlayPhaseUpdate();

	/// <summary>
	/// デス演出フェーズの処理
	/// </summary>
	void DeathPhaseUpdate();

	/// <summary>
	/// フェーズの切り替え
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// シーン終了フラグのgetter
	/// </summary>
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 3Dモデルデータ
	Model* modelBlock_ = nullptr;

	// ブロック用のワールドトランスフォーム
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// 3Dモデル（天球）
	Model* modelSkydome_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 3Dモデル（自キャラ）
	Model* modelPlayer_ = nullptr;

	// 敵キャラ
	std::list<Enemy*> enemies_;
	const int32_t kEnemyNum_ = 5;

	// 3Dモデル（敵キャラ）
	Model* modelEnemy_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	// カメラの移動可能範囲
	Rect movableArea_ = {22, 1000, 12, 12};

	// 死亡時パーティクル
	DeathParticles* deathParticles_ = nullptr;
	// 死亡時パーティクルモデル
	Model* modelDeathParticle_ = nullptr;

	// ゲームの現在フェーズ
	Phase phase_;

	// 終了フラグ
	bool finished_ = false;
};
