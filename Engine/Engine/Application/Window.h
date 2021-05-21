#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/String.h"
#include "Engine/System/Math/Vector2.h"
#include "Engine/System/Collection/Dictionary.h"
#include "Engine/System/Thread/Atomic.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Thread/ThreadUtil.h"

namespace Engine{
	class WindowManager;

	/// @brief A native window of the platform.
	class Window: public ManualObject {
		REFLECTION_CLASS(::Engine::Window, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		using ID = uint64;
		static inline constexpr ID NullId = 0;

		/// @brief Deinitialize the native window here.\n
		/// The dtor ensures that the deinitialization is definitely executed.
		virtual ~Window() = default;

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
		virtual bool HasCloseButton() const = 0;
		virtual bool SetCloseButton(bool enabled) = 0;
		virtual bool HasMaximizeButton() const = 0;
		virtual bool SetMaximizeButton(bool enabled) = 0;
		virtual bool HasMinimizeButton() const = 0;
		virtual bool SetMinimizeButton(bool enabled) = 0;

		virtual bool HasBorder() const = 0;
		virtual bool SetBorder(bool enabled) = 0;
		virtual bool IsResizable() const = 0;
		virtual bool SetResizable(bool resizable) = 0;

		ID GetId() const;
		WindowManager* GetManager() const;

		void* GetRenderContext() const;

		virtual void OnRender() = 0;

	protected:
		virtual bool Initialize() = 0;
		void* renderContext = nullptr;

	private:
		ID id = -1;
		WindowManager* manager = nullptr;
		friend class WindowManager;
	};

	/// @brief Controls the native window of the platform.
	class WindowManager :public ManualObject {
		REFLECTION_CLASS(::Engine::WindowManager, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		virtual ~WindowManager();
		/// @brief Create a native window with default style, and hidden.
		/// @return nullptr when failed.
		Window* Create();
		/// @brief Get a native window by ID.
		/// @return nullptr when not found.
		Window* Get(Window::ID id) const;
		/// @brief Get the count of current existing native windows.
		int32 GetCount() const;
		/// @brief Check if the native window exists.
		bool IsExists(Window::ID id) const;
		/// @brief Destroy a native window.
		bool Destroy(Window::ID id);
		/// @brief Destroy all native windows.
		void Clear();

		void RenderAll();

		virtual void Update() = 0;

	private:
		AtomicValue<Window::ID> idCounter{ 1 };
		Dictionary<Window::ID, SharedPtr<Window>> windows;
		mutable Mutex windowsMutex;
	};
}