#include "LoadFile.h"
#include <cassert>

Vector3 LoadFile::Vector3FromCSV(const std::string& filename, std::string label) {
	std::ifstream file(filename);
	// ファイルが開けない場合
	if (!file.is_open()) {
		assert(false);
	}

	 std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string currentLabel;
		float x, y, z;
		char comma;
		// ラベルを読み込む
		if (ss >> currentLabel) {
			// ラベルが一致するかチェック
			if (currentLabel == label) {
				// ラベルが一致した場合、値を読み込む
				if (ss >> x >> comma >> y >> comma >> z) {
					// 読み込んだ値を返す
					return Vector3(x, y, z);
				}
			}
		}
	}

	return Vector3(0, 0, 0);
}
