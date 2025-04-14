/**
* @ file
* @ brief
*/

#pragma once

#include <functional>
/// <summary>
/// 時限発動
/// </summary>
class TimedCall
{
public:
	TimedCall(std::function<void()> p, uint32_t time);

	void Update();
	bool IsFinished() {
		return isFinish_;
	}
private:
	uint32_t time_;
	bool isFinish_ = false;
	std::function<void()> pFunc_;
};

