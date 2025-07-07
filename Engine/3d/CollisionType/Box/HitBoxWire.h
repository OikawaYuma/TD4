#pragma once
#include <memory>
#include <vector>
#include "WorldTransform.h"
#include "Line/Line.h"
class HitBoxWire
{
public:
	void Init();
	void Update();
	void Draw();

public:
	void SetCamera(Camera* camera) { camera_ = camera; }

private:
	WorldTransform worldTransform_;
	std::vector<std::unique_ptr<Line>> lines_;
	Camera* camera_ = nullptr;
};

