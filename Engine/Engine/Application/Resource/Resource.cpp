#include "Engine/Application/Resource/Resource.h"

namespace Engine {
	bool ResourceFileHandler::CanHandle(const String& extension) const {
		for (const auto& v : extensions) {
			if (v == extension) {
				return true;
			}
		}
		return false;
	}
	int32 ResourceFileHandler::GetSupportedExtensions(List<String>& result) const {
		for (const auto& v : extensions) {
			result.Add(v);
		}
		return extensions.GetCount();
	}
}