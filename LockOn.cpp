#include "LockOn.h"
#include <cmath>
#include "TextureManager.h"
#include "imgui.h"
#include "WinApp.h"

#include "MyMath.h"

void LockOn::Initialize() {
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	// スプライトの生成処理
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_.reset(Sprite::Create(
		textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}
	));
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// 前回のトリガー状態を保持する変数
	static bool wasTriggerPressed = false;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		bool isTriggerPressed = joyState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

		// ロックオン状態なら
		if (target_) {
			// ロックオン解除処理
			// ロックオンボタンをトリガーしたら
			if (isTriggerPressed && !wasTriggerPressed) {
				// ロックオンを外す
				target_ = nullptr;

				// 範囲外判定
			} else if (OutOfRange(viewProjection)) {
				// ロックオンを外す
				target_ = nullptr;
			}
		} else {
			// ロックオンボタンをトリガーしたら
			if (isTriggerPressed && !wasTriggerPressed) {
				// ロックオン対象の検索
				Search(enemies, viewProjection);
				// 範囲外判定
			}
		}

		// ロックオン状態なら
		if (target_) {
			// ロックオンマークの座標計算
			// 敵のロックオン座標取得
			Vector3 positionWorld = target_->GetCenterPosition();
			// ワールド座標からスクリーン座標に変換
			Vector3 positionScreen = WorldToScreen(positionWorld, viewProjection);
			// Vector2に格納
			Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
			// スプライトの座標を設定
			lockOnMark_->SetPosition(positionScreenV2);
		}

		 // トリガー状態の更新
		wasTriggerPressed = isTriggerPressed;
	}
}

void LockOn::Draw() {
	// ロックオンマークを描画
	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetCenterPosition();

		// ワールド->ビュー座標変換
		Vector3 positionView = MyMath::Transform(positionWorld, viewProjection.matView);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			// カメラ前方との角度を計算
			float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

			// 角度条件チェック（コーンに収まっているか）
			if (fabs(arcTangent) <= angleRange_) {
				targets.emplace_back((std::make_pair(positionView.z, enemy.get())));
			}
		}
		// ロックオン対象をリセット
		target_ = nullptr;
		if (!targets.empty()) {
			// 距離で昇順にソート
			targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
			// ソートの結果一番近い敵をロックオン対象とする
			target_ = targets.front().second;
		}
	}
}

bool LockOn::OutOfRange(const ViewProjection& viewProjection) {
	if (!target_) {
		return true;
	}

	// 敵のロックオン座標取得
	Vector3 positionWorld = target_->GetCenterPosition();

	// ワールド->ビュー座標変換
	Vector3 positionView = MyMath::Transform(positionWorld, viewProjection.matView);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック（コーンに収まっているか）
		if (fabs(arcTangent) <= angleRange_) {
			// 範囲外ではない
			return false;
		}
	}

	// 範囲外である
	return true;
}

Vector3 LockOn::WorldToScreen(Vector3 worldPosition, const ViewProjection& viewProjection) {
	// ビューポート行列
	Matrix4x4 matViewport = MyMath::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列をプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールド->スクリーン座標変換
	Vector3 screenPosition = MyMath::Transform(worldPosition, matViewProjectionViewport);

	return screenPosition;
}
