#include <experimental/filesystem>
#include <vector>
#include <fstream>
#include <openssl/md5.h>
#include <ctime> 
#include <chrono> 

#ifndef FILEINFO_H
#define FILEINFO_H

struct FileInfo
{
	std::string filename;
	std::string hash;
	std::string lastModifiedDate;
};

std::vector<FileInfo> listFiles();
std::string getFileContentsHash(const std::string& filePath);
std::string convertFileTimeToString(const std::experimental::filesystem::file_time_type& fileTime);

#endif