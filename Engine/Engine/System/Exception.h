#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"
#include "Engine/System/String.h"

namespace Engine {
	// Base exception.
	class Exception:public Object {
	public:
		explicit Exception(const String& message);
		const String GetMessage() const;

		virtual String ToString() const override;
	protected:
		String message;
	};

	class ArgumentException :public Exception {
	public:
		ArgumentException(const String& argumentName, const String& message);
	};

	class ArgumentOutOfRangeException :public ArgumentException {
	public:
		ArgumentOutOfRangeException(const String& argumentName, const String& message);
	};

	class NotImplementedException :public Exception {
	public:
		NotImplementedException(const String& methodName, const String& message);
	};

	class KeyNotFoundException :public Exception {
	public:
		KeyNotFoundException(const String& keyName);
	};

	class OverflowException :public Exception {
	public:
		OverflowException(const String& message);
	};

	class OutOfMemoryException :public Exception {
	public:
		OutOfMemoryException(const String& message);
	};
}