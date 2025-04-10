#include "DatabaseConnecter.h"
#include "DebugLogging.h"

#include <psp2/sysmodule.h>

DatabaseConnecter::DatabaseConnecter(const std::string& path) : dbPath(path), db(nullptr) {
    sceSysmoduleLoadModule(SCE_SYSMODULE_SQLITE);
    sqlite3_rw_init();
    if (Open()) {
        logToFile("Database Connection established.");
    } else {
        logToFile("Failed to open database.");
    }
}
DatabaseConnecter::~DatabaseConnecter() {
    Close();
    sqlite3_rw_exit();
}

bool DatabaseConnecter::Open() {
    SceUID fd = sceIoOpen(dbPath.c_str(), SCE_O_RDWR | SCE_O_CREAT, 0777);
    if (fd > 0) {
        sceIoClose(fd);
        if (sqlite3_open_v2(dbPath.c_str(), &db,SQLITE_OPEN_READWRITE,NULL) != SQLITE_OK) {
            logToFile("Failed to open database.");
            return false;
        } else {
            logToFile("Database opened successfully.");
            return true;

        }
    } else {
        logToFile("Failed to create or open database file.");
        return false;
    }
}
void DatabaseConnecter::Close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        
    }
}

bool DatabaseConnecter::ExecuteQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        logToFile(("SQL error: " + std::string(errMsg)).c_str());
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<std::unordered_map<std::string, std::string>> DatabaseConnecter::FetchQueryResults(const std::string& query) {
    sqlite3_stmt* stmt;
    std::vector<std::unordered_map<std::string, std::string>> results;
    logToFile(("Executing query: " + query).c_str());
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logToFile(("Failed to prepare statement: " + std::string(sqlite3_errmsg(db))).c_str());
        return results;
    }
    //logToFile("Statement prepared successfully.");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        //logToFile("Row fetched successfully.");
        std::unordered_map<std::string, std::string> row;
        int columnCount = sqlite3_column_count(stmt);
        //logToFile(("Column count: " + std::to_string(columnCount)).c_str());
        for (int i = 0; i < columnCount; ++i) {
            //logToFile(("Column index: " + std::to_string(i)).c_str());
            logToFile(("Column name: " + std::string(sqlite3_column_name(stmt, i))).c_str());
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row[columnName] = columnValue ? columnValue : "";
            switch (sqlite3_column_type(stmt, i)) {
            case SQLITE_INTEGER:
                row[sqlite3_column_name(stmt, i)] = std::to_string(sqlite3_column_int(stmt, i));
                //logToFile(("Column int value : " + std::to_string(sqlite3_column_int(stmt, i))).c_str());
                break;
            case SQLITE_FLOAT:
                row[sqlite3_column_name(stmt, i)] = std::to_string(sqlite3_column_double(stmt, i));
                //logToFile(("Column float value: " + std::to_string(sqlite3_column_double(stmt, i))).c_str());
                break;
            case SQLITE_TEXT:
                row[sqlite3_column_name(stmt, i)] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                //logToFile(("Column text value: " + std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)))).c_str());
                break;
            case SQLITE_BLOB:
                row[sqlite3_column_name(stmt, i)] = "BLOB data"; // Handle BLOB data as needed
                //logToFile("Column blob value: blob unimplemented");
                break;
            case SQLITE_NULL:
                row[sqlite3_column_name(stmt, i)] = "NULL";
                //logToFile("Column value: NULL");
                break;      

            

            default:
                logToFile("Unknown column type.");
                break;
            }
        }
        results.push_back(row);
    }
    logToFile("Query executed successfully.");
    sqlite3_finalize(stmt);
    return results;
}


bool DatabaseConnecter::ExecutePreparedStatement(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        logToFile(("Failed to prepare statement: " + std::string(sqlite3_errmsg(db))).c_str());
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        logToFile(("Execution failed: " + std::string(sqlite3_errmsg(db))).c_str());
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

std::string DatabaseConnecter::EscapeString(const std::string& str) {
    std::string escapedStr;
    for (char c : str) {
        if (c == '\'') {
            escapedStr += "''"; // Escape single quote
        } else {
            escapedStr += c;
        }
    }
    return escapedStr;
}
