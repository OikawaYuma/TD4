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

private:
	std::unique_ptr<PostProcess> postProcess_;
	std::unique_ptr<Car> car_;
	std::unique_ptr<CarSmoke> carSmoke_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<Logo> logo_;
};

