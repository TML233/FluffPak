#include "Engine/Platform/Linux/X11/NativeWindow.h"

namespace Engine::PlatformSpecific {
	NativeWindowManager::NativeWindowManager() {
		display=XOpenDisplay(NULL);
		FATAL_ASSERT(display!=nullptr,"XOpenDisplay failed to open a display! Cannot initialize Linux X11 window system!");
	}
	Display* NativeWindowManager::GetDisplay() const{
		return display;
	}
	int NativeWindowManager::GetDefaultScreen() const{
		return DefaultScreen(display);
	}
	NativeWindowManager::~NativeWindowManager() {
		XCloseDisplay(display);
    }
	void NativeWindowManager::Update(){
		XNextEvent(display,&event);
	}

	NativeWindow::NativeWindow() {
		Display* display=GetDetailedManager()->GetDisplay();
		int screen=GetDetailedManager()->GetDefaultScreen();
		window=XCreateSimpleWindow(display,RootWindow(display,screen),100,100,100,100,1,BlackPixel(display,screen),WhitePixel(display,screen));
		XSelectInput(display,window,ExposureMask|KeyPressMask);
		XMapWindow(display,window);
	}
	NativeWindowManager* NativeWindow::GetDetailedManager() const{
		return (NativeWindowManager*)GetManager();
	}

	bool NativeWindow::IsValid() const {
		return true;
	}

	String NativeWindow::GetTitle() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return String::GetEmpty());

		return String::GetEmpty();
	}
	bool NativeWindow::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return true;
	}

	Vector2 NativeWindow::GetPosition() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		return Vector2();
	}
	bool NativeWindow::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return true;
	}
	Vector2 NativeWindow::GetSize() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		return Vector2();
	}
	bool NativeWindow::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return true;
	}

	bool NativeWindow::IsVisible() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	
	bool NativeWindow::IsMinimized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::IsMaximized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}

	bool NativeWindow::HasBorder() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::IsResizable() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
	bool NativeWindow::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return true;
	}
}