#include "Engine/Application/Window.h"
#include "Engine/Platform/Window.h"

namespace Engine {
	//Window::~Window() {}
	Window::ID Window::GetId() const {
		return id;
	}
	WindowSystem* Window::GetManager() const {
		return manager;
	}

	WindowSystem::~WindowSystem() {}

	Window* WindowSystem::CreateWindow() {
		SharedPtr<Window> window = SharedPtr<PLATFORM_SPECIFIC_CLASS_WINDOW>::Create();

		window->id = idCounter.Add(1);
		window->manager = this;

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Add(window->id, window);
		}

		if (!window->IsValid()) {
			ERR_MSG(u8"Failed to initialize a Window!");
			DestroyWindow(window->GetId());
			return nullptr;
		} else {
			return window.GetRaw();
		}
	}
	bool WindowSystem::IsWindowExists(Window::ID id) const {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		return windows.ContainsKey(id);
	}
	bool WindowSystem::DestroyWindow(Window::ID id) {
		ERR_ASSERT(IsWindowExists(id), u8"Specified window id not found!", return false);

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Remove(id);
		}
		return true;
	}
	Window* WindowSystem::GetWindow(Window::ID id) const {
		SharedPtr<Window> ptr;
		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.TryGet(id, ptr);
		}
		return ptr.GetRaw();
	}
	int32 WindowSystem::GetWindowCount() const {
		return windows.GetCount();
	}
	void WindowSystem::DestroyAllWindows() {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		windows.Clear();
	}
}
