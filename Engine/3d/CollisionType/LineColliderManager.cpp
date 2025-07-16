#include "LineColliderManager.h"

void LineColliderManager::Init()
{
    LineColliderManager::GetInstance()->wireObjects_.clear();
}


void LineColliderManager::Update()
{
    for (const auto& pair : LineColliderManager::GetInstance()->wireObjects_) {
        pair->Update();
    }
}

void LineColliderManager::Draw()
{
    for (const auto& pair : LineColliderManager::GetInstance()->wireObjects_) {
        pair->Draw();
    }
}

void LineColliderManager::Release()
{
    for (auto& pair : LineColliderManager::GetInstance()->wireObjects_) {
        pair->Release(); // ComPtr は自動的に解放するので、これで十分
        // srvIndex や他のリソースも適切に解放する必要があれば、ここで行う
    }
}


void LineColliderManager::CreateObject(const std::string& objectName)
{
    std::string a = objectName;
}


std::shared_ptr<WireObjectPram> LineColliderManager::StoreObject(const std::string& objectName,uint32_t transparency)
{
    transparency;
    for (auto& pair : LineColliderManager::GetInstance()->wireObjects_) {
        if (pair->GetObjectName() == objectName) {

            std::shared_ptr<WireObjectPram> objectPram = std::make_shared<WireObjectPram>();
            objectPram->isAlive = true;
            objectPram->worldTransform.Initialize();
            objectPram->color = { 1.0f,1.0f,1.0f,1.0f };
            pair->AddListPram(objectPram);

            return objectPram;
        };
    }
    std::unique_ptr<HitBoxWire> object = std::make_unique<HitBoxWire>();
   // object->Init();

    std::shared_ptr<WireObjectPram> objectPram = std::make_shared<WireObjectPram>();
    objectPram->isAlive = true;
    objectPram->worldTransform.Initialize();
    objectPram->color = { 1.0f,1.0f,1.0f,1.0f };
    object->AddListPram(objectPram);
    LineColliderManager::GetInstance()->wireObjects_.push_back(std::move(object));
    return objectPram;
}

LineColliderManager* LineColliderManager::GetInstance()
{
    static LineColliderManager instance;
    return &instance;

}