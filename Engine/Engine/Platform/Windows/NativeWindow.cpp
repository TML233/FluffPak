#include "Engine/Platform/Windows/NativeWindow.h"
#include "Engine/Platform/Windows/UnicodeHelper.h"

namespace Engine::PlatformSpecific {
	typename NativeWindowManager::_Initializer NativeWindowManager::_initializer{};

	NativeWindowManager::_Initializer::_Initializer() {
		// Make console support UTF-8
		SetConsoleOutputCP(65001);

		// Register basic window class
		WNDCLASSW wc = {};
		wc.lpszClassName = NativeWindow::GetGlobalWindowClassName();
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
		wc.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
		wc.lpszMenuName = NULL;
		wc.hInstance = NULL;
		wc.lpfnWndProc = NativeWindow::WndProc;

		bool succeeded = RegisterClassW(&wc);
		FATAL_ASSERT(succeeded, "RegisterClassW failed to register window class!");
	}

	NativeWindowManager::_Initializer::~_Initializer() {
		UnregisterClassW(NativeWindow::GetGlobalWindowClassName(), NULL);
	}

	void NativeWindowManager::Update() {
		MSG msg = {};
		while (PeekMessageW(&msg, NULL, NULL, NULL,PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	WCHAR* NativeWindow::GetGlobalWindowClassName() {
		return L"EngineBasicWindowClass";
	}

	NativeWindow* NativeWindow::GetFromHWnd(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return nullptr;
		}

		LONG_PTR p = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		if (p == NULL) {
			return nullptr;
		}

		NativeWindow* wp = (NativeWindow*)p;

		return wp;
	}

	NativeWindow::NativeWindow() {
		const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		HWND w = CreateWindowW(GetGlobalWindowClassName(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
		ERR_ASSERT(IsWindow(w), "CreateWindowW failed to create a window!", return);
		// Set user data to let hwnd trace back to NativeWindow.
		SetWindowLongPtrW(w, GWLP_USERDATA, (LONG_PTR)this);

		hWnd = w;
	}
	NativeWindow::~NativeWindow() {
		if (!IsWindow(hWnd)) {
			return;
		}
		DestroyWindow(hWnd);
	}

	LRESULT CALLBACK NativeWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
			{
				NativeWindow* nw = NativeWindow::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					nw->Destroy();
				} else {
					ERR_MSG("User data in hWnd is not a NativeWindow ptr! This shouldn't happen!");
					DestroyWindow(hWnd);
				}
			}
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	bool NativeWindow::IsValid() const {
		return IsWindow(hWnd);
	}

	String NativeWindow::GetTitle() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return String::GetEmpty());

		int len = GetWindowTextLengthW(hWnd);
		if (len <= 0) {
			return String::GetEmpty();
		}
		len += 1;

		UniquePtr<WCHAR[]> buffer = UniquePtr<WCHAR[]>::Create(len);
		GetWindowTextW(hWnd, buffer.GetRaw(), len);

		String result;
		bool succeeded = UnicodeHelper::UnicodeToUTF8(buffer.GetRaw(), result);

		ERR_ASSERT(succeeded, "Failed to convert Windows wide string to engine string!", return String::GetEmpty());

		return result;
	}
	bool NativeWindow::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		UniquePtr<WCHAR[]> buffer;
		bool succeeded = UnicodeHelper::UTF8ToUnicode(title, buffer);
		
		ERR_ASSERT(succeeded, "Failed to convert engine string to Windows wide string!", return false);

		succeeded=SetWindowTextW(hWnd, buffer.GetRaw());
		ERR_ASSERT(succeeded, "SetWindowTextW failed to set window title!", return false);
		return true;
	}

	Vector2 NativeWindow::GetPosition() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		POINT pos{};
		ClientToScreen(hWnd, &pos);

		return Vector2((float)pos.x, (float)pos.y);
	}
	bool NativeWindow::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		RECT rect = {};
		rect.left = (int)position.x;
		rect.top = (int)position.y;
		rect.right = 100;
		rect.bottom = 100;
		bool succeeded = AdjustWindowRect(&rect, GetStyle(), FALSE);
		ERR_ASSERT(succeeded, "AdjustWindowRect failed to calculate window rect!", return false);

		succeeded=SetWindowPos(hWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
		ERR_ASSERT(succeeded, "SetWindowPos failed to set window rect!", return false);
		return true;
	}
	Vector2 NativeWindow::GetSize() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		RECT rect = {};
		GetClientRect(hWnd, &rect);

		return Vector2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
	}
	bool NativeWindow::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		RECT rect = {};
		rect.left = 0;
		rect.top = 0;
		rect.right = (int)size.x;
		rect.bottom = (int)size.y;
		AdjustWindowRect(&rect, GetStyle(), FALSE);

		SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION | SWP_NOMOVE);
		return true;
	}

	bool NativeWindow::IsVisible() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_VISIBLE);
	}
	bool NativeWindow::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		bool succeed = SetStyleFlag(WS_VISIBLE, visible);
		UpdateWindow(hWnd);
		return succeed;
	}
	
	bool NativeWindow::IsMinimized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_MINIMIZE);
	}
	bool NativeWindow::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_MINIMIZE, minimized);
	}
	bool NativeWindow::IsMaximized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_MAXIMIZE);
	}
	bool NativeWindow::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_MAXIMIZE, maximized);
	}
	bool NativeWindow::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_MINIMIZEBOX);
	}
	bool NativeWindow::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_MINIMIZEBOX, enabled);
	}
	bool NativeWindow::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_MAXIMIZEBOX);
	}
	bool NativeWindow::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_MAXIMIZEBOX, enabled);
	}

	bool NativeWindow::HasBorder() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_BORDER);
	}
	bool NativeWindow::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_BORDER, enabled);
	}
	bool NativeWindow::IsResizable() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return HasStyleFlag(WS_SIZEBOX);
	}
	bool NativeWindow::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return SetStyleFlag(WS_SIZEBOX, resizable);
	}

	DWORD NativeWindow::GetStyle() const {
		return GetWindowLongW(hWnd, GWL_STYLE);
	}
	bool NativeWindow::HasStyleFlag(DWORD style) const {
		return GetStyle() & style;
	}
	bool NativeWindow::SetStyleFlag(DWORD style, bool enabled) {
		DWORD s = GetStyle();
		if (enabled) {
			s |= style;
		} else {
			s &= ~style;
		}
		SetWindowLongW(hWnd, GWL_STYLE, s);
		return true;
	}
}
