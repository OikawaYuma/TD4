#include "Gear0State.h"
#include "CarParts/CarEngine/CarGear/CarGear.h"

void NeutralGearState::Update(Gear* gear)
{
	gear;
}

float NeutralGearState::GetSpeed() const
{
	return 0.0f;
}
