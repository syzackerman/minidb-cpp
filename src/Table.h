#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include "Page.h"
#include <unordered_map>
#include "BufferManager.h"
#include "BPlusTree.h"

class Table
{
private:
    std::vector<std::string> columns;
    std::vector<Page> pages;
    std::unordered_map<std::string, BPlusTree> indexes;
    BufferManager bufferManager;
    void rebuildIndexes();

public:
    Table();
    Table(const std::vector<std::string> &columns);

    void insertRow(const std::vector<std::string> &values);
    void print() const;
    void printWhere(const std::string &columnName, const std::string &value) const;
    void deleteWhere(const std::string &columnName, const std::string& value);
    void updateWhere(const std::string &targetColumn, const std::string & newValue, const std::string &conditionColumn, const std::string &conditionValue);
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    int getColumnIndex(const std::string& columnName) const;
    void createIndex(const std::string& columnName);
    bool hasIndex(const std::string& columnName) const;
    void printBufferStatus() const;
    void dropIndex(const std::string& columnName);
    void showIndexes() const;
    void describe() const;
    void printOrderBy(const std::string& columnName, bool descending) const;
    void printLimit(int limit) const;
    int countRows() const;
    void minValue(const std::string& columnName) const;
    void maxValue(const std::string& columnName) const;
    void avgValue(const std::string& columnName) const;
    void sumValue(const std::string& columnName) const;
    void countGroupBy(const std::string& columnName) const;
    void avgGroupBy(const std::string& valueColumn,
                const std::string& groupColumn) const;
    void distinctValue(const std::string& columnName) const;
    void avgGroupByHaving(const std::string& valueColumn,
                      const std::string& groupColumn,
                      const std::string& op,
                      double threshold) const;
    std::vector<std::string> getColumns() const;
    std::vector<std::vector<std::string>> getAllRows() const;
    void saveBinary(const std::string& filename) const;
    void loadBinary(const std::string& filename);  
};

#endif