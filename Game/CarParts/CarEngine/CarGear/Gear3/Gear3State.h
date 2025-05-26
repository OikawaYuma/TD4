#pragma once
#include "CarParts/CarEngine/CarGear/IGearState.h"

class Gear3State : public IGearState {
public:
    void Update(Gear* gear) override;
    float GetSpeed() const override;
};
