#include "CarGear.h"
#include "Gear0/Gear0State.h"
#include "Gear1/Gear1State.h"
#include "Gear2/Gear2State.h"
#include "Gear3/Gear3State.h"
#include "Gear4/Gear4State.h"
#include "Gear5/Gear5State.h"

void Gear::Initialize()
{
    // 各ギア状態を初期化
    gearStates[0] = std::make_unique<NeutralGearState>();
    gearStates[1] = std::make_unique<Gear1State>();
    gearStates[2] = std::make_unique<Gear2State>();
    gearStates[3] = std::make_unique<Gear3State>();
    gearStates[4] = std::make_unique<Gear4State>();
    gearStates[5] = std::make_unique<Gear5State>();

    // ニュートラルから開始
    currentGear = 0;
    // 現在のステートをギアに対応するステートに設定
    currentState = gearStates[currentGear].get();
}

void Gear::ChangeState(int gearNum) {
    // 範囲外の入力を無視
    if (gearNum < 0 || gearNum >= static_cast<int>(gearStates.size())) return;
    // 同じギアへの切り替えは不要
    if (gearNum == currentGear) return;

    // ギア変更処理
    currentGear = gearNum;
    // 新しいギアステートに切り替え
    currentState = gearStates[currentGear].get();
}

void Gear::Update()
{
    // 数字キーでギア切り替え（0～5）
    if (Input::GetInstance()->PushKey(DIK_0)) ChangeState(0);
    else if (Input::GetInstance()->PushKey(DIK_1)) ChangeState(1);
    else if (Input::GetInstance()->PushKey(DIK_2)) ChangeState(2);
    else if (Input::GetInstance()->PushKey(DIK_3)) ChangeState(3);
    else if (Input::GetInstance()->PushKey(DIK_4)) ChangeState(4);
    else if (Input::GetInstance()->PushKey(DIK_5)) ChangeState(5);

    // 現在のギア状態に応じた処理を実行
    currentState->Update(this);
}

float Gear::GetCurrentSpeed() const {
    return currentState ? currentState->GetSpeed() : 0.0f;
}

int Gear::GetCurrentGear() const
{
    return currentGear;
}
