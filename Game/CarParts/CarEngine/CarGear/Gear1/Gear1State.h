#pragma once
#include "CarParts/CarEngine/CarGear/IGearState.h"

class Gear1State : public IGearState {
public:
    void Update(Gear* gear) override;
    float GetSpeed() const override;
};