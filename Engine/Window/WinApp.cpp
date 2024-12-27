#include "WinApp.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

// imgui
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib,"winmm.lib")

//============================================================================*/
//	WinApp classMethods
//============================================================================*/

void WinApp::Init() {

	timeBeginPeriod(1);
	RegisterWindowClass();

	RECT wrc = {};
	wrc.right = kWindowWidth;
	wrc.bottom = kWindowHeight;

	UINT windowStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRect(&wrc, windowStyle, false);

	hwnd_ = CreateWindow(
		L"WindowClass", kWindowTitle, windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wrc.right - wrc.left, wrc.bottom - wrc.top,
		nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

	ShowWindow(hwnd_, SW_SHOW);

}

bool WinApp::ProcessMessage() {

	MSG msg{};

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT) {

		return true;
	}

	return false;
}

LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	// ImGuiマウス有効
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	switch (msg) {
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


void WinApp::RegisterWindowClass() {

	// Window Procedure
	windowClass_.lpfnWndProc = WindowProc;
	windowClass_.lpszClassName = L"WindowClass";
	windowClass_.hInstance = GetModuleHandle(nullptr);
	windowClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&windowClass_);
}