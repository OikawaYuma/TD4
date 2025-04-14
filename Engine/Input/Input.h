/**
* @ file Input.h
* @ brief キーボードやPadの入力をまとめるクラス　Singleton Patternで作成
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <Xinput.h>
#include <stdint.h>
//#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input 
{
public:
	static Input* GetInstance();
	Input() = default;
	~Input() = default;

	const Input& operator=(const Input&) = delete;

	void Initialize();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);
	
	XINPUT_STATE GetJoyState() { return joyState; };

	bool GetJoystickState();

	bool PushJoyButton(uint32_t button);

	bool TriggerJoyButton(uint32_t button);

	float JoyStickParmLX(float num);
	float JoyStickParmLY(float num);
	float JoyStickParmRX(float num);
	float JoyStickParmRY(float num);

	// namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	HRESULT result;
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	BYTE keys[256];
	BYTE preKeys[256];

	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;
	
	// デッドゾーンの閾値を定義
	const static float DEADZONE_THRESHOLD;
};

