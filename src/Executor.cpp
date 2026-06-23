#include "Executor.h"
#include <iostream>

Executor::Executor(Database& database)
    : db(database) {
}

void Executor::execute(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return;
    }

    std::string command = tokens[0];

    if (command == "CREATE") handleCreate(tokens);
    else if (command == "INSERT") handleInsert(tokens);
    else if (command == "SELECT") handleSelect(tokens);
    else if (command == "DELETE") handleDelete(tokens);
    else if (command == "UPDATE") handleUpdate(tokens);
    else if (command == "SAVE") handleSave(tokens);
    else if (command == "LOAD") handleLoad(tokens);
    else if (command == "CREATE_INDEX" || command == "DROP_INDEX" || command == "BUFFER_STATUS") handleIndex(tokens);
    else if (command == "SHOW" || command == "DESCRIBE" || command == "DROP") handleShow(tokens);
    else if (command == "BEGIN" || command == "COMMIT" || command == "ROLLBACK") handleTransaction(tokens);
    else std::cout << "Unknown command." << std::endl;
}

void Executor::handleCreate(const std::vector<std::string>& tokens) {
    if (tokens.size() < 4 || tokens[1] != "TABLE") {
        std::cout << "Error: invalid CREATE syntax." << std::endl;
        return;
    }

    std::vector<std::string> columns;
    for (int i = 3; i < tokens.size(); i++) columns.push_back(tokens[i]);

    db.createTable(tokens[2], columns);
}

void Executor::handleInsert(const std::vector<std::string>& tokens) {
    if (tokens.size() < 5 || tokens[1] != "INTO" || tokens[3] != "VALUES") {
        std::cout << "Error: invalid INSERT syntax." << std::endl;
        return;
    }

    std::vector<std::string> values;
    for (int i = 4; i < tokens.size(); i++) values.push_back(tokens[i]);

    db.insertInto(tokens[2], values);
}

void Executor::handleSelect(const std::vector<std::string>& tokens) {
    if (tokens.size() == 4 && tokens[1] == "COUNT" && tokens[2] == "FROM") {
        db.countRows(tokens[3]);
        return;
    }
    else if (tokens.size() == 5 && tokens[1] == "MIN" && tokens[3] == "FROM") {
        db.minValue(tokens[4], tokens[2]);
        return;
    }
    else if (tokens.size() == 5 && tokens[1] == "MAX" && tokens[3] == "FROM") {
        db.maxValue(tokens[4], tokens[2]);
        return;
    }
    else if (tokens.size() == 5 && tokens[1] == "AVG" && tokens[3] == "FROM") {
        db.avgValue(tokens[4], tokens[2]);
        return;
    }
    else if (tokens.size() == 5 && tokens[1] == "SUM" && tokens[3] == "FROM") {
        db.sumValue(tokens[4], tokens[2]);
        return;
    }
    else if (tokens.size() == 5 && tokens[1] == "DISTINCT" && tokens[3] == "FROM") {
        db.distinctValue(tokens[4], tokens[2]);
        return;
    }
    else if (tokens.size() == 7 && tokens[1] == "COUNT" && tokens[2] == "GROUP" &&
             tokens[3] == "BY" && tokens[5] == "FROM") {
        db.countGroupBy(tokens[6], tokens[4]);
        return;
    }
    else if (tokens.size() == 8 && tokens[1] == "AVG" && tokens[3] == "GROUP" &&
             tokens[4] == "BY" && tokens[6] == "FROM") {
        db.avgGroupBy(tokens[7], tokens[2], tokens[5]);
        return;
    }
    else if (tokens.size() == 11 && tokens[1] == "AVG" && tokens[3] == "GROUP" &&
             tokens[4] == "BY" && tokens[6] == "FROM" && tokens[8] == "HAVING") {
        double threshold = std::stod(tokens[10]);
        db.avgGroupByHaving(tokens[7], tokens[2], tokens[5], tokens[9], threshold);
        return;
    }
    else if (tokens.size() == 9 && tokens[1] == "*" && tokens[2] == "FROM" &&
             tokens[4] == "JOIN" && tokens[6] == "ON") {
        db.innerJoin(tokens[3], tokens[5], tokens[7], tokens[8]);
        return;
    }
    else if (tokens.size() == 10 && tokens[1] == "*" && tokens[2] == "FROM" &&
             tokens[4] == "LEFT" && tokens[5] == "JOIN" && tokens[7] == "ON") {
        db.leftJoin(tokens[3], tokens[6], tokens[8], tokens[9]);
        return;
    }
    else if (tokens.size() < 4 || tokens[1] != "*" || tokens[2] != "FROM") {
        std::cout << "Error: invalid SELECT syntax." << std::endl;
        return;
    }
    else if (tokens.size() == 4) {
        db.selectAll(tokens[3]);
        return;
    }
    else if (tokens.size() == 7 && tokens[4] == "WHERE") {
        db.selectWhere(tokens[3], tokens[5], tokens[6]);
        return;
    }
    else if (tokens.size() == 7 && tokens[4] == "ORDER" && tokens[5] == "BY") {
        db.orderBy(tokens[3], tokens[6], false);
        return;
    }
    else if (tokens.size() == 8 && tokens[4] == "ORDER" && tokens[5] == "BY" && tokens[7] == "DESC") {
        db.orderBy(tokens[3], tokens[6], true);
        return;
    }
    else if (tokens.size() == 6 && tokens[4] == "LIMIT") {
        int limit = std::stoi(tokens[5]);
        db.selectLimit(tokens[3], limit);
        return;
    }

    std::cout << "Error: invalid SELECT syntax." << std::endl;
}

void Executor::handleDelete(const std::vector<std::string>& tokens) {
    if (tokens.size() != 6 || tokens[1] != "FROM" || tokens[3] != "WHERE") {
        std::cout << "Error: invalid DELETE syntax." << std::endl;
        return;
    }

    db.deleteWhere(tokens[2], tokens[4], tokens[5]);
}

void Executor::handleUpdate(const std::vector<std::string>& tokens) {
    if (tokens.size() != 8 || tokens[2] != "SET" || tokens[5] != "WHERE") {
        std::cout << "Error: invalid UPDATE syntax." << std::endl;
        return;
    }

    db.updateWhere(tokens[1], tokens[3], tokens[4], tokens[6], tokens[7]);
}

void Executor::handleSave(const std::vector<std::string>& tokens) {
    if (db.hasActiveTransaction()) {
        std::cout << "Error: cannot SAVE during an active transaction. Use COMMIT or ROLLBACK first." << std::endl;
        return;
    }

    if (tokens.size() != 2) {
        std::cout << "Error: invalid SAVE syntax." << std::endl;
        return;
    }

    db.saveTable(tokens[1]);
}

void Executor::handleLoad(const std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
        std::cout << "Error: invalid LOAD syntax." << std::endl;
        return;
    }

    db.loadTable(tokens[1], tokens[1] + ".db");
}

void Executor::handleIndex(const std::vector<std::string>& tokens) {
    std::string command = tokens[0];

    if (command == "CREATE_INDEX") {
        if (tokens.size() != 3) {
            std::cout << "Error: invalid CREATE_INDEX syntax." << std::endl;
            return;
        }
        db.createIndex(tokens[1], tokens[2]);
    }
    else if (command == "DROP_INDEX") {
        if (tokens.size() != 3) {
            std::cout << "Error: invalid DROP_INDEX syntax." << std::endl;
            return;
        }
        db.dropIndex(tokens[1], tokens[2]);
    }
    else if (command == "BUFFER_STATUS") {
        if (tokens.size() != 2) {
            std::cout << "Error: invalid BUFFER_STATUS syntax." << std::endl;
            return;
        }
        db.printBufferStatus(tokens[1]);
    }
}

void Executor::handleShow(const std::vector<std::string>& tokens) {
    std::string command = tokens[0];

    if (command == "SHOW") {
        if (tokens.size() == 2 && tokens[1] == "TABLES") {
            db.showTables();
        }
        else if (tokens.size() == 3 && tokens[1] == "INDEXES") {
            db.showIndexes(tokens[2]);
        }
        else {
            std::cout << "Error: invalid SHOW syntax." << std::endl;
        }
    }
    else if (command == "DESCRIBE") {
        if (tokens.size() != 2) {
            std::cout << "Error: invalid DESCRIBE syntax." << std::endl;
            return;
        }
        db.describeTable(tokens[1]);
    }
    else if (command == "DROP") {
        if (tokens.size() != 3 || tokens[1] != "TABLE") {
            std::cout << "Error: invalid DROP syntax." << std::endl;
            return;
        }
        db.dropTable(tokens[2]);
    }
}

void Executor::handleTransaction(const std::vector<std::string>& tokens) {
    std::string command = tokens[0];

    if (tokens.size() != 1) {
        std::cout << "Error: invalid transaction syntax." << std::endl;
        return;
    }

    if (command == "BEGIN") db.beginTransaction();
    else if (command == "COMMIT") db.commitTransaction();
    else if (command == "ROLLBACK") db.rollbackTransaction();
}