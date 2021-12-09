#include "doctest.h"
#include "Engine/System/File/FileSystem.h"

using namespace Engine;

TEST_SUITE("File System") {
	TEST_CASE("Native") {
		FileSystem fs;
		String path = STRL("file://NativeFileTest.txt");
		{
			IntrusivePtr<FileStream> file;
			auto result = fs.OpenFile(path, FileStream::OpenMode::WriteTruncate, file);
			CHECK(result == ResultCode::OK);

			CHECK(file->CanWrite());
			CHECK(!file->CanRead());
			file->WriteText(STRL("我是伞兵！！"));
			file->Close();
		}
		CHECK(fs.IsFileExists(path));
	}
}
