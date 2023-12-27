#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <openssl/md5.h>
#include <ctime> 
#include <chrono> 
#include <sqlite3.h>
using namespace std;

struct FileInfo
{
	string filename;
	string hash;
	string lastModifiedDate;
};

string getFileContentsHash(const string& filePath);
string convertFileTimeToString(const filesystem::file_time_type& fileTime);
void saveToDatabase(const vector<FileInfo>& files);
void createSqliteDatabase();

vector<FileInfo> listFiles()
{
	vector<FileInfo> files;
	for (auto& entry : filesystem::recursive_directory_iterator("."))
	{
		if (!entry.is_regular_file()) {
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

void saveToDatabase(const vector<FileInfo>& files)
{
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
	bool dbConnected = false;

	while(!dbConnected)
	{
		// Open database
		rc = sqlite3_open("test.db", &db);

		if (rc) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return;
		}
		else
		{
			dbConnected = true;
		}
	}
    // Open database
    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Create SQL statement to check if table exists
    char* sql = "CREATE TABLE IF NOT EXISTS FileInfo("  \
                "Filename TEXT PRIMARY KEY NOT NULL," \
                "LastModifiedDate TEXT NOT NULL," \
                "Hash TEXT NOT NULL);";

    // Execute SQL statement
	auto callback = [](void* NotUsed, int argc, char** argv, char** azColName) -> int { return 0; };
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    // Insert FileInfo results into the table
    for (const auto& file : files) {
        std::string sql = "INSERT INTO FileInfo (Filename,LastModifiedDate,Hash) " \
                          "VALUES ('" + file.filename + "', '" + file.lastModifiedDate + "', '" + file.hash + "');";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }
    }

    sqlite3_close(db);
}
int main()
{
	vector<FileInfo> files = listFiles();
	saveToDatabase(files);
	for (auto& file : files)
	{
		cout << file.filename << " " << file.lastModifiedDate << endl;
	}
	return 0;
}
