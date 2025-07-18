#pragma once

#include "Camera.h"
#include <list>
#include <string>
#include <memory>
#include "Line/Line.h"
	

class LineManager
{
public:
	LineManager() = default;
	~LineManager() = default;
	const LineManager& operator=(const LineManager&) = delete;
public:
	static void Init();
	static void Update();
	static void Draw(Camera* camera);
	static void Release();

public:
	static void CreateLine(const std::string& lineName);
	//static uint32_t StoreObject(const std::string& objectName,WorldTransform* worldTransform,uint32_t skinTex, Vector4* color,uint32_t transparency);

public:
	static std::shared_ptr<LineParam> StoreLine();

public: // Getter

	static LineManager* GetInstance();

private:
	std::unique_ptr<Line> lines_;
	WorldTransform worldTransform_;
	std::list <std::weak_ptr<LineParam>> lineParams_;
	std::string objectName_;
	//ModelData modelData_;
};

