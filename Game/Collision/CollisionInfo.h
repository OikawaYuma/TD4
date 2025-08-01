#pragma once
class Collider; // 前方宣言

#include "Vector3.h"

struct CollisionInfo {
	Collider* other = nullptr;
	Vector3 normal{};
	float penetration = 0.0f;
	float time = 0.0f;
};
