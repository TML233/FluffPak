#ifndef HEADER_CORE_SYSTEM_EXCEPTIONS
#define HEADER_CORE_SYSTEM_EXCEPTIONS

#include "Core/Definition.h"
#include "Core/System/Object.h"

namespace Core {
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
}

#endif