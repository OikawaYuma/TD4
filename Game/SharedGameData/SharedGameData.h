#pragma once
#include "Vector4.h"
/**
* @ file SharedGameData.h
* @ brief SharedGameDataクラス　Scene間で共有するデータを保持するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/
class SharedGameData
{
public: //初期化
	void Init();
private:
	SharedGameData() = default;
	~SharedGameData() = default;
	// コピーコンストラクタと代入演算子を削除
	const SharedGameData& operator=(const SharedGameData&) = delete;
public:
	static SharedGameData* GetInstance();


public: //Getter
	//　選ばれてるステージの番号
	int GetSelectedStageNo() const { return selectedStageNo_; } // 0:ステージ1, 1:ステージ2, 2:ステージ3, 3:ステージ4
	// 作られた車のカラー
	const Vector4& GetCarColor() const { return carColor_; }
public: //Setter
	//　選ばれてるステージの番号
	void SetSelectedStageNo(int stageNo) { selectedStageNo_ = stageNo; } // 0:ステージ1, 1:ステージ2, 2:ステージ3, 3:ステージ4
	// 作られた車のカラー
	void SetCarColor(const Vector4& color) { carColor_ = color; } 

public: // 共有するデータ
	//　選ばれてるステージの番号
	int selectedStageNo_ = 0; // 0:ステージ1, 1:ステージ2, 2:ステージ3, 3:ステージ4

	// 作られた車のカラー
	Vector4 carColor_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // デフォルトは白色
	
};

