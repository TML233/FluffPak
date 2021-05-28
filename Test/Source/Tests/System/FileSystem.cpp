#include "doctest.h"
#include "Engine/System/File/FileSystem.h"

using namespace Engine;

TEST_SUITE("File System") {
	TEST_CASE("Native") {
		FileSystem fs;
		String path = STRL("file://NativeFileTest.txt");
		{
			auto r = fs.OpenFile(path, FileStream::OpenMode::WriteTruncate);
			CHECK(r.result == ResultCode::OK);

			auto file = r.value;
			CHECK(file->CanWrite());
			CHECK(!file->CanRead());
			file->WriteText(STRL("我是伞兵！！"));
			file->Close();
		}
		CHECK(fs.IsFileExists(path));
	}
}
