#pragma once
#include <memory>
#include <vector>
#include "WorldTransform.h"
#include "Line/Line.h"

// object1個分のデータ
struct WireObject3dData {
	std::string objectName;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	uint32_t instancingNum;
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

};

struct WireObjectPram {
	bool isAlive;
	WorldTransform worldTransform;
	Vector4 color;
};
class HitBoxWire
{
public:
	void Init(Vector3& scale,Vector3 translate);
	void Update();
	void Draw();
	void Release();

public:
	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetWorldTransformParent(const WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

public: // Getter

	std::string GetObjectName() { return wireObject3dData_.objectName; }

public:
	void AddListPram(std::shared_ptr<WireObjectPram > objectParm);

private:
	WorldTransform worldTransform_;
	std::vector<std::unique_ptr<Line>> lines_;
	Camera* camera_ = nullptr;

	// object1個分のデータ
	WireObject3dData wireObject3dData_;
};

