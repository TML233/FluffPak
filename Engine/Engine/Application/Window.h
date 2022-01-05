#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/String.h"
#include "Engine/System/Math/Vector.h"
#include "Engine/System/Collection/Dictionary.h"
#include "Engine/System/Thread/Atomic.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Thread/ThreadUtil.h"

namespace Engine{
	class WindowSystem;
	using WindowID = int32;
	struct MonitorInfo {
		String name;
		Vector2 position;
		Vector2 size;
		float dpi;
	};

	/// @brief A native window of the platform.
	class Window: public ManualObject {
		REFLECTION_CLASS(::Engine::Window, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
			
			REFLECTION_SIGNAL(STRL("OnUserResizing"), ARGLIST(
				SIGARG(STRL("edge"), Variant::Type::Vector2),
				SIGARG(STRL("size"), Variant::Type::Vector2)
			));
			REFLECTION_SIGNAL(STRL("OnResized"), ARGLIST(
				SIGARG(STRL("size"), Variant::Type::Vector2)
			));
		}

	public:
		static inline constexpr WindowID NullId = -1;

		/// @brief Initialize the native window here.\n
		/// The manager will use IsValid() to check if it succeeded.
		Window() = default;
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

		WindowID GetId() const;
		WindowSystem* GetManager() const;

	private:
		WindowID id = NullId;
		WindowSystem* manager = nullptr;
		friend class WindowSystem;
	};

	/// @brief Controls the native window of the platform.
	class WindowSystem :public ManualObject {
		REFLECTION_CLASS(::Engine::WindowSystem, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		virtual ~WindowSystem();
		/// @brief Create a native window with default style, and hidden.
		/// @return nullptr when failed.
		Window* CreateWindow();
		/// @brief Get a native window by ID.
		/// @return nullptr when not found.
		Window* GetWindow(WindowID id) const;
		/// @brief Get the count of current existing native windows.
		int32 GetWindowCount() const;
		/// @brief Check if the native window exists.
		bool IsWindowExists(WindowID id) const;
		/// @brief Destroy a native window.
		bool DestroyWindow(WindowID id);
		/// @brief Destroy all native windows.
		void DestroyAllWindows();

		virtual void Update() = 0;

	private:
		AtomicValue<WindowID> idCounter{ Window::NullId };
		Dictionary<WindowID, SharedPtr<Window>> windows;
		mutable Mutex windowsMutex;
	};
}