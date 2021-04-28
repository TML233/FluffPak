#include "Engine/Application/NativeWindow.h"
#include "Engine/Platform/NativeWindow.h"

namespace Engine {
	//NativeWindow::~NativeWindow() {}
	uint64 NativeWindow::GetId() const {
		return id;
	}
	void NativeWindow::Destroy() {
		manager->Destory(id);
	}
	NativeWindowManager* NativeWindow::GetManager() const {
		return manager;
	}

	NativeWindowManager::~NativeWindowManager() {}

	NativeWindow* NativeWindowManager::Create() {
		SharedPtr<PlatformSpecific::NativeWindow> platformWindow = SharedPtr<PlatformSpecific::NativeWindow>::Create();
		SharedPtr<NativeWindow> window = platformWindow.StaticCastTo<NativeWindow>();

		window->id = idCounter.FetchAdd(1);
		window->manager = this;

		windows.Add(window->id, window);

		bool succeeded = window->Initialize();
		if (!succeeded) {
			ERR_MSG("Failed to initialize a NativeWindow!");
			window->Destroy();
			return nullptr;
		} else {
			return window.GetRaw();
		}
	}
	bool NativeWindowManager::IsExists(NativeWindow::ID id) const {
		return windows.ContainsKey(id);
	}
	bool NativeWindowManager::Destory(NativeWindow::ID id) {
		ERR_ASSERT(IsExists(id), "Specified window id not found!", return false);

		windows.Remove(id);
		return true;
	}
	NativeWindow* NativeWindowManager::Get(NativeWindow::ID id) const {
		if (!IsExists(id)) {
			return nullptr;
		}
		return windows.Get(id).GetRaw();
	}
	int32 NativeWindowManager::GetCount() const {
		return windows.GetCount();
	}
	void NativeWindowManager::Clear() {
		windows.Clear();
	}
}
