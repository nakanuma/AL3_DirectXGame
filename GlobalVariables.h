#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <Vector3.h>
#include <map>

// 項目
struct Item {
	// 項目の値
	std::variant<int32_t, float, Vector3> value;
};
// グループ
struct Group {
	std::map<std::string, Item> items;
};

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

	// 全データ
	std::map<std::string, Group> datas_;
};

