#pragma once
#include "Engine/Application/NativeWindow.h"
#include "Engine/Platform/Windows/BetterWindows.h"

#define PLATFORM_SPECIFIC_CLASS_NATIVEWINDOWMANAGER ::Engine::PlatformSpecific::Windows::NativeWindowManager
#define PLATFORM_SPECIFIC_CLASS_NATIVEWINDOW ::Engine::PlatformSpecific::Windows::NativeWindow

namespace Engine::PlatformSpecific::Windows {
	class NativeWindowManager final :public ::Engine::NativeWindowManager {
		REFLECTION_CLASS(::Engine::PlatformSpecific::Windows::NativeWindowManager, ::Engine::NativeWindowManager) {}

	public:
		void Update() override;

	private:
		class _Initializer final {
		public:
			_Initializer();
			~_Initializer();
		};
		static _Initializer _initializer;
	};

	class NativeWindow final:public ::Engine::NativeWindow {
		REFLECTION_CLASS(::Engine::PlatformSpecific::Windows::NativeWindow, ::Engine::NativeWindow) {
			REFLECTION_SIGNAL(STRL("KeyDown"), { SIGARG(STRL("keyCode"),Variant::Type::Int64) });
		}

	public:
		~NativeWindow();

		bool IsValid() const override;

		String GetTitle() const override;
		bool SetTitle(const String& title) override;
		Vector2 GetPosition() const override;
		bool SetPosition(const Vector2& position) override;
		Vector2 GetSize() const override;
		bool SetSize(const Vector2& size) override;
		
		bool IsVisible() const override;
		bool SetVisible(bool visible) override;
		
		bool IsMinimized() const override;
		bool SetMinimized(bool minimized) override;
		bool IsMaximized() const override;
		bool SetMaximized(bool maximized) override;
		bool HasCloseButton() const override;
		bool SetCloseButton(bool enabled) override;
		bool HasMinimizeButton() const override;
		bool SetMinimizeButton(bool enabled) override;
		bool HasMaximizeButton() const override;
		bool SetMaximizeButton(bool enabled) override;

		bool HasBorder() const override;
		bool SetBorder(bool enabled) override;
		bool IsResizable() const override;
		bool SetResizable(bool resizable) override;

		HWND GetHWnd() const;

		DWORD GetStyle() const;
		DWORD GetExStyle() const;
		bool HasStyleFlag(DWORD style) const;
		bool SetStyleFlag(DWORD style, bool enabled);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static inline constexpr const WCHAR* GlobalWindowClassName = L"EngineBasicWindowClass";
		static inline constexpr DWORD DefaultWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;
		static inline constexpr DWORD DefaultWindowExStyle = WS_EX_ACCEPTFILES;

		static NativeWindow* GetFromHWnd(HWND hWnd);

		static DWORD GetStyle(HWND hWnd);
		static DWORD GetExStyle(HWND hWnd);
		static bool HasStyleFlag(HWND hWnd, DWORD style);
		static bool SetStyleFlag(HWND hWnd, DWORD style,bool enabled);
		

	protected:
		bool Initialize() override;

	private:
		HWND hWnd = NULL;
	};
}
