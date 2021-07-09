#include "Engine/Platform/Linux/Window.h"

namespace Engine::PlatformSpecific::Linux {
	typename WindowSystem::_Initializer WindowSystem::_initializer{};

	WindowSystem::_Initializer::_Initializer() {
		bool inited = gtk_init_check(0, NULL);
		FATAL_ASSERT(inited, u8"Cannot initialize window framework!");
	}

	WindowSystem::_Initializer::~_Initializer() {
		
	}

	void WindowSystem::Update(){
		gtk_main_iteration_do(false);
	}

	Window::Window() {
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		g_object_set_data(G_OBJECT(window), "WindowPtr", this);
		g_signal_connect(window, "delete-event",G_CALLBACK(OnGtkCloseWindow), NULL);
	}
	Window::~Window() {
		if (IsValid) {
			gtk_widget_destroy(window);
		}
	}
	WindowSystem* Window::GetDetailedManager() const{
		return (WindowSystem*)GetManager();
	}
	
	bool Window::IsValid() const {
		return GTK_IS_WIDGET(window);
	}

	String Window::GetTitle() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return String::GetEmpty());
		
		const gchar* title=gtk_window_get_title(GTK_WINDOW(window));

		if (title != NULL) {
			return String(title);
		} else {
			return String::GetEmpty();
		}
	}
	bool Window::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		gtk_window_set_title(GTK_WINDOW(window), reinterpret_cast<const char*>(title.ToIndividual().GetRawArray()));
		return true;
	}

	Vector2 Window::GetPosition() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		gint x = 0;
		gint y = 0;
		gtk_window_get_position(GTK_WINDOW(window), &x, &y);

		return Vector2(x, y);
	}
	bool Window::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		return true;
	}
	Vector2 Window::GetSize() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		int w = 0;
		int h = 0;
		gtk_window_get_size(GTK_WINDOW(window), &w, &h);
	
		return Vector2(w, h);
	}
	bool Window::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		gtk_window_resize(GTK_WINDOW(window), (gint)size.x, (gint)size.y);
		return true;
	}

	bool Window::IsVisible() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		return gtk_widget_is_visible(window);
	}
	bool Window::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		if (visible) {
			gtk_widget_show(window);
		} else {
			gtk_widget_hide(window);
		}
		return true;
	}
	
	bool Window::IsMinimized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		WARN_MSG(u8"This function is not available for Linux. Will always return false.");
		return false;
	}
	bool Window::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		if (minimized) {
			gtk_window_iconify(GTK_WINDOW(window));
		} else {
			gtk_window_deiconify(GTK_WINDOW(window));
		}
		return true;
	}
	bool Window::IsMaximized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		return gtk_window_is_maximized(GTK_WINDOW(window));
	}
	bool Window::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		if (maximized) {
			gtk_window_maximize(GTK_WINDOW(window));
		} else {
			gtk_window_unmaximize(GTK_WINDOW(window));
		}
		return true;
	}
	bool Window::HasCloseButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		return gtk_window_get_deletable(GTK_WINDOW(window));
	}
	bool Window::SetCloseButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		gtk_window_set_deletable(GTK_WINDOW(window), enabled);
		return true;
	}
	bool Window::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		WARN_MSG(u8"This function is not available for Linux. Will always return true.");
		return true;
	}
	bool Window::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		WARN_MSG(u8"This function is not available for Linux. Will always return false.");
		return false;
	}
	bool Window::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		return true;
	}
	bool Window::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		return true;
	}

	bool Window::HasBorder() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		return gtk_window_get_decorated(GTK_WINDOW(window));
	}
	bool Window::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		gtk_window_set_decorated(GTK_WINDOW(window), enabled);
		return true;
	}
	bool Window::IsResizable() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		return gtk_window_get_resizable(GTK_WINDOW(window));
	}
	bool Window::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		gtk_window_set_resizable(GTK_WINDOW(window), resizable);
		return true;
	}

	void Window::OnCallbackClose() {
		GetManager()->Destroy(GetId());
	}
	gboolean Window::OnGtkCloseWindow(GtkWidget* widget, GdkEvent* event, gpointer data){
		Window* nw = (Window*)g_object_get_data(G_OBJECT(widget), "WindowPtr");
		if (nw == nullptr) {
			ERR_MSG(u8"Window ptr in GTK object data is nullptr! This shouldn't happen!");
		} else {
			nw->OnCallbackClose();
		}
		return TRUE;
	}
}