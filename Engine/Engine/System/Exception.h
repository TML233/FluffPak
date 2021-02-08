#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"

namespace Engine {
	// Base exception.
	class Exception:public Object {
	public:
		Exception();
		explicit Exception(const String& message);
		Exception& operator=(const Exception& obj);
		const String& GetMessage() const;
		virtual ~Exception();
		virtual String ToString() const override;
	protected:
		String* message;
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
}