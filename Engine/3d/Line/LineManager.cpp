#include "LineManager.h"

void LineManager::Init()
{
	LineManager::GetInstance()->lines_ = std::make_unique<Line>();
    LineManager::GetInstance()->lines_->Initialize();
}


void LineManager::Update()
{
	LineManager::GetInstance()->lines_->Update();
}

void LineManager::Draw(Camera* camera)
{
	LineManager::GetInstance()->lines_->Draw(camera);
}

void LineManager::Release()
{
	//LineManager::GetInstance()->lines_->Release();
}

void LineManager::CreateLine(const std::string& lineName)
{
	lineName;
}

std::shared_ptr<LineParam> LineManager::StoreLine()
{
    std::shared_ptr<LineParam> lineParam = std::make_shared<LineParam>();
    lineParam->isAlive = true;
    lineParam->worldTransform.Initialize();
    lineParam->color = { 1.0f,1.0f,1.0f,1.0f };
    LineManager::GetInstance()->lines_->AddListPram(lineParam);
    return lineParam;
}

LineManager* LineManager::GetInstance()
{
    static LineManager instance;
    return &instance;

}