#pragma once
#include "Engine/Application/Engine.h"
#include "Engine/System/File/FileSystem.h"
#include "glad/glad.h"

class GLShader final :public ::Engine::ReferencedObject {
	REFLECTION_CLASS(::GLShader, ::Engine::ReferencedObject) {}
public:
	~GLShader();
	::Engine::ResultCode Open(const ::Engine::String& vertexPath, const ::Engine::String& fragmentPath);
	::Engine::uint32 GetId() const;

private:
	::Engine::uint32 id = 0;

	enum class Type {
		Vertex,
		Fragment
	};
	::Engine::ResultCode OpenOne(const ::Engine::String& path, Type type,::Engine::uint32& result);
};
