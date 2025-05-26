#pragma once
#include "CarParts/CarEngine/CarGear/IGearState.h"

class Gear5State : public IGearState {
public:
    void Update(Gear* gear) override;
    float GetSpeed() const override;
};