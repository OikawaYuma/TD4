/**
* @ file ObjectManager.h
* @ brief Objectを管理するクラス Singleton Patternで制作
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "Collisiontype/Box/HitBoxWire.h"
#include "Camera.h"
#include <map>
#include <string>

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
	static std::shared_ptr<WireObjectPram> StoreObject(const std::string& objectName,  uint32_t transparency);

public: // Getter

	static LineColliderManager* GetInstance();

private:
	//std::map<std::string, Object3dData> objectDatas_;
	std::list<std::unique_ptr<HitBoxWire>> wireObjects_;
	static std::string objectName_;
	//ModelData modelData_;
};
