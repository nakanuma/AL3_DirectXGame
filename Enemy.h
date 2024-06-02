#pragma once
#include "Model.h"
#include "WorldTransform.h"

// MyClass
#include "EnemyBullet.h"

// 前方宣言
class Player;
class GameScene;

// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_">モデル</param>
	/// <param name="position">初期化</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 敵が接近してくる際の更新処理
	/// </summary>
	void Approach();

	/// <summary>
	/// 敵が離脱する際の更新処理
	/// </summary>
	void Leave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void InitializeApproach();

	/// <summary>
	/// 外部からプレイヤーのアドレスを設定
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 弾リストを取得
	/// </summary>
	/*const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }*/

	/// <summary>
	/// 半径を取得
	/// </summary>
	float GetRadius() { return kRadius_; }

	/// <summary>
	/// ゲームシーンをセット
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// デスフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetDeathFlag() { return isDead_; }

private:
	///
	///	3Dモデル
	/// 

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデルのポインタ
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	///
	///	移動
	/// 

	// 初期フェーズ
	Phase phase_ = Phase::Approach;

	// 接近時の速度
	Vector3 approachSpeed_;
	// 離脱時の速度
	Vector3 leaveSpeed_;

	///
	///	弾
	/// 

	// 弾
	/*std::list<EnemyBullet*> bullets_;*/

	// 発射間隔
	static const int kFireInterval = 60;
	// 発射タイマー
	int32_t fireTimer_ = 0;

	///
	/// その他
	/// 
	
	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 半径
	const float kRadius_ = 1.0f;

	// デスフラグ
	bool isDead_ = false;
};
