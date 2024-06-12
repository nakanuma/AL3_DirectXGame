#pragma once
#include <vector>
#include <memory>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

// MyClass
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "enemy.h"

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

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデルデータ
	std::unique_ptr<Model> model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラの3Dモデル
	/*std::unique_ptr<Model> modelPlayer_ = nullptr;*/

	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;

	// 自キャラ
	std::unique_ptr<Player> player_ = nullptr;

	// 敵キャラ
	std::unique_ptr<Enemy> enemy_ = nullptr;
	// 敵キャラの3Dモデル
	std::unique_ptr<Model> modelEnemy_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// 天球の3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 地面の3Dモデル
	std::unique_ptr<Model> modelGround_ = nullptr;
	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
};
