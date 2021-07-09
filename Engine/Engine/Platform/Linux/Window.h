#pragma once
#include "Engine/Application/Window.h"
#include <gtk/gtk.h>

#define PLATFORM_SPECIFIC_CLASS_WINDOWMANAGER ::Engine::PlatformSpecific::Linux::WindowSystem
#define PLATFORM_SPECIFIC_CLASS_WINDOW ::Engine::PlatformSpecific::Linux::Window

namespace Engine::PlatformSpecific::Linux{
	class Window;

	class WindowSystem final :public ::Engine::WindowSystem {
		REFLECTION_CLASS(::Engine::PlatformSpecific::Linux::WindowSystem, ::Engine::WindowSystem) {}

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
	class Window final :public ::Engine::Window {
	public:
		~Window();

		WindowSystem* GetDetailedManager() const;
		
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
		bool HasMaximizeButton() const override;
		bool SetMaximizeButton(bool enabled) override;
		bool HasMinimizeButton() const override;
		bool SetMinimizeButton(bool enabled) override;

		bool HasBorder() const override;
		bool SetBorder(bool enabled) override;
		bool IsResizable() const override;
		bool SetResizable(bool resizable) override;


		void OnCallbackClose();
		static gboolean OnGtkCloseWindow(GtkWidget* widget,GdkEvent* event,gpointer data);
		
	private:
		GtkWidget* window;
	};
}