#ifndef DATABASE_H
#define DATABASE_H

#include "Table.h"
#include <string>
#include <unordered_map>
#include <vector>

class Database{
private:
    std::unordered_map<std::string, Table> tables;
    std::unordered_map<std::string, Table> transactionBackup;
    bool inTransaction = false;
public:
    void createTable(const std::string& name, const std::vector<std::string>& columns);
    void insertInto(const std::string& name, const std::vector<std::string>& values);
    void selectAll(const std::string& name) const;
    void selectWhere(const std::string& name, 
                    const std::string& columnName, 
                    const std::string& value) const;
    void deleteWhere(const std::string& name, 
                    const std::string& columnName, 
                    const std::string& value);
    void updateWhere(const std::string& name, 
                    const std::string& targetColumn, 
                    const std::string& newValue, 
                    const std::string& conditionColumn, 
                    const std::string& conditionValue);
    void saveTable(const std::string& tableName) const;
    void loadTable(const std::string& tableName,
                    const std::string& filename);
    void saveAll() const;
    void loadAll();
    void createIndex(const std::string& tableName,
                 const std::string& columnName);
    void printBufferStatus(const std::string& tableName) const;
    void dropTable(const std::string& tableName);
    void dropIndex(const std::string& tableName,
               const std::string& columnName);
    void showTables() const;
    void showIndexes(const std::string& tableName) const;
    void describeTable(const std::string& tableName) const;
    void orderBy(const std::string& tableName,
             const std::string& columnName,
             bool descending) const;
    void selectLimit(const std::string& tableName, int limit) const;
    void countRows(const std::string& tableName) const;
    void minValue(const std::string& tableName, const std::string& columnName) const;
    void maxValue(const std::string& tableName, const std::string& columnName) const;
    void avgValue(const std::string& tableName,
              const std::string& columnName) const;
    void sumValue(const std::string& tableName,
              const std::string& columnName) const;
    void countGroupBy(const std::string& tableName,
                  const std::string& columnName) const;
    void avgGroupBy(const std::string& tableName,
                const std::string& valueColumn,
                const std::string& groupColumn) const;
    void distinctValue(const std::string& tableName,
                   const std::string& columnName) const;
    void avgGroupByHaving(const std::string& tableName,
                    const std::string& valueColumn,
                    const std::string& groupColumn,
                    const std::string& op,
                    double threshold) const;
    void innerJoin(const std::string& leftTable,
               const std::string& rightTable,
               const std::string& leftColumn,
               const std::string& rightColumn) const;
    void leftJoin(const std::string& leftTable,
              const std::string& rightTable,
              const std::string& leftColumn,
              const std::string& rightColumn) const;
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
    bool hasActiveTransaction() const;
};  

#endif