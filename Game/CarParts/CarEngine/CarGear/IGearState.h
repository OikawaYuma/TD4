#pragma once

class Gear;

class IGearState {
public:
    virtual ~IGearState() = default;
    virtual void Update(Gear* gear) = 0;
    virtual float GetSpeed() const = 0;
};