#include "DatabaseConnecter.h"
#include "DebugLogging.h"



DatabaseConnecter::DatabaseConnecter(const std::string& path) : dbPath(path), db(nullptr) {
    if (Open()) {
        LogInfo("Database opened successfully.");
    } else {
        LogError("Failed to open database.");
    }
}
DatabaseConnecter::~DatabaseConnecter() {
    Close();
}

bool DatabaseConnecter::Open() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        LogError("Can't open database: " + std::string(sqlite3_errmsg(db)));
        return false;
    }
    return true;
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
        LogError("SQL error: " + std::string(errMsg));
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<std::unordered_map<std::string, std::string>> DatabaseConnecter::FetchQueryResults(const std::string& query) {
    sqlite3_stmt* stmt;
    std::vector<std::unordered_map<std::string, std::string>> results;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        LogError("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return results;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::unordered_map<std::string, std::string> row;
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; ++i) {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row[columnName] = columnValue ? columnValue : "";
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

bool DatabaseConnecter::ExecutePreparedStatement(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        LogError("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LogError("Execution failed: " + std::string(sqlite3_errmsg(db)));
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
