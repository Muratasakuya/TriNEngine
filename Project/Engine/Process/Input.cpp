#include "Input.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Window/WinApp.h>

// imgui
#include <imgui.h>

#pragma comment(lib,"dInput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")

//============================================================================*/
//	Input classMethods
//============================================================================*/

Input* Input::instance_ = nullptr;

Input* Input::GetInstance() {

	if (instance_ == nullptr) {
		instance_ = new Input();
	}
	return instance_;
}

void Input::Finalize() {

	if (instance_ != nullptr) {

		delete instance_;
		instance_ = nullptr;
	}
}

bool Input::PushKey(BYTE keyNumber) {

	// 指定キーを押していればtrue
	if (key_[keyNumber]) {

		return true;
	}

	return false;
}
bool Input::TriggerKey(BYTE keyNumber) {

	// 現在のフレームで押されていて、前のフレームで押されていなかった場合にtrueを返す
	if (key_[keyNumber] && !keyPre_[keyNumber]) {

		return true;
	}

	return false;
}
bool Input::PushGamepadButton(InputGamePadButtons button) {

	return gamepadButtons_[static_cast<size_t>(button)];
}
bool Input::TriggerGamepadButton(InputGamePadButtons button) {
	return gamepadButtons_[static_cast<size_t>(button)] && !gamepadButtonsPre_[static_cast<size_t>(button)];
}
Vector2 Input::GetLeftStickVal() const {
	return { leftThumbX_,leftThumbY_ };
}
Vector2 Input::GetRightStickVal() const {
	return { rightThumbX_,rightThumbY_ };
}
float Input::ApplyDeadZone(float value) {

	if (std::fabs(value) < deadZone_) {
		return 0.0f;
	}
	return value;
}
Vector2 Input::GetMousePos() const {

	return mousePos_;
}
Vector2 Input::GetMousePrePos() const {

	return mousePrePos_;
}
Vector2 Input::GetMouseMoveValue() const {

	return { static_cast<float>(mouseState_.lX),static_cast<float>(mouseState_.lY) };
}
float Input::GetMouseWheel() {

	return wheelValue_;
}
bool Input::PushMouseLeft() const {

	return mouseButtons_[0];
}
bool Input::PushMouseRight() const {

	return mouseButtons_[1];
}
bool Input::PushMouseCenter() const {

	return mouseButtons_[2];
}
void Input::SetDeadZone(float deadZone) {

	deadZone_ = deadZone;
}

void Input::Init(WinApp* winApp) {

	winApp_ = winApp;

	HRESULT hr;

	// DirectInputの初期化
	dInput_ = nullptr;
	hr = DirectInput8Create(winApp_->GetWindowClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの初期化
	keyboard_ = nullptr;
	hr = dInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット 標準形式
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのリセット
	hr = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	// マウスデバイスの初期化
	hr = dInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのリセット
	hr = mouse_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));

	// マウスの取得開始
	hr = mouse_->Acquire();

}

