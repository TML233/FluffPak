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
	Atom NativeWindowManager::GetInternAtom(const char* name) {
		Atom r;
		if (internAtomCache.TryGet(name, r)) {
			return r;
		}
		r = XInternAtom(display, name, False);
		internAtomCache.Set(name, r);
		return r;
	}
	void NativeWindowManager::SetWindowToNative(Window window, NativeWindow* native) {
		if (native == nullptr) {
			windowToNative.Remove(window);
		} else {
			windowToNative.Set(window, native);
		}
	}
	NativeWindow* NativeWindowManager::GetNativeFromWindow(Window window) {
		NativeWindow* p = nullptr;
		windowToNative.TryGet(window, p);
		return p;
	}
	NativeWindowManager::~NativeWindowManager() {
		XCloseDisplay(display);
    }
	void NativeWindowManager::Update(){
		XNextEvent(display,&event);
		switch (event.type) {
			case Expose:
				XFillRectangle(display, event.xclient.window, DefaultGC(display, GetDefaultScreen()), 20, 20, 10, 10);
				break;
			case ClientMessage:
			{
				if (event.xclient.data.l[0] == GetInternAtom("WM_DELETE_WINDOW")) {
					NativeWindow* nw = GetNativeFromWindow(event.xclient.window);
					if (nw == nullptr) {
						ERR_MSG("Event ClientMessage, GetNativeFromWindow returns nullptr! This shouldn't happen!");
					} else {
						nw->Destroy();
					}
				}
			}
			break;
		}
	}

	bool NativeWindow::Initialize() {
		NativeWindowManager* manager = GetDetailedManager();

		Display* display = GetDisplay();
		int screen = manager->GetDefaultScreen();
		
		window=XCreateSimpleWindow(display,RootWindow(display,screen),100,100,100,100,1,BlackPixel(display,screen),WhitePixel(display,screen));
		
		XSelectInput(display,window,ExposureMask|KeyPressMask);
		XMapWindow(display,window);

		Atom WM_DELETE_WINDOW = manager->GetInternAtom("WM_DELETE_WINDOW");
		XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

		manager->SetWindowToNative(window, this);

		return true;
	}
	NativeWindow::~NativeWindow() {
		GetDetailedManager()->SetWindowToNative(window, nullptr);
		XDestroyWindow(GetDisplay(), window);
	}
	NativeWindowManager* NativeWindow::GetDetailedManager() const{
		return (NativeWindowManager*)GetManager();
	}
	Display* NativeWindow::GetDisplay() const {
		return GetDetailedManager()->GetDisplay();
	}
	XWindowAttributes NativeWindow::GetAttributes() const {
		XWindowAttributes attr{};
		XGetWindowAttributes(GetDisplay(), window, &attr);
		return attr;
	}

	bool NativeWindow::IsValid() const {
		return true;
	}

	String NativeWindow::GetTitle() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return String::GetEmpty());
		char* name = nullptr;
		XFetchName(GetDisplay(), window, &name);
		if (name == nullptr) {
			return String::GetEmpty();
		} else {
			String r{ name };
			XFree(name);
			return r;
		}
	}
	bool NativeWindow::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		
		XStoreName(GetDisplay(), window, title.ToIndividual().GetRawArray());

		return true;
	}

	Vector2 NativeWindow::GetPosition() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		XWindowAttributes attr = GetAttributes();
		return Vector2(attr.x, attr.y);
	}
	bool NativeWindow::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		XMoveWindow(GetDisplay(), window, (int)position.x, (int)position.y);

		return true;
	}
	Vector2 NativeWindow::GetSize() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		XWindowAttributes attr = GetAttributes();
		return Vector2(attr.width, attr.height);
	}
	bool NativeWindow::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		XResizeWindow(GetDetailedManager()->GetDisplay(), window, (unsigned int)size.x, (unsigned int)size.y);

		return true;
	}

	bool NativeWindow::IsVisible() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return GetAttributes().map_state != IsUnmapped;
	}
	bool NativeWindow::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		if (IsVisible()&&!visible) {
			XUnmapWindow(GetDisplay(), window);
		} else if (!IsVisible() && visible) {
			XMapWindow(GetDisplay(), window);
		}
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