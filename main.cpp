#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <openssl/md5.h>
#include <ctime> // Add this line
#include <chrono> 
using namespace std;

// a struct that represents file information:
// - filename
// - file contents hash
// - last modified date
struct FileInfo
{
	string filename;
	string hash;
	string lastModifiedDate;
};

string getFileContentsHash(const string& filePath);
string convertFileTimeToString(const filesystem::file_time_type& fileTime);

vector<FileInfo> listFiles()
{
	vector<FileInfo> files;
	for (auto& entry : filesystem::recursive_directory_iterator("."))
	{
		if (!entry.is_regular_file()) 
		{
            continue; // Skip directories
        }
		FileInfo info;
		info.filename = filesystem::relative(entry.path()).string();
		info.lastModifiedDate = convertFileTimeToString(entry.last_write_time());
		info.hash = getFileContentsHash(entry.path().string());
		files.push_back(info);
	}
	return files;
}

std::string getFileContentsHash(const std::string& filePath)
{
    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    // Read the file into a string
    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Create a buffer to hold the md5 hash
    unsigned char result[MD5_DIGEST_LENGTH];

    // Calculate the md5 hash of the file contents
    MD5((unsigned char*)fileContents.c_str(), fileContents.size(), result);

    // Convert the hash to a string
    std::string md5String;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        md5String += std::to_string(result[i]);
    }

    return md5String;
}
template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}
string convertFileTimeToString(const filesystem::file_time_type& fileTime)
{
	std::time_t tt = to_time_t(fileTime);
	// Convert the last modified time to a time_point
	std::string lastModifiedTimeString = std::ctime(&tt);

	// Remove the newline character from the end of the string
	lastModifiedTimeString.pop_back();

	return lastModifiedTimeString;
}
int main()
{
	vector<FileInfo> files = listFiles();
	for (auto& file : files)
	{
		cout << file.filename << " " << file.lastModifiedDate << endl;
	}
	return 0;
}