void Input::Update() {

	mousePrePos_ = mousePos_;

	HRESULT hr;

	// キーボード情報の取得開始
	hr = keyboard_->Acquire();

	// 前回のキー入力を保存
	std::memcpy(keyPre_.data(), key_.data(), key_.size());

	// 全キーの入力状態を取得する
	hr = keyboard_->GetDeviceState(static_cast<DWORD>(key_.size()), key_.data());

	// 前回のゲームパッドの状態を保存
	gamepadStatePre_ = gamepadState_;
	std::memcpy(gamepadButtonsPre_.data(), gamepadButtons_.data(), gamepadButtons_.size());

	// ゲームパッドの現在の状態を取得
	ZeroMemory(&gamepadState_, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &gamepadState_);

	if (dwResult == ERROR_SUCCESS) {

#pragma region ///ゲームパッドが接続されている場合の処理 ///
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_UP)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_DOWN)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_LEFT)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_RIGHT)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::START)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::BACK)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::LEFT_THUMB)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::RIGHT_THUMB)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::LEFT_SHOULDER)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::RIGHT_SHOULDER)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::A)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::B)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::X)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
		gamepadButtons_[static_cast<size_t>(InputGamePadButtons::Y)] = (gamepadState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

		// スティックの状態を更新
		leftThumbX_ = ApplyDeadZone(gamepadState_.Gamepad.sThumbLX);
		leftThumbY_ = ApplyDeadZone(gamepadState_.Gamepad.sThumbLY);
		rightThumbX_ = ApplyDeadZone(gamepadState_.Gamepad.sThumbRX);
		rightThumbY_ = ApplyDeadZone(gamepadState_.Gamepad.sThumbRY);
#pragma endregion
	} else {

		// ゲームパッドが接続されていない場合の処理
		std::fill(gamepadButtons_.begin(), gamepadButtons_.end(), false);

		leftThumbX_ = 0.0f;
		leftThumbY_ = 0.0f;
		rightThumbX_ = 0.0f;
		rightThumbY_ = 0.0f;
	}

	// マウス情報の取得開始
	hr = mouse_->Acquire();
	if (FAILED(hr)) {
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {

			mouse_->Acquire();
		}
	}

	hr = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);

	if (FAILED(hr)) {
		// 取得失敗時の処理
		std::fill(mouseButtons_.begin(), mouseButtons_.end(), false);
		mousePos_ = { 0.0f, 0.0f };
	} else {

		// マウスボタンの状態を保存
		mouseButtons_[0] = (mouseState_.rgbButtons[0] & 0x80) != 0;
		mouseButtons_[1] = (mouseState_.rgbButtons[1] & 0x80) != 0;
		mouseButtons_[2] = (mouseState_.rgbButtons[2] & 0x80) != 0;

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(winApp_->GetHwnd(), &point);

		// マウスの移動量を保存
		mousePos_.x = static_cast<float>(point.x);
		mousePos_.y = static_cast<float>(point.y);

		// ホイール値
		wheelValue_ = static_cast<float>(mouseState_.lZ) / WHEEL_DELTA;
	}
}

void Input::ImGui() {

	if (ImGui::CollapsingHeader("Mouse Input")) {

		Vector2 mouseMoveValue = GetMouseMoveValue();
		ImGui::Text("Position: { %4.1f, %4.1f }", mousePos_.x, mousePos_.y);
		ImGui::Text("Movement: { %4.1f, %4.1f }", mouseMoveValue.x, mouseMoveValue.y);
		ImGui::Text("WheelDirection:   %4.1f", wheelValue_);
		ImGui::Text("Left Button:   %d", mouseButtons_[0]);
		ImGui::Text("Right Button:  %d", mouseButtons_[1]);
		ImGui::Text("Center Button: %d", mouseButtons_[2]);
	}

	// ゲームパッドが接続されている場合にのみ表示
	if (ImGui::CollapsingHeader("Gamepad Input")) {
		if (XInputGetState(0, &gamepadState_) == ERROR_SUCCESS) {

			ImGui::Text("Button A: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::A)]);
			ImGui::Text("Button B: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::B)]);
			ImGui::Text("Button X: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::X)]);
			ImGui::Text("Button Y: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::Y)]);
			ImGui::Text("D-Pad Up: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_UP)]);
			ImGui::Text("D-Pad Down: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_DOWN)]);
			ImGui::Text("D-Pad Left: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_LEFT)]);
			ImGui::Text("D-Pad Right: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::ARROW_RIGHT)]);
			ImGui::Text("Left Shoulder: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::LEFT_SHOULDER)]);
			ImGui::Text("Right Shoulder: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::RIGHT_SHOULDER)]);
			ImGui::Text("Start: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::START)]);
			ImGui::Text("Back: %d", gamepadButtons_[static_cast<size_t>(InputGamePadButtons::BACK)]);

			ImGui::DragFloat("DeadZone", &deadZone_);
			ImGui::Text("Left Stick: { %4.1f, %4.1f }", leftThumbX_, leftThumbY_);
			ImGui::Text("Right Stick: { %4.1f, %4.1f }", rightThumbX_, rightThumbY_);
		} else {

			ImGui::Text("Gamepad is not connected");
		}
	}
}