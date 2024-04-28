#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField; // 前方宣言

// 左右
enum class LRDirection {
	kRight,
	kLeft,
};

// 角
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

// マップとの当たり判定情報
struct CollisionMapInfo {
	bool isCeilingCollision = false; // 天井衝突フラグ
	bool onGround = false; // 着地フラグ
	bool isWallCollision = false; // 壁接触フラグ
	Vector3 moveAmount; // 移動量
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 移動入力
	/// </summary>
	void MoveInput();

	/// <summary>
	/// WorldTransformを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }

	/// <summary>
	/// 外部からマップチップフィールドのポインタをセット
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionDetection(CollisionMapInfo& info);

	void MapCollisionDetectionUp(CollisionMapInfo& info); // 上方向
	void MapCollisionDetectionDown(CollisionMapInfo& info); // 下方向
	void MapCollisionDetectionRight(CollisionMapInfo& info); // 右方向
	void MapCollisionDetectionLeft(CollisionMapInfo& info); // 左方向

	// 天井に接触している場合の処理
	void CollisionCeiling(const CollisionMapInfo& info);

	// 判定結果を反映して移動させる
	void moveBasedOnJudgment(const CollisionMapInfo& info);

	/// <summary>
	/// 指定した角の座標を計算
	/// </summary>
	/// <param name="center"></param>
	/// <param name="corner"></param>
	/// <returns></returns>
	Vector3 CornerPosition(const Vector3& center, Corner corner);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// プレイヤーの情報
	// 速度
	static inline const float kAcceleration = 0.01f; // 加速度
	static inline const float kAttenuation = 0.05f; // 速度減衰率
	static inline const float kLimitRunSpeed = 0.5f; // 最大速度
	Vector3 velocity_ = {}; // 速度
	// 回転
	LRDirection lrDirection_ = LRDirection::kRight; // 向き
	float turnFirstRotationY_ = 0.0f; // 旋回開始時の角度
	float turnTimer_ = 0.0f; // 旋回タイマー
	static inline const float kTimeTurn = 0.3f; // 旋回時間<秒>
	// ジャンプ
	bool onGround_ = true; // 接地状態フラグ
	static inline const float kGravityAcceleration = 0.05f; // 重力加速度（下方向）
	static inline const float kLimitFallSpeed = 1.0f; // 最大落下速度（下方向）
	static inline const float kJumpAcceleration = 1.0f; // ジャンプ初速（上方向）
	static inline const float kAttenuationLanding = 0.05f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.0001f;
};
