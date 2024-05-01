#pragma once
#include "MyMath.h"
#include <fstream>
#include <sstream>
#include <vector>

class LoadFile {
public:
	/// <summary>
	/// CSVファイルからVector3型の数値を読み込む
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <param name="lavel">読み込む種類</param>
	static Vector3 Vector3FromCSV(const std::string& filename, std::string label);
};
