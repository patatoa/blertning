#include <iostream>
#include <vector>
#include "FileInfo.h"
#include "SqliteDataAccess.h"

int main()
{
	std::vector<FileInfo> files = listFiles();
	saveToDatabase(files);
	for (auto& file : files)
	{
		std::cout << file.filename << " " << file.lastModifiedDate << std::endl;
	}
	return 0;
}
