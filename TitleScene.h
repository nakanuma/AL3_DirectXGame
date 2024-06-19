#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

// MyClass
#include "TitleText.h"
#include "Player.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene
{
public:
	TitleScene();
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン終了フラグのgetter
	/// </summary>
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	/// <summary>
	/// タイトルシーン用
	/// </summary>

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// タイトル文字
	TitleText* titleText_ = nullptr;
	// 3Dモデル（タイトル文字）
	Model* modelTitleText_ = nullptr;


	// プレイヤーのワールド変換データ
	WorldTransform worldTransformPlayer_;
	// 3Dモデル（プレイヤー）
	Model* modelPlayer_ = nullptr;

	// 終了フラグ
	bool finished_ = false;
};

