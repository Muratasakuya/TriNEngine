#pragma once

//============================================================================*/
//	include
//============================================================================*/

// windows
#include <Windows.h>

//============================================================================*/
//	WinApp class
//============================================================================*/
class WinApp {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	WinApp() = default;
	~WinApp() = default;

	void Init();

	bool ProcessMessage();

	//* getter *//

	HWND GetHwnd() const { return hwnd_; }

	WNDCLASS GetWindowClass() const { return windowClass_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	HWND hwnd_;

	WNDCLASS windowClass_;

	//========================================================================*/
	//* functions

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void RegisterWindowClass();

};