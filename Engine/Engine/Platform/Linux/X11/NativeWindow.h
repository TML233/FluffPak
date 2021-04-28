#pragma once
#include "Engine/Application/NativeWindow.h"
#include <X11/Xlib.h>

namespace Engine::PlatformSpecific{
	class NativeWindow;

	class NativeWindowManager final :public ::Engine::NativeWindowManager {
	public:
		NativeWindowManager();
		~NativeWindowManager();
		
		int GetDefaultScreen() const;
		Display* GetDisplay() const;
		Atom GetInternAtom(const char* name);
		void SetWindowToNative(Window window, NativeWindow* native);
		NativeWindow* GetNativeFromWindow(Window window);

		void Update() override;

	private:
		Display* display = nullptr;
		int screen = 0;
		XEvent event{};
		Dictionary<const void*, Atom> internAtomCache{};
		Dictionary<Window, NativeWindow*> windowToNative{};
	};
	class NativeWindow final :public ::Engine::NativeWindow {
	public:
		~NativeWindow();

		NativeWindowManager* GetDetailedManager() const;
		Display* GetDisplay() const;

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
		bool HasMaximizeButton() const override;
		bool SetMaximizeButton(bool enabled) override;
		bool HasMinimizeButton() const override;
		bool SetMinimizeButton(bool enabled) override;

		bool HasBorder() const override;
		bool SetBorder(bool enabled) override;
		bool IsResizable() const override;
		bool SetResizable(bool resizable) override;

	protected:
		bool Initialize() override;
		XWindowAttributes GetAttributes() const;

	private:
		Window window{};
	};
}