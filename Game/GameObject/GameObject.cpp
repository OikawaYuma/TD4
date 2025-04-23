#include "GameObject.h"

void GameObject::Initialize()
{
	// 空にしておく
	objects_.clear();

	// 車
	std::unique_ptr<BaseObject> obj = std::make_unique<Car>();
	obj->Initialize({ 1.0f,1.0f,1.0f }, { 0,0,0 }, { 0,0,0 }, "car.obj");
	objects_.push_back(std::move(obj));
}

void GameObject::Update()
{
	for (const auto& obj : objects_) {
		obj->Update();
	}
}
