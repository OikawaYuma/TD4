#pragma once

#include "Camera.h"
#include <list>
#include <string>
#include <memory>
#include "Line/Line.h"
	

class LineColliderManager
{
public:
	LineColliderManager() = default;
	~LineColliderManager() = default;
	const LineColliderManager& operator=(const LineColliderManager&) = delete;
public:
	static void Init();
	static void Update();
	static void Draw();
	static void Release();

public:
	static void CreateObject(const std::string& objectName);
	//static uint32_t StoreObject(const std::string& objectName,WorldTransform* worldTransform,uint32_t skinTex, Vector4* color,uint32_t transparency);

public:
	static std::shared_ptr<LineParam> StoreObject(const std::string& objectName, uint32_t transparency);

public: // Getter

	static LineColliderManager* GetInstance();

private:
	//std::map<std::string, Object3dData> objectDatas_;
	std::list<std::unique_ptr<Line>> wireObjects_;
	static std::string objectName_;
	//ModelData modelData_;
};
class LineManager
{
};

