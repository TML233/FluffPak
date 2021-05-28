#pragma once
#include "Engine/Application/Window.h"
#include "Engine/Platform/Windows/BetterWindows.h"

#define PLATFORM_SPECIFIC_CLASS_WINDOWMANAGER ::Engine::PlatformSpecific::Windows::WindowManager
#define PLATFORM_SPECIFIC_CLASS_WINDOW ::Engine::PlatformSpecific::Windows::Window

namespace Engine::PlatformSpecific::Windows {
	class WindowManager final :public ::Engine::WindowManager {
		REFLECTION_CLASS(::Engine::PlatformSpecific::Windows::WindowManager, ::Engine::WindowManager) {}

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

	class Window final:public ::Engine::Window {
		REFLECTION_CLASS(::Engine::PlatformSpecific::Windows::Window, ::Engine::Window) {
			REFLECTION_SIGNAL(STRL("KeyDown"), { SIGARG(STRL("keyCode"),Variant::Type::Int64) });
			REFLECTION_SIGNAL(STRL("PrepareRender"), {});
			REFLECTION_SIGNAL(STRL("Render"), {});
			REFLECTION_SIGNAL(STRL("CleanupRender"), {});
		}

	public:
		~Window();

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

		void OnRender() override;

		HGLRC InitRender(HWND hWnd);
		void PrepareRender();
		void DoRender();
		void CleanupRender();
		void UninitRender(HWND hWnd,HGLRC hGLRC);

		HWND GetHWnd() const;

		DWORD GetStyle() const;
		DWORD GetExStyle() const;
		bool HasStyleFlag(DWORD style) const;
		bool SetStyleFlag(DWORD style, bool enabled);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static inline constexpr const WCHAR* GlobalWindowClassName = L"EngineBasicWindowClass";
		static inline constexpr DWORD DefaultWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;
		static inline constexpr DWORD DefaultWindowExStyle = WS_EX_ACCEPTFILES;
		static inline constexpr int32 DefaultSizeX = 640;
		static inline constexpr int32 DefaultSizeY = 480;

		static Window* GetFromHWnd(HWND hWnd);

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
