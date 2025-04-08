#ifndef DATABASECONNECTER_H
#define DATABASECONNECTER_H


#include <psp2/sqlite.h>

#include <string>
#include <vector>
#include <unordered_map>


class DatabaseConnecter {
    private:
        sqlite3* db;
        std::string dbPath;

    public:
        DatabaseConnecter(const std::string& path);
        ~DatabaseConnecter();

        bool Open();
        void Close();
        bool ExecuteQuery(const std::string& query);
        std::vector<std::unordered_map<std::string, std::string>> FetchQueryResults(const std::string& query);
        bool ExecutePreparedStatement(const std::string& sql, const std::vector<std::string>& params);

        std::string EscapeString(const std::string& str);

};
#endif