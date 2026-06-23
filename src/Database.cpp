#include "Database.h"
#include <iostream>
#include <fstream>
#include <cstdio>

void Database::createTable(const std::string& name, const std::vector<std::string>& columns){
    if(tables.count(name)){
        std::cout<<"Error: table already exists."<<std::endl;
        return;
    }
    tables[name] = Table(columns);
    std::cout<<"Table created"<<std::endl;
}

void Database::insertInto(const std::string& name, const std::vector<std::string>& values){
    if(!tables.count(name)){
        std::cout<<"Error: table does not exist."<<std::endl;
        return;
    }
    tables[name].insertRow(values);
    std::cout<<"Row inserted"<<std::endl;
}

void Database::selectAll(const std::string& name) const{
    auto it = tables.find(name);
    if(it == tables.end()){
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }
    it->second.print();
}

void Database::selectWhere(const std::string& name, 
                            const std::string& columnName, 
                            const std::string& value) const{
    auto it = tables.find(name);
    if(it == tables.end()){
        std::cout<<"Error: table does not exist." << std::endl;
        return;
    }
    it->second.printWhere(columnName, value);
}

void Database::deleteWhere(const std::string& name, 
                            const std::string& columnName, 
                            const std::string& value){
    auto it = tables.find(name);
    if(it==tables.end()){
        std::cout<<"Error: table does not exist." << std::endl;
        return;
    }
    it->second.deleteWhere(columnName, value);
}

void Database::updateWhere(const std::string& name, 
                            const std::string& targetColumn, 
                            const std::string& newValue, 
                            const std::string& conditionColumn, 
                            const std::string& conditionValue){
    auto it = tables.find(name);
    if(it==tables.end()){
        std::cout<<"Error: table does not exist." << std::endl;
        return;
    }
    it->second.updateWhere(targetColumn, newValue, conditionColumn, conditionValue);
}

void Database::saveTable(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Table does not exist." << std::endl;
        return;
    }

    it->second.saveBinary(tableName + ".db");
}

void Database::loadTable(const std::string& tableName,
                         const std::string& filename) {
    tables[tableName] = Table();
    tables[tableName].loadBinary(filename);
}

void Database::saveAll() const {
    std::ofstream metaFile("tables.meta");

    if (!metaFile) {
        std::cout << "Error opening tables.meta." << std::endl;
        return;
    }

    for (const auto& pair : tables) {
        const std::string& tableName = pair.first;
        const Table& table = pair.second;

        metaFile << tableName << std::endl;
        table.saveBinary(tableName + ".db");
    }

    metaFile.close();

    std::cout << "Database saved." << std::endl;
}

void Database::loadAll() {
    std::ifstream metaFile("tables.meta");

    if (!metaFile) {
        return;
    }

    std::string tableName;

    while (std::getline(metaFile, tableName)) {
        if (tableName.empty()) {
            continue;
        }

        tables[tableName] = Table();
        tables[tableName].loadBinary(tableName + ".db");
    }

    metaFile.close();

    std::cout << "Database loaded." << std::endl;
}

void Database::dropTable(const std::string& tableName) {
    if (!tables.count(tableName)) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    tables.erase(tableName);
    std::remove((tableName + ".db").c_str());

    std::cout << "Table dropped." << std::endl;
}

void Database::createIndex(const std::string& tableName,
                           const std::string& columnName) {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.createIndex(columnName);
}

void Database::printBufferStatus(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.printBufferStatus();
}


void Database::dropIndex(const std::string& tableName,
                         const std::string& columnName) {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.dropIndex(columnName);
}

void Database::showTables() const {
    if (tables.empty()) {
        std::cout << "No tables." << std::endl;
        return;
    }

    for (const auto& pair : tables) {
        std::cout << pair.first << std::endl;
    }
}

void Database::showIndexes(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.showIndexes();
}

void Database::describeTable(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.describe();
}

void Database::orderBy(const std::string& tableName,
                       const std::string& columnName,
                       bool descending) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.printOrderBy(columnName, descending);
}

void Database::selectLimit(const std::string& tableName, int limit) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.printLimit(limit);
}

void Database::countRows(const std::string& tableName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    std::cout << it->second.countRows() << std::endl;
}

void Database::minValue(const std::string& tableName,
                        const std::string& columnName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.minValue(columnName);
}

void Database::maxValue(const std::string& tableName,
                        const std::string& columnName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.maxValue(columnName);
}

void Database::avgValue(const std::string& tableName,
                        const std::string& columnName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.avgValue(columnName);
}

void Database::sumValue(const std::string& tableName,
                        const std::string& columnName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.sumValue(columnName);
}

