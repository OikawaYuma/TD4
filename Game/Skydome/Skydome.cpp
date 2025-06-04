#include "Skydome.h"
#include "TextureManager.h"
#include "ModelManager.h"

void Skydome::Init()
{
	//ModelManager::GetInstance()->LoadModel("Resources/skydome", "skydome.obj");
	ModelManager::GetInstance()->LoadModel("Resources/tenq", "TenQ.obj");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f }; // スケール
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f }; // 原点に配置
	object_ = std::make_unique<Object3d>();
	object_->Init();
	object_->SetMaterial({ {1.0f,1.0f,1.0f,1.0f} , false });
	//object_->SetModel("skydome.obj");
	object_->SetModel("TenQ.obj");
	object_->SetWorldTransform(worldTransform_);
	
	object_->Update();
	worldTransform_.UpdateMatrix();
	//skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/skydome.png");
	skinTex_ = TextureManager::GetInstance()->StoreTexture("Resources/tenq/TenQ.png");

	object_->SetSkinTex(skinTex_);
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
	object_->SetWorldTransform(worldTransform_);
	object_->Update();
	object_->ModelDebug();
}

void Skydome::Draw(Camera* camera)
{
	object_->SetSkinTex(skinTex_);
	object_->Draw(camera);
}
