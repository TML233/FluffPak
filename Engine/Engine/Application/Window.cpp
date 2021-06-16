#include "Engine/Application/Window.h"
#include "Engine/Platform/Window.h"

namespace Engine {
	//Window::~Window() {}
	Window::ID Window::GetId() const {
		return id;
	}
	WindowManager* Window::GetManager() const {
		return manager;
	}

	void* Window::GetRenderContext() const {
		return renderContext;
	}

	WindowManager::~WindowManager() {}

	Window* WindowManager::Create() {
		SharedPtr<Window> window = SharedPtr<PLATFORM_SPECIFIC_CLASS_WINDOW>::Create();

		window->id = idCounter.Add(1);
		window->manager = this;

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Add(window->id, window);
		}

		if (!window->IsValid()) {
			ERR_MSG(u8"Failed to initialize a Window!");
			Destroy(window->GetId());
			return nullptr;
		} else {
			return window.GetRaw();
		}
	}
	bool WindowManager::IsExists(Window::ID id) const {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		return windows.ContainsKey(id);
	}
	bool WindowManager::Destroy(Window::ID id) {
		ERR_ASSERT(IsExists(id), u8"Specified window id not found!", return false);

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Remove(id);
		}
		return true;
	}
	Window* WindowManager::Get(Window::ID id) const {
		SharedPtr<Window> ptr;
		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.TryGet(id, ptr);
		}
		return ptr.GetRaw();
	}
	int32 WindowManager::GetCount() const {
		return windows.GetCount();
	}
	void WindowManager::Clear() {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		windows.Clear();
	}

	struct _NWRender {
		Window* window;
	};

	void WindowManager::RenderAll() {
		for (const auto& w : windows) {
			Window* ptr = w.value.GetRaw();
			w.value->OnRender();
		}
	}
}
