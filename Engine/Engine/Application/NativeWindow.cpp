#include "Engine/Application/NativeWindow.h"
#include "Engine/Platform/NativeWindow.h"

namespace Engine {
	//NativeWindow::~NativeWindow() {}
	uint64 NativeWindow::GetId() const {
		return id;
	}
	NativeWindowManager* NativeWindow::GetManager() const {
		return manager;
	}

	void* NativeWindow::GetRenderContext() const {
		return renderContext;
	}

	NativeWindowManager::~NativeWindowManager() {}

	NativeWindow* NativeWindowManager::Create() {
		SharedPtr<NativeWindow> window = SharedPtr<PLATFORM_SPECIFIC_CLASS_NATIVEWINDOW>::Create();

		window->id = idCounter.FetchAdd(1);
		window->manager = this;

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Add(window->id, window);
		}

		bool succeeded = window->Initialize();
		if (!succeeded) {
			ERR_MSG(u8"Failed to initialize a NativeWindow!");
			Destroy(window->GetId());
			return nullptr;
		} else {
			return window.GetRaw();
		}
	}
	bool NativeWindowManager::IsExists(NativeWindow::ID id) const {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		return windows.ContainsKey(id);
	}
	bool NativeWindowManager::Destroy(NativeWindow::ID id) {
		ERR_ASSERT(IsExists(id), u8"Specified window id not found!", return false);

		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.Remove(id);
		}
		return true;
	}
	NativeWindow* NativeWindowManager::Get(NativeWindow::ID id) const {
		SharedPtr<NativeWindow> ptr;
		{
			auto lock = SimpleLock<Mutex>(windowsMutex);
			windows.TryGet(id, ptr);
		}
		return ptr.GetRaw();
	}
	int32 NativeWindowManager::GetCount() const {
		return windows.GetCount();
	}
	void NativeWindowManager::Clear() {
		auto lock = SimpleLock<Mutex>(windowsMutex);
		windows.Clear();
	}
}
