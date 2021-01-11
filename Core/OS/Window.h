#ifndef HEADER_CORE_OS_WINDOW
#define HEADER_CORE_OS_WINDOW

#include "Core/Definition.h"
#include "Core/System/Object.h"
#include "Core/Math/Vector2.h"
#include "Core/Template/Dictionary.h"
#include <iostream>

namespace Core {
	class Window :public Object {
	public:
		// Create a window
		static std::shared_ptr<Window> Create(const String& title, const Vector2& position, const Vector2& size);

		virtual ~Window() = 0;

		virtual void Destroy() = 0;

		virtual void SetVisible(Bool visible) = 0;
		virtual Bool IsVisible() const = 0;
		virtual String GetTitle() const = 0;
		virtual void SetTitle(const String& title) = 0;
		virtual Vector2 GetSize() const = 0;
		virtual void SetSize(const Vector2& size) = 0;
		virtual Vector2 GetPosition() const = 0;
		virtual void SetPosition(const Vector2& position) = 0;
		virtual Vector2 GetRawPosition() const = 0;
		virtual void SetRawPosition(const Vector2& position) = 0;
	};

	class WindowManager {
	public:
		virtual ~WindowManager() = 0;

		virtual std::shared_ptr<Window> Create(const String& id, const String& title, const Vector2& position, const Vector2& size) = 0;
		virtual Bool Destroy(const String& id) = 0;

		static WindowManager& GetInstance();
	protected:
		WindowManager();
		Dictionary<String, std::shared_ptr<Window>> windows;
	};
}

#endif