#include "Engine/Platform/Windows/Window.h"
#include "Engine/Platform/Windows/UnicodeHelper.h"
#include <ShellScalingApi.h>
#include "Engine/Application/Engine.h"
#include "Engine/System/Thread/JobSystem.h"

namespace Engine::PlatformSpecific {
	WindowSystem::WindowSystem() {
		// Make console support UTF-8
		SetConsoleOutputCP(65001);

		bool dpiAwareness = SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		if (!dpiAwareness) {
			WARN_MSG(u8"Cannot enable DPI Awareness mode: Per-Monitor V2. Falling back to Per-Monitor.");
			dpiAwareness = SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
			if (!dpiAwareness) {
				WARN_MSG(u8"Cannot enable DPI Awareness mode: Per-Monitor. Falling back to System. The DPI scaling will be fixed to the scaling of your primary display.");
				dpiAwareness = SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
				FATAL_ASSERT(dpiAwareness, u8"Failed to set a appropriate DPI Awareness mode.");
			}
		}

		// Register basic window class
		WNDCLASSW wc = {};
		wc.lpszClassName = Window::GlobalWindowClassName;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
		wc.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
		wc.lpszMenuName = NULL;
		wc.hInstance = NULL;
		wc.lpfnWndProc = Window::WndProc;

		bool succeeded = RegisterClassW(&wc);
		FATAL_ASSERT(succeeded, u8"RegisterClassW failed to register window class!");
	}

	WindowSystem::~WindowSystem() {
		UnregisterClassW(Window::GlobalWindowClassName, NULL);
	}

	void WindowSystem::Update() {
		MSG msg = {};
		if (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message != WM_QUIT) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}

	Window* Window::GetFromHWnd(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return nullptr;
		}

		LONG_PTR p = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		if (p == NULL) {
			return nullptr;
		}

		Window* wp = (Window*)p;

