#include "Engine/Platform/Linux/NativeWindow.h"

namespace Engine::PlatformSpecific {
	typename NativeWindowManager::_Initializer NativeWindowManager::_initializer{};

	NativeWindowManager::_Initializer::_Initializer() {
		bool inited = gtk_init_check(0, NULL);
		FATAL_ASSERT(inited, "gtk_init_check returned false. Cannot initialize window framework!");
	}

	NativeWindowManager::_Initializer::~_Initializer() {
		
	}

	void NativeWindowManager::Update(){
		gtk_main_iteration_do(false);
	}

	bool NativeWindow::Initialize() {
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		g_object_set_data(G_OBJECT(window), "NativeWindowPtr", this);
		g_signal_connect(window, "delete-event",G_CALLBACK(OnGtkCloseWindow), NULL);
		
		return true;
	}
	NativeWindow::~NativeWindow() {
		
	}
	NativeWindowManager* NativeWindow::GetDetailedManager() const{
		return (NativeWindowManager*)GetManager();
	}
	
	bool NativeWindow::IsValid() const {
		return GTK_IS_WIDGET(window);
	}

	String NativeWindow::GetTitle() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return String::GetEmpty());
		
		const gchar* title=gtk_window_get_title(GTK_WINDOW(window));

		if (title != NULL) {
			return String(title);
		} else {
			return String::GetEmpty();
		}
	}
	bool NativeWindow::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		
		gtk_window_set_title(GTK_WINDOW(window), title.ToIndividual().GetRawArray());
		return true;
	}

	Vector2 NativeWindow::GetPosition() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		gint x = 0;
		gint y = 0;
		gtk_window_get_position(GTK_WINDOW(window), &x, &y);

		return Vector2(x, y);
	}
	bool NativeWindow::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		
		return true;
	}
	Vector2 NativeWindow::GetSize() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return Vector2());

		int w = 0;
		int h = 0;
		gtk_window_get_size(GTK_WINDOW(window), &w, &h);
	
		return Vector2(w, h);
	}
	bool NativeWindow::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		
		gtk_window_resize(GTK_WINDOW(window), (gint)size.x, (gint)size.y);
		return true;
	}

	bool NativeWindow::IsVisible() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return gtk_widget_is_visible(window);
	}
	bool NativeWindow::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		if (visible) {
			gtk_widget_show(window);
		} else {
			gtk_widget_hide(window);
		}
		return true;
	}
	
	bool NativeWindow::IsMinimized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		WARN_MSG("This function is not available for Linux. Will always return false.");
		return false;
	}
	bool NativeWindow::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		
		if (minimized) {
			gtk_window_iconify(GTK_WINDOW(window));
		} else {
			gtk_window_deiconify(GTK_WINDOW(window));
		}
		return true;
	}
	bool NativeWindow::IsMaximized() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		return gtk_window_is_maximized(GTK_WINDOW(window));
	}
	bool NativeWindow::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);
		if (maximized) {
			gtk_window_maximize(GTK_WINDOW(window));
		} else {
			gtk_window_unmaximize(GTK_WINDOW(window));
		}
		return true;
	}
	bool NativeWindow::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		WARN_MSG("This function is not available for Linux. Will always return true.");
		return true;
	}
	bool NativeWindow::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		WARN_MSG("This function is not available for Linux. Will always return false.");
		return false;
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

		return gtk_window_get_decorated(GTK_WINDOW(window));
	}
	bool NativeWindow::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		gtk_window_set_decorated(GTK_WINDOW(window), enabled);
		return true;
	}
	bool NativeWindow::IsResizable() const {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		return gtk_window_get_resizable(GTK_WINDOW(window));
	}
	bool NativeWindow::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), "The window is not valid!", return false);

		gtk_window_set_resizable(GTK_WINDOW(window), resizable);
		return true;
	}

	void NativeWindow::OnCallbackClose() {
		Destroy();
	}
	gboolean NativeWindow::OnGtkCloseWindow(GtkWidget* widget, GdkEvent* event, gpointer data){
		NativeWindow* nw = (NativeWindow*)g_object_get_data(G_OBJECT(widget), "NativeWindowPtr");
		if (nw == nullptr) {
			ERR_MSG("NativeWindow ptr in GTK object data is nullptr! This shouldn't happen!");
		} else {
			nw->OnCallbackClose();
		}
		return TRUE;
	}
}