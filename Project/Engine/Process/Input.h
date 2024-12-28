#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Lib/MathUtils/Vector2.h>

// directInput
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <XInput.h>

// c++
#include <cmath>
#include <cstdint>
#include <array>
#include <cassert>

// front
class WinApp;

//============================================================================*/
//	Input enum class
//============================================================================*/
// XINPUTGamePadのボタンの種類
enum class InputGamePadButtons {

	ARROW_UP,       // 十字ボタンの上方向
	ARROW_DOWN,     // 十字ボタンの下方向
	ARROW_LEFT,     // 十字ボタンの左方向
	ARROW_RIGHT,    // 十字ボタンの右方向
	START,          // スタートボタン
	BACK,           // バックボタン
	LEFT_THUMB,     // 左スティックのボタン
	RIGHT_THUMB,    // 右スティックのボタン
	LEFT_SHOULDER,  // 左ショルダーボタン（LB）
	RIGHT_SHOULDER, // 右ショルダーボタン（RB）
	A,              // Aボタン
	B,              // Bボタン
	X,              // Xボタン
	Y,              // Yボタン
	Counts          // ボタンの数を表すための定数
};

//============================================================================*/
//	Input class
//============================================================================*/
class Input {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	static Input* GetInstance();
	static void Finalize();

	void Init(WinApp* winApp);

	void Update();

	void ImGui();

	//* inputKey *//

	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

	//* inputGamepad *//

	bool PushGamepadButton(InputGamePadButtons button);
	bool TriggerGamepadButton(InputGamePadButtons button);

	//* inputMouse *//

	bool PushMouseLeft() const;
	bool PushMouseRight() const;
	bool PushMouseCenter() const;

	//* getter *//

	Vector2 GetLeftStickVal() const;
	Vector2 GetRightStickVal() const;

	Vector2 GetMousePos() const;
	Vector2 GetMousePrePos() const;
	Vector2 GetMouseMoveValue() const;
	float GetMouseWheel();

	//* setter *//

	void SetDeadZone(float deadZone);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static Input* instance_;

	WinApp* winApp_ = nullptr;

	//* key *//

	static const uint32_t keyMaxNum_ = 256;

	std::array<BYTE, keyMaxNum_> key_{};
	std::array<BYTE, keyMaxNum_> keyPre_{};

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> keyboard_;
	
	//* gamepad *//

	XINPUT_STATE gamepadState_{};
	XINPUT_STATE gamepadStatePre_{};

	std::array<bool, static_cast<size_t>(InputGamePadButtons::Counts)> gamepadButtons_{};
	std::array<bool, static_cast<size_t>(InputGamePadButtons::Counts)> gamepadButtonsPre_{};

	float leftThumbX_;
	float leftThumbY_;
	float rightThumbX_;
	float rightThumbY_;

	// デッドゾーンの閾値
	float deadZone_ = 4800.0f;

	//* mouse *//

	DIMOUSESTATE mouseState_;

	ComPtr<IDirectInputDevice8> mouse_; // マウスデバイス

	std::array<bool, 3> mouseButtons_;  // マウスボタンの状態
	Vector2 mousePos_;                  // マウスの座標
	Vector2 mousePrePos_;               // マウスの前座標
	float wheelValue_;                  // ホイール移動量

	//========================================================================*/
	//* functions

	float ApplyDeadZone(float value);

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;

};