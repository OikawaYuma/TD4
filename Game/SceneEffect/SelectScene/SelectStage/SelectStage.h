/**
* @ file Score.h
* @ brief ゲームのスコアに関する設定
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include <list>

#include "Sprite.h"

class SelectStage
{
public:
    void Init(int num);
    void Update(int num);
    void Draw();

public: // Getter
    int32_t GetSumScore() { return sumScore_; }

public: // Setter 
    void SetSumScore(int32_t score) { sumScore_ = score; }
private:
    int gameNumber_[10];
    std::unique_ptr<Sprite> frameSprite_;
    std::unique_ptr<Sprite> numSprite_;

    int digitScore_[6];

    int32_t sumScore_;

    // スコアが加算されるか
    bool isSum_ = false;

	int myNum_ = 0; // 自分の番号
	float timer_ = 0.0f; // タイマー
};
