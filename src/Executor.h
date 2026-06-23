#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Database.h"
#include <vector>
#include <string>

class Executor {
private:
    Database& db;

    void handleCreate(const std::vector<std::string>& tokens);
    void handleInsert(const std::vector<std::string>& tokens);
    void handleSelect(const std::vector<std::string>& tokens);
    void handleDelete(const std::vector<std::string>& tokens);
    void handleUpdate(const std::vector<std::string>& tokens);
    void handleSave(const std::vector<std::string>& tokens);
    void handleLoad(const std::vector<std::string>& tokens);
    void handleIndex(const std::vector<std::string>& tokens);
    void handleShow(const std::vector<std::string>& tokens);
    void handleTransaction(const std::vector<std::string>& tokens);

public:
    Executor(Database& database);
    void execute(const std::vector<std::string>& tokens);
};

#endif