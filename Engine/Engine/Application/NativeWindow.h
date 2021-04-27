#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/SharedPtr.h"

namespace Engine{
	class NativeWindowManager;
	class NativeWindow {
	public:
		using ID = uint64;
		const static inline ID NullId = 0;

		virtual ~NativeWindow();

		virtual bool IsValid() const = 0;

		virtual String GetTitle() const = 0;
		virtual bool SetTitle(const String& title) = 0;
		virtual Vector2 GetPosition() const = 0;
		virtual bool SetPosition(const Vector2& position) = 0;
		virtual Vector2 GetSize() const = 0;
		virtual bool SetSize(const Vector2& size) = 0;

		virtual bool IsVisible() const = 0;
		virtual bool SetVisible(bool visible) = 0;

		virtual bool IsMinimized() const = 0;
		virtual bool SetMinimized(bool minimized) = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool SetMaximized(bool maximized) = 0;
		virtual bool HasMaximizeButton() const = 0;
		virtual bool SetMaximizeButton(bool enabled) = 0;
		virtual bool HasMinimizeButton() const = 0;
		virtual bool SetMinimizeButton(bool enabled) = 0;

		virtual bool HasBorder() const = 0;
		virtual bool SetBorder(bool enabled) = 0;
		virtual bool IsResizable() const = 0;
		virtual bool SetResizable(bool resizable) = 0;

		void Destroy();
		ID GetId() const;
		NativeWindowManager* GetManager() const;

	private:
		ID id = -1;
		NativeWindowManager* manager = nullptr;
		friend class NativeWindowManager;
	};

	class NativeWindowManager {
	public:
		virtual ~NativeWindowManager();
		NativeWindow* Create();
		NativeWindow* Get(NativeWindow::ID id) const;
		int32 GetCount() const;
		bool IsExists(NativeWindow::ID id) const;
		bool Destory(NativeWindow::ID id);
		void Clear();
		virtual void Update() = 0;

	private:
		AtomicValue<NativeWindow::ID> idCounter{ 1 };
		Dictionary<NativeWindow::ID, SharedPtr<NativeWindow>> windows;
	};
}