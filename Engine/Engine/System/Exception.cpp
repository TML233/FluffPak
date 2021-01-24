#include "Engine/System/Exception.h"
#include "Engine/System/String.h"

namespace Engine {
	Exception::Exception() {
		message = new String;
	}
	Exception::Exception(const String& message) {
		this->message = new String(message);
	}
	Exception& Exception::operator=(const Exception& obj) {
		*message = *(obj.message);
		return *this;
	}
	Exception::~Exception() {
		delete message;
	}
	const String& Exception::GetMessage() const {
		return *message;
	}

	String Exception::ToString() const {
		return GetMessage();
	}

	ArgumentException::ArgumentException(const String& argumentName, const String& message) : Exception(String::Format("Argument {0}: {1}", argumentName, message)) {}
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& argumentName, const String& message) : ArgumentException(argumentName, message) {}
	NotImplementedException::NotImplementedException(const String& methodName, const String& message) : Exception(String::Format("Method {0} is not implemented: {1}", methodName, message)) {}
	KeyNotFoundException::KeyNotFoundException(const String& keyName) : Exception(String::Format("Key not found: {0}", keyName)) {}
}