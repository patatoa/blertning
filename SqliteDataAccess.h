#include <vector>
#include <sqlite3.h>
#include "FileInfo.h"

void saveToDatabase(const std::vector<FileInfo>& files);
void createSqliteDatabase();