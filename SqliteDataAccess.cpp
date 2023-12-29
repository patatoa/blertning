#include "SqliteDataAccess.h"
void saveToDatabase(const std::vector<FileInfo>& files)
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