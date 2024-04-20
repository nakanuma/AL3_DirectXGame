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
};
