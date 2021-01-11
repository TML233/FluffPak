#ifndef HEADER_PLATFORM_WINDOWS_WINDOW
#define HEADER_PLATFORM_WINDOWS_WINDOW

#include "Core/OS/Window.h"
#include "Platform/Windows/Include.h"

using namespace Core;

namespace Platform {
	// Window event callback
	LRESULT CALLBACK _Windows_WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	class Window final:public Core::Window {
	public:
		~Window();

		void Destroy() override;
		void SetVisible(Bool visible) override;
		Bool IsVisible() const override;
		String GetTitle() const override;
		void SetTitle(const String& title) override;
		Vector2 GetSize() const override;
		void SetSize(const Vector2& size) override;
		Vector2 GetPosition() const override;
		void SetPosition(const Vector2& position) override;
		Vector2 GetRawPosition() const override;
		void SetRawPosition(const Vector2& position) override;
	private:
		friend Core::Window;

		Window();
		HWND hwnd = NULL;
	};
}

#endif