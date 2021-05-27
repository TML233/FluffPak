#include "doctest.h"
#include "Engine/System/File/FileSystem.h"

using namespace Engine;

TEST_SUITE("File System") {
	TEST_CASE("Native") {
		FileSystem fs;
		auto r=fs.OpenFile(STRL("test.txt"), FileStream::OpenMode::WriteTruncate);
		CHECK(r.result == ResultCode::OK);
		auto file = r.value;
		file->WriteInt32(3);
		file->Close();
	}
}
