#include "HitBoxWire.h"
#include "Line/LineManager.h"
void HitBoxWire::Init(Vector3& scale, Vector3& rotate, Vector3 translate) {
	


	worldTransform_.Initialize();
    worldTransform_.scale_ = scale;
    worldTransform_.rotation_ = rotate;
	worldTransform_.translation_ = translate;
	worldTransform_.UpdateMatrix();

    // 登録
    std::weak_ptr<LineParam> lineParam1 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam1.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = {-0.5f,-0.5f,-0.5f};
		lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam1);
    }

    std::weak_ptr<LineParam> lineParam2 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam2.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,0.5f,-0.5f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam2);
    }
    std::weak_ptr<LineParam> lineParam3 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam3.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { 0.5f,-0.5f,-0.5f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam3);
    }

    std::weak_ptr<LineParam> lineParam4 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam4.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { 0.5f,0.5f,-0.5f };
        lp->worldTransform.parent_ = &worldTransform_;
		lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam4);
    }

    std::weak_ptr<LineParam> lineParam5 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam5.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,-0.5f,-0.5f };
        lp->worldTransform.rotation_ = { -3.1415f/2,0.0f,0.0f};
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam5);
    }

    std::weak_ptr<LineParam> lineParam6 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam6.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,-0.5f,0.5f };
        lp->worldTransform.rotation_ = { -3.1415f / 2,0.0f,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam6);
    }
    std::weak_ptr<LineParam> lineParam7 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam7.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { 0.5f,-0.5f,-0.5f };
        lp->worldTransform.rotation_ = { -3.1415f / 2,0.0f,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam7);
    }

    std::weak_ptr<LineParam> lineParam8 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam8.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { 0.5f,-0.5f,0.5f };
        lp->worldTransform.rotation_ = { -3.1415f / 2,0.0f,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam8);
    }

    std::weak_ptr<LineParam> lineParam9 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam9.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,-0.5f,-0.5f };
        lp->worldTransform.rotation_ = { 0.0f,3.1415f / 2,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam9);
    }

    std::weak_ptr<LineParam> lineParam10 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam10.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,-0.5f,0.5f };
        lp->worldTransform.rotation_ = { 0.0f,3.1415f / 2,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam10);
    }
    std::weak_ptr<LineParam> lineParam11 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam11.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,0.5f,0.5f };
        lp->worldTransform.rotation_ = { 0.0f,3.1415f / 2,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam11);
    }

    std::weak_ptr<LineParam> lineParam12 = LineManager::GetInstance()->StoreLine();
    if (auto lp = lineParam12.lock()) {
        lp->isAlive = true;
        lp->worldTransform.translation_ = { -0.5f,0.5f,-0.5f };
        lp->worldTransform.rotation_ = { 0.0f,3.1415f / 2,0.0f };
        lp->worldTransform.parent_ = &worldTransform_;
        lp->worldTransform.UpdateMatrix();
        lineParams_.push_back(lineParam12);
    }

}

void HitBoxWire::Update() {

}



void HitBoxWire::Release() {

}

