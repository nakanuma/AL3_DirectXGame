#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "Vector3.h"

// マップチップ種別の定義
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

// マップチップデータ構造体
struct MapChipData {
	std::vector <std::vector<MapChipType>> data;
};

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField
{
public:
	// 矩形範囲
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

	// 読み込み済みデータをリセット
	void ResetMapChipData();
	// CSVファイルの読み込み
	void LoadMapChipCsv(const std::string& filePath);

	// マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップ座標の取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	// 指定座標からマップチップの番号を取得
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	// 指定ブロックの全方向の境界の座標を取得
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	// アクセッサ
	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};

