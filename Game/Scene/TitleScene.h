/**
* @ file TitleScene.h
* @ brief Titleの部分に関するScene
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "PostProcess.h"
#include "GameCamera/TitleCamera/TitleCamera.h"
#include "Skydome/Skydome.h"
#include "WorldDesign/WorldDesign.h"
#include <Car/Car.h>
#include <Logo/Logo.h>
#include <CarSmoke/CarSmoke.h>
#include <map/map.h>
#include "Loder.h"

class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Draw2d()override;
	void Release()override;
	int GameClose()override;

	void DeleteObject();
	void ArrageObj(std::list<std::unique_ptr<map>>& maps);

private:
	std::unique_ptr<PostProcess> postProcess_;
	std::unique_ptr<Car> car_;
	std::unique_ptr<CarSmoke> carSmoke_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Logo> logo_;
	LevelData levelData_{};
	std::list<std::unique_ptr<map>> maps_;
};

