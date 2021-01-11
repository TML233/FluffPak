#include "Core/System/Exception.h"
#include <iostream>
#include "Core/System/String.h"

namespace Core {
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

	ArgumentException::ArgumentException(const String& argumentName, const String& message) :Exception("Argument "+argumentName+": "+message) {}

	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const String& argumentName, const String& message) : ArgumentException(argumentName, message) {}
}