#pragma once
#include <optional>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "BaseCharacter.h"

// 前方宣言
class LockOn;
class Hammer;

// 振る舞い
enum class Behavior {
	kRoot, // 通常状態
	kAttack, // 攻撃中
	kJump, // ジャンプ中
};

class Player : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// カメラのビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕振りギミック初期化
	/// </summary>
	void InitializeArmSwingGimmick();

	/// <summary>
	/// 腕振りギミック更新
	/// </summary>
	void UpdateArmSwingGimmick();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// ジャンプ行動更新
	/// </summary>
	void BehaviorJumpUpdate();

	/// <summary>
	/// 通常状態初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 攻撃時状態初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// ジャンプ行動初期化
	/// </summary>
	void BehaviorJumpInitialize();

	// 調整項目の適用
	void ApplyGlobalVariables();

	/// <summary>
	/// ロックオンのsetter
	/// </summary>
	void SetLockOn(LockOn* lockOn);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ハンマーを設定
	/// </summary>
	void SetHammer(Hammer* hammer) { hammer_ = hammer; }

private:
	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	/*WorldTransform worldTransformHammer_;*/

	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	/*Model* modelHammer_ = nullptr;*/

	// モデル番号
	const uint32_t kModelIndexBody = 0;
	const uint32_t kModelIndexHead = 1;
	const uint32_t kModelIndexL_arm = 2;
	const uint32_t kModelIndexR_arm = 3;
	const uint32_t kModelIndexHammer = 4;

	// ハンマー
	Hammer* hammer_ = nullptr;
	// ハンマーの回転角
	float rotateHammer_ = 0.0f;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 腕振りギミックの媒介変数
	float armSwingParameter_ = 0.0f;
	// 腕振りの最大角度
	float idleArmAngleMax_;

	// 浮遊移動のサイクル<frame>
	int32_t period_;
	// 浮遊の振幅
	float floatingAmplitude_;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	// 攻撃後の硬直時間をカウントする変数
	uint32_t postAttackTimer_ = 0;
	// 攻撃後の硬直時間
	const uint32_t kPostAttackCooldown = 10;

	// 速度
	Vector3 velocity_ = {};

	// 攻撃時の前方への速度
	float moveSpeed_ = 1.0f;

	// ロックオン
	const LockOn* lockOn_ = nullptr;
};

