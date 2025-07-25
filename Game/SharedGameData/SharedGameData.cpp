#include "SharedGameData.h"

SharedGameData* SharedGameData::GetInstance() {
	static SharedGameData instance;
	return &instance;
}

void SharedGameData::Init() {
	// 初期化処理が必要ならここに書く
	selectedStageNo_ = 0; // デフォルトのステージ番号
	carColor_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // デフォルトの車のカラーは白色
}