		return wp;
	}

	Window::Window() {
		HWND w = CreateWindowExW(DefaultWindowExStyle, GlobalWindowClassName, L"", DefaultWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
		if (IsWindow(w)) {
			// Set user data to let hwnd trace back to Window.
			SetWindowLongPtrW(w, GWLP_USERDATA, (LONG_PTR)this);
		}
		ERR_ASSERT(IsWindow(w), u8"CreateWindowW failed to create a window!", return);
		hWnd = w;
	}

	Window::~Window() {
		if (!IsWindow(hWnd)) {
			return;
		}
		DestroyWindow(hWnd);
	}

	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
			{
				Window* nw = Window::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					nw->GetManager()->DestroyWindow(nw->GetId());
				}
				return 0;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

			case WM_KEYDOWN:
			{
				Window* nw = Window::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					Variant key = wParam;
					const Variant* args[1] = { &key };
					nw->EmitSignal(STRL("KeyDown"), (const Variant**)args, 1);
				}
				break;
			}
			case WM_SIZE:
			{
				Window* nw = Window::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					if (wParam != SIZE_MINIMIZED) {
						if (lParam != nw->lastSize) {
							nw->lastSize = lParam;
							Variant argSize = Vector2(LOWORD(lParam), HIWORD(lParam));
							const Variant* args[] = { &argSize };
							nw->EmitSignal(STRL("OnResized"), args, 1);
							//INFO_MSG(String::Format(STRL("RESIZE: {0}"), ObjectUtil::ToString(argSize.AsVector2())).GetRawArray());
						}
					}
				}
			}
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	bool Window::IsValid() const {
		return IsWindow(hWnd);
	}

	String Window::GetTitle() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return String::GetEmpty());

		return title;
	}
	bool Window::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		UniquePtr<WCHAR[]> buffer;
		bool succeeded = UnicodeHelper::UTF8ToUnicode(title, buffer);
		ERR_ASSERT(succeeded, u8"Failed to convert engine string to Windows wide string!", return false);

		succeeded = SetWindowTextW(hWnd, buffer.GetRaw());
		ERR_ASSERT(succeeded, u8"SetWindowTextW failed to set window title!", return false);

		this->title = title;
		return true;
	}

	Vector2 Window::GetPosition() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		POINT pos{};
		ClientToScreen(hWnd, &pos);

		return Vector2(pos.x, pos.y);
	}
	bool Window::SetPosition(const Vector2& position) {

		RECT rect = {};
		rect.left = (int)position.x;
		rect.top = (int)position.y;
		rect.right = 100;
		rect.bottom = 100;
		bool succeeded = AdjustClientToWindow(rect);
		ERR_ASSERT(succeeded, u8"AdjustWindowRect failed to calculate window rect!", return false);

		succeeded = SetWindowPos(hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
		ERR_ASSERT(succeeded, u8"SetWindowPos failed to set window rect!", return false);
		return true;
	}

	Vector2 Window::GetSize() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		RECT rect = {};
		GetClientRect(hWnd, &rect);

		return Vector2(rect.right - rect.left, rect.bottom - rect.top);
	}
	bool Window::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		RECT rect = {};
		rect.left = 0;
		rect.top = 0;
		rect.right = (int)size.x;
		rect.bottom = (int)size.y;
		bool succeeded = AdjustClientToWindow(rect);
		ERR_ASSERT(succeeded, u8"AdjustClientToWindow failed to calculate window rect!", return false);

		succeeded = SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION | SWP_NOMOVE);
		ERR_ASSERT(succeeded, u8"SetWindowPos failed to set window rect!", return false);
		return true;
	}

	bool Window::IsVisible() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_VISIBLE);
		return result;
	}
	bool Window::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		ShowWindow(hWnd, visible ? SW_SHOW : SW_HIDE);
		return true;
	}

	bool Window::IsMinimized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_MINIMIZE);
		return result;
	}
	bool Window::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		ShowWindow(hWnd, minimized ? SW_MINIMIZE : SW_RESTORE);

		return true;
	}
	bool Window::IsMaximized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_MAXIMIZE);
		return result;
	}
	bool Window::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		ShowWindow(hWnd, maximized ? SW_MAXIMIZE : SW_RESTORE);
		return true;
	}
	bool Window::HasCloseButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = GetMenuState(GetSystemMenu(hWnd, false), SC_CLOSE, MF_BYCOMMAND) & MF_ENABLED;
		return result;
	}
	bool Window::SetCloseButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		DWORD flag = (enabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED);
		bool succeeded = EnableMenuItem(GetSystemMenu(hWnd, false), SC_CLOSE, MF_BYCOMMAND | flag);
		return succeeded;
	}
	bool Window::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_MINIMIZEBOX);
		return result;
	}
	bool Window::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool succeeded = Window::SetStyleFlag(hWnd, WS_MINIMIZEBOX, enabled);
		return succeeded;
	}
	bool Window::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_MAXIMIZEBOX);
		return result;
	}
	bool Window::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool succeeded = Window::SetStyleFlag(hWnd, WS_MAXIMIZEBOX, enabled);
		return succeeded;
	}

	bool Window::HasBorder() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_BORDER);
		return result;
	}
	bool Window::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		POINT pos{};
		ClientToScreen(hWnd, &pos);
		RECT rect = {};
		GetClientRect(hWnd, &rect);
		rect.right += rect.left + pos.x;
		rect.bottom += rect.top + pos.y;
		rect.left = pos.x;
		rect.top = pos.y;

		bool succeeded = Window::SetStyleFlag(hWnd, WS_BORDER | WS_CAPTION, enabled);

		bool adjusted = AdjustWindowRectEx(&rect, Window::GetStyle(hWnd), FALSE, Window::GetExStyle(hWnd));
		if (adjusted) {
			SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION);
		}
		return succeeded;
	}

	bool Window::IsResizable() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool result = Window::HasStyleFlag(hWnd, WS_SIZEBOX);
		return result;
	}
	bool Window::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		bool succeeded = Window::SetStyleFlag(hWnd, WS_SIZEBOX, resizable);
		return succeeded;
	}

	HWND Window::GetHWnd() const {
		return hWnd;
	}

	DWORD Window::GetStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_STYLE);
	}
	DWORD Window::GetStyle() const {
		return GetStyle(hWnd);
	}

	DWORD Window::GetExStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_EXSTYLE);
	}
	DWORD Window::GetExStyle() const {
		return GetExStyle(hWnd);
	}

	bool Window::HasStyleFlag(HWND hWnd, DWORD style) {
		return GetStyle(hWnd) & style;
	}
	bool Window::HasStyleFlag(DWORD style) const {
		return HasStyleFlag(hWnd, style);
	}

	bool Window::SetStyleFlag(HWND hWnd, DWORD style, bool enabled) {
		DWORD s = GetStyle(hWnd);
		if (enabled) {
			s |= style;
		} else {
			s &= ~style;
		}
		SetWindowLongW(hWnd, GWL_STYLE, s);
		return true;
	}
	bool Window::SetStyleFlag(DWORD style, bool enabled) {
		return SetStyleFlag(hWnd, style, enabled);
	}
}
