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

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField
{
public:
	// 読み込み済みデータをリセット
	void ResetMapChipData();
	// CSVファイルの読み込み
	void LoadMapChipCsv(const std::string& filePath);

	// マップチップ種別の取得
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップ座標の取得
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

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

