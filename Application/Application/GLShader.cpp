#include "Application/GLShader.h"

using namespace Engine;

GLShader::~GLShader() {
	if (id != 0) {
		glDeleteProgram(id);
	}
}

ResultCode GLShader::Open(const String& vertexPath, const String& fragmentPath) {
	uint32 vertexShader = 0;
	auto vertexR = OpenOne(vertexPath, Type::Vertex,vertexShader);
	if (vertexR != ResultCode::OK) {
		ERR_MSG(u8"Vertex shader failed!");
		return ResultCode::InvalidArgument;
	}

	uint32 fragmentShader = 0;
	auto fragmentR = OpenOne(fragmentPath, Type::Fragment,fragmentShader);
	if (fragmentR != ResultCode::OK) {
		ERR_MSG(u8"Fragment shader failed!");
		glDeleteShader(vertexShader);
		return ResultCode::InvalidArgument;
	}

	// Shader program
	uint32 shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Check if shader program is linked successfully
	int32 succeed = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succeed);
	if (!succeed) {
		char info[512];
		glGetProgramInfoLog(shaderProgram, sizeof(info), NULL, info);
		ERR_MSG(String::Format(STRL("Shader program failed to compile: {0}"), info).GetRawArray());

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		return ResultCode::UnknownError;
	}

	id = shaderProgram;
	//After linking shader program, the shader is not useful for drawing
	//Delete the shaders if they are no longer used
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

::Engine::uint32 GLShader::GetId() const {
	return id;
}

ResultCode GLShader::OpenOne(const String& path, Type type,uint32& result) {
	// Read file
	auto fs = ENGINEINST->GetFileSystem();
	::Engine::String code;
	{
		auto f = fs->OpenFile(path, ::Engine::FileStream::OpenMode::ReadOnly);
		ERR_ASSERT(f.result == ::Engine::ResultCode::OK, u8"Failed to open vertexShader file!", return ::Engine::ResultCode::NotFound);
		code = f.value->ReadAllText();
	}

	// Compile shader
	int32 shader = glCreateShader(type==Type::Vertex?GL_VERTEX_SHADER:GL_FRAGMENT_SHADER);
	const GLchar* codep = (const GLchar*)code.GetRawArray();
	glShaderSource(shader, 1, &codep, nullptr);
	glCompileShader(shader);

	// Check error
	int32 succeed = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succeed);
	if (!succeed) {
		char info[512];
		glGetShaderInfoLog(shader, sizeof(info), NULL, info);
		ERR_MSG(String::Format(STRL("Vertex shader failed to compile: {0}"), info).GetRawArray());
		glDeleteShader(shader);
		return ResultCode::InvalidArgument;
	}

	result = shader;
	return ResultCode::OK;
}
