#include "HitBoxWire.h"

void HitBoxWire::Init() {
	
	// 8頂点の定義
	Vector3 v0 = { -0.5f, -0.5f, -0.5f };
	Vector3 v1 = { 0.5f, -0.5f, -0.5f };
	Vector3 v2 = { -0.5f,  0.5f, -0.5f };
	Vector3 v3 = { 0.5f,  0.5f, -0.5f };
	Vector3 v4 = { -0.5f, -0.5f,  0.5f };
	Vector3 v5 = { 0.5f, -0.5f,  0.5f };
	Vector3 v6 = { -0.5f,  0.5f,  0.5f };
	Vector3 v7 = { 0.5f,  0.5f,  0.5f };

	// 頂点ペアの配列（12本）
	std::vector<std::pair<Vector3, Vector3>> edges = {
		{v0, v1}, {v1, v5}, {v5, v4}, {v4, v0}, // bottom
		{v2, v3}, {v3, v7}, {v7, v6}, {v6, v2}, // top
		{v0, v2}, {v1, v3}, {v5, v7}, {v4, v6}  // verticals
	};

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 2.0f, 5.0f };
	worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
	worldTransform_.UpdateMatrix();
	// 追加
	for (const auto& [start, end] : edges) {
		std::unique_ptr<Line> line = std::make_unique<Line>();
		line->Initialize(start, end);
		line->SetWorldTransform(worldTransform_);
		lines_.push_back(std::move(line));
	}
}

void HitBoxWire::Update() {
	for (auto itr = lines_.begin(); itr != lines_.end(); itr++) {
		(*itr)->Update();
	}

}

void HitBoxWire::Draw() {
	for (auto itr = lines_.begin(); itr != lines_.end(); itr++) {
		(*itr)->Draw(camera_);
	}
			
}