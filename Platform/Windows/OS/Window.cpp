#include "Platform/Windows/OS/Window.h"
#include "Core/System/Exception.h"
#include "Core/System/String.h"
#include "Platform/Windows/UnicodeHelper.h"

namespace Core {
	void Window::Initialize() {
		WNDCLASSW wc = {};
		wc.lpszClassName = L"EngineStandard";
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpszMenuName = NULL;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.lpfnWndProc = Platform::_Windows_WndProc;
		wc.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));

		if (!RegisterClass(&wc)) {
			throw Exception("Window::Initialize: RegisterClass failed!");
		}
	}
	std::shared_ptr<Window> Window::Create(const String& title, const Vector2& position, const Vector2& size) {
		std::shared_ptr<Window> window = std::make_shared<Window>(new Platform::Window);
		window->SetTitle(title);
		window->SetPosition(position);
		window->SetSize(size);
		return window;
	}
}
namespace Platform {
	LRESULT CALLBACK _Windows_WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return DefWindowProc(wnd, msg, wparam, lparam);
	}

	Window::Window() {
		hwnd = CreateWindowW(L"EngineStandard", L"", WS_OVERLAPPEDWINDOW, 0, 0, 256, 256, NULL, NULL, NULL, NULL);
		if (hwnd == NULL) {
			throw Exception("Window::Window: CreateWindow failed!");
		}
		UpdateWindow(hwnd);
		ShowWindow(hwnd, SW_SHOW);
	}

	void Window::Destroy() {

	}
	void Window::SetVisible(Bool visible) {
		ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
	}
	Bool Window::IsVisible() const {
		return IsWindowVisible(hwnd);
	}
	String Window::GetTitle() const {
		Int len = GetWindowTextLength(hwnd);
		if (len <= 0) {
			return "";
		}
		len += 1;
		std::unique_ptr<WCHAR[]> wchar = std::unique_ptr<WCHAR[]>(new WCHAR[len]());
		GetWindowText(hwnd, wchar.get(), len);
		return UnicodeHelper::UnicodeToUTF8(wchar.get());
	}
	void Window::SetTitle(const String& title) {
		SetWindowText(hwnd, UnicodeHelper::UTF8ToUnicode(title).get());
	}
	Vector2 Window::GetSize() const {
		RECT rect = {};
		if (!GetClientRect(hwnd, &rect)) {
			return Vector2(0, 0);
		}
		return Vector2(rect.right, rect.bottom);
	}
	void Window::SetSize(const Vector2& size) {

	}
	Vector2 Window::GetPosition() const {

	}
	void Window::SetPosition(const Vector2& position) {

	}
	Vector2 Window::GetRawPosition() const {
		RECT rect = {};
		if (!GetWindowRect(hwnd, &rect)) {
			return Vector2(0, 0);
		}
		return Vector2(rect.left, rect.top);
	}
	void Window::SetRawPosition(const Vector2& position) {
		SetWindowPos(hwnd, NULL, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}