void Database::countGroupBy(const std::string& tableName,
                            const std::string& columnName) const {

    auto it = tables.find(tableName);

    if(it == tables.end()){
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.countGroupBy(columnName);
}

void Database::avgGroupBy(const std::string& tableName,
                          const std::string& valueColumn,
                          const std::string& groupColumn) const {
    auto it = tables.find(tableName);

    if(it == tables.end()){
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.avgGroupBy(valueColumn, groupColumn);
}

void Database::distinctValue(const std::string& tableName,
                             const std::string& columnName) const {
    auto it = tables.find(tableName);

    if (it == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.distinctValue(columnName);
}

void Database::avgGroupByHaving(const std::string& tableName,
                                const std::string& valueColumn,
                                const std::string& groupColumn,
                                const std::string& op,
                                double threshold) const {
    auto it = tables.find(tableName);

    if(it == tables.end()){
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    it->second.avgGroupByHaving(valueColumn, groupColumn, op, threshold);
}

void Database::innerJoin(const std::string& leftTable,
                         const std::string& rightTable,
                         const std::string& leftColumn,
                         const std::string& rightColumn) const {
    auto leftIt = tables.find(leftTable);
    auto rightIt = tables.find(rightTable);

    if (leftIt == tables.end() || rightIt == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    std::vector<std::string> leftCols = leftIt->second.getColumns();
    std::vector<std::string> rightCols = rightIt->second.getColumns();

    int leftIndex = -1;
    int rightIndex = -1;

    for (int i = 0; i < leftCols.size(); i++) {
        if (leftCols[i] == leftColumn) leftIndex = i;
    }

    for (int i = 0; i < rightCols.size(); i++) {
        if (rightCols[i] == rightColumn) rightIndex = i;
    }

    if (leftIndex == -1 || rightIndex == -1) {
        std::cout << "Error: column does not exist." << std::endl;
        return;
    }

    for (const std::string& col : leftCols) {
        std::cout << leftTable << "." << col << "\t";
    }

    for (const std::string& col : rightCols) {
        std::cout << rightTable << "." << col << "\t";
    }

    std::cout << std::endl;

    auto leftRows = leftIt->second.getAllRows();
    auto rightRows = rightIt->second.getAllRows();

    for (const auto& leftRow : leftRows) {
        for (const auto& rightRow : rightRows) {
            if (leftRow[leftIndex] == rightRow[rightIndex]) {
                for (const std::string& value : leftRow) {
                    std::cout << value << "\t";
                }
                for (const std::string& value : rightRow) {
                    std::cout << value << "\t";
                }
                std::cout << std::endl;
            }
        }
    }
}

void Database::leftJoin(const std::string& leftTable,
                        const std::string& rightTable,
                        const std::string& leftColumn,
                        const std::string& rightColumn) const {
    auto leftIt = tables.find(leftTable);
    auto rightIt = tables.find(rightTable);

    if (leftIt == tables.end() || rightIt == tables.end()) {
        std::cout << "Error: table does not exist." << std::endl;
        return;
    }

    auto leftCols = leftIt->second.getColumns();
    auto rightCols = rightIt->second.getColumns();

    int leftIndex = -1;
    int rightIndex = -1;

    for (int i = 0; i < leftCols.size(); i++) {
        if (leftCols[i] == leftColumn) leftIndex = i;
    }

    for (int i = 0; i < rightCols.size(); i++) {
        if (rightCols[i] == rightColumn) rightIndex = i;
    }

    if (leftIndex == -1 || rightIndex == -1) {
        std::cout << "Error: column does not exist." << std::endl;
        return;
    }

    for (const std::string& col : leftCols) {
        std::cout << leftTable << "." << col << "\t";
    }

    for (const std::string& col : rightCols) {
        std::cout << rightTable << "." << col << "\t";
    }

    std::cout << std::endl;

    auto leftRows = leftIt->second.getAllRows();
    auto rightRows = rightIt->second.getAllRows();

    for (const auto& leftRow : leftRows) {
        bool matched = false;

        for (const auto& rightRow : rightRows) {
            if (leftRow[leftIndex] == rightRow[rightIndex]) {
                matched = true;

                for (const std::string& value : leftRow) {
                    std::cout << value << "\t";
                }

                for (const std::string& value : rightRow) {
                    std::cout << value << "\t";
                }

                std::cout << std::endl;
            }
        }

        if (!matched) {
            for (const std::string& value : leftRow) {
                std::cout << value << "\t";
            }

            for (int i = 0; i < rightCols.size(); i++) {
                std::cout << "NULL\t";
            }

            std::cout << std::endl;
        }
    }
}

void Database::beginTransaction() {
    if (inTransaction) {
        std::cout << "Error: transaction already in progress." << std::endl;
        return;
    }

    transactionBackup = tables;
    inTransaction = true;

    std::cout << "Transaction started." << std::endl;
}

void Database::commitTransaction() {
    if (!inTransaction) {
        std::cout << "Error: no transaction in progress." << std::endl;
        return;
    }

    transactionBackup.clear();
    inTransaction = false;

    saveAll();

    std::cout << "Transaction committed." << std::endl;
}

void Database::rollbackTransaction() {
    if (!inTransaction) {
        std::cout << "Error: no transaction in progress." << std::endl;
        return;
    }

    tables = transactionBackup;
    transactionBackup.clear();
    inTransaction = false;

    std::cout << "Transaction rolled back." << std::endl;
}

bool Database::hasActiveTransaction() const {
    return inTransaction;
}