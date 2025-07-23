#pragma once
#include <memory>
#include <vector>
#include "WorldTransform.h"
#include "Line/Line.h"
#include <cmath>


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
	void Init(Vector3& scale, Vector3& rotate ,Vector3 translate);
	void Update();
	void Release();

public:
	void SetWorldTransformParent(const WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

public: // Getter

	std::string GetObjectName() { return wireObject3dData_.objectName; }

private:
	WorldTransform worldTransform_;
	std::list<std::weak_ptr<LineParam>> lineParams_;
	// object1個分のデータ
	WireObject3dData wireObject3dData_;
};

