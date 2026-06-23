#include "Table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

Table::Table()
    : bufferManager(3) {
}

Table::Table(const std::vector<std::string>& columns)
    : columns(columns), bufferManager(3) {
}

void Table::insertRow(const std::vector<std::string>& values) {
    if (values.size() != columns.size()) {
        std::cout << "Error: column count does not match value count." << std::endl;
        return;
    }

    if (pages.empty() || pages.back().isFull()) {
        pages.push_back(Page());
    }

    pages.back().addRow(values);

    int pageId = pages.size() - 1;
    bufferManager.loadPage(pageId, pages.back());

    for (auto& pair : indexes) {
        const std::string& columnName = pair.first;
        BPlusTree& index = pair.second;

        int columnIndex = getColumnIndex(columnName);

        if (columnIndex != -1) {
            index.insert(values[columnIndex], values);
        }
    }
}

void Table::print() const{
    for(const std::string& column : columns){
        std::cout<<column<<"\t";
    }
    std::cout<<std::endl;

    for(const Page& page : pages){
        page.print();
    }
}

void Table::printWhere(const std::string& columnName,
                       const std::string& value) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Error: column does not exist." << std::endl;
        return;
    }

    if (hasIndex(columnName)) {
        for (const std::string& column : columns) {
            std::cout << column << "\t";
        }
        std::cout << std::endl;

        std::vector<std::vector<std::string>> result =
            indexes.at(columnName).search(value);

        for (const auto& row : result) {
            for (const std::string& entry : row) {
                std::cout << entry << "\t";
            }
            std::cout << std::endl;
        }

        return;
    }

    // normal page scan if no index
    for (const std::string& column : columns) {
        std::cout << column << "\t";
    }
    std::cout << std::endl;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            if (row[columnIndex] == value) {
                for (const std::string& entry : row) {
                    std::cout << entry << "\t";
                }
                std::cout << std::endl;
            }
        }
    }
}

void Table::deleteWhere(const std::string& columnName,
                        const std::string& value) {

    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    for (Page& page : pages) {

        auto& rows = page.getRows();

        rows.erase(
            std::remove_if(
                rows.begin(),
                rows.end(),
                [&](const std::vector<std::string>& row) {
                    return row[columnIndex] == value;
                }),
            rows.end()
        );
    }
    rebuildIndexes();
}

void Table::updateWhere(const std::string& targetColumn,
                        const std::string& newValue,
                        const std::string& conditionColumn,
                        const std::string& conditionValue) {

    int targetIndex = getColumnIndex(targetColumn);
    int conditionIndex = getColumnIndex(conditionColumn);

    if (targetIndex == -1 || conditionIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    for (Page& page : pages) {

        for (auto& row : page.getRows()) {

            if (row[conditionIndex] == conditionValue) {
                row[targetIndex] = newValue;
            }

        }

    }
    rebuildIndexes();
}

void Table::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    for (const std::string& column : columns) {
        outFile << column << " ";
    }
    outFile << std::endl;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            for (const std::string& value : row) {
                outFile << value << " ";
            }
            outFile << std::endl;
        }
    }

    outFile.close();
}

void Table::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile) {
        std::cout << "File not found." << std::endl;
        return;
    }

    columns.clear();
    pages.clear();

    std::string line;

    // Read columns
    if(std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string column;

        while (ss >> column) {
            columns.push_back(column);
        }
    }

    // Read rows
    while(std::getline(inFile, line)) {
        std::stringstream ss(line);

        std::vector<std::string> row;
        std::string value;

        while (ss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            if (pages.empty() || pages.back().isFull()) {
                pages.push_back(Page());
            }

            pages.back().addRow(row);
        }
    }

    inFile.close();
}

int Table::getColumnIndex(const std::string& columnName) const {

    for (int i = 0; i < columns.size(); i++) {

        if (columns[i] == columnName) {
            return i;
        }

    }

    return -1;
}

void Table::createIndex(const std::string& columnName) {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Error: column does not exist." << std::endl;
        return;
    }

    indexes[columnName] = BPlusTree();

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            indexes[columnName].insert(row[columnIndex], row);
        }
    }

    std::cout << "B+ tree index created on " << columnName << "." << std::endl;
}

bool Table::hasIndex(const std::string& columnName) const {
    return indexes.count(columnName) > 0;
}

void Table::printBufferStatus() const {
    bufferManager.printStatus();
}

void Table::rebuildIndexes() {
    std::vector<std::string> indexedColumns;

    for (const auto& pair : indexes) {
        indexedColumns.push_back(pair.first);
    }

    indexes.clear();

    for (const std::string& columnName : indexedColumns) {
        createIndex(columnName);
    }
}

void Table::dropIndex(const std::string& columnName) {
    if (!hasIndex(columnName)) {
        std::cout << "Error: index does not exist." << std::endl;
        return;
    }

    indexes.erase(columnName);

    std::cout << "Index dropped on " << columnName << "." << std::endl;
}

void Table::showIndexes() const {
    if (indexes.empty()) {
        std::cout << "No indexes." << std::endl;
        return;
    }

    for (const auto& pair : indexes) {
        std::cout << pair.first << std::endl;
    }
}

void Table::describe() const {
    for (const std::string& column : columns) {
        std::cout << column << std::endl;
    }
}

void Table::printOrderBy(const std::string& columnName, bool descending) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    std::vector<std::vector<std::string>> allRows;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            allRows.push_back(row);
        }
    }

    std::sort(allRows.begin(), allRows.end(),
              [columnIndex, descending](const std::vector<std::string>& a,
                                        const std::vector<std::string>& b) {
                  if (descending) {
                      return a[columnIndex] > b[columnIndex];
                  }
                  return a[columnIndex] < b[columnIndex];
              });

    for (const std::string& column : columns) {
        std::cout << column << "\t";
    }
    std::cout << std::endl;

    for (const auto& row : allRows) {
        for (const std::string& value : row) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
}

void Table::printLimit(int limit) const {
    for (const std::string& column : columns) {
        std::cout << column << "\t";
    }
    std::cout << std::endl;

    int count = 0;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            if (count >= limit) {
                return;
            }

            for (const std::string& value : row) {
                std::cout << value << "\t";
            }
            std::cout << std::endl;

            count++;
        }
    }
}

int Table::countRows() const {
    int count = 0;

    for (const Page& page : pages) {
        count += page.getRows().size();
    }

    return count;
}

void Table::minValue(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    bool first = true;
    std::string minVal;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            if (first || row[columnIndex] < minVal) {
                minVal = row[columnIndex];
                first = false;
            }
        }
    }

    if (first) {
        std::cout << "No rows." << std::endl;
    } else {
        std::cout << minVal << std::endl;
    }
}

void Table::maxValue(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    bool first = true;
    std::string maxVal;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            if (first || row[columnIndex] > maxVal) {
                maxVal = row[columnIndex];
                first = false;
            }
        }
    }

    if (first) {
        std::cout << "No rows." << std::endl;
    } else {
        std::cout << maxVal << std::endl;
    }
}

void Table::avgValue(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    double sum = 0.0;
    int count = 0;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            sum += std::stod(row[columnIndex]);
            count++;
        }
    }

    if (count == 0) {
        std::cout << "No rows." << std::endl;
        return;
    }

    std::cout << sum / count << std::endl;
}

void Table::sumValue(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    double sum = 0.0;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            sum += std::stod(row[columnIndex]);
        }
    }

    std::cout << sum << std::endl;
}

void Table::countGroupBy(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if(columnIndex == -1){
        std::cout << "Column not found." << std::endl;
        return;
    }

    std::unordered_map<std::string,int> counts;

    for(const Page& page : pages){
        for(const auto& row : page.getRows()){
            counts[row[columnIndex]]++;
        }
    }

    for(const auto& pair : counts){
        std::cout << pair.first << "\t"
                  << pair.second << std::endl;
    }
}

void Table::avgGroupBy(const std::string& valueColumn,
                       const std::string& groupColumn) const {
    int valueIndex = getColumnIndex(valueColumn);
    int groupIndex = getColumnIndex(groupColumn);

    if(valueIndex == -1 || groupIndex == -1){
        std::cout << "Column not found." << std::endl;
        return;
    }

    std::unordered_map<std::string, double> sums;
    std::unordered_map<std::string, int> counts;

    for(const Page& page : pages){
        for(const auto& row : page.getRows()){
            std::string group = row[groupIndex];

            sums[group] += std::stod(row[valueIndex]);
            counts[group]++;
        }
    }

    for(const auto& pair : sums){
        std::string group = pair.first;
        double avg = sums[group] / counts[group];

        std::cout << group << "\t" << avg << std::endl;
    }
}

void Table::distinctValue(const std::string& columnName) const {
    int columnIndex = getColumnIndex(columnName);

    if (columnIndex == -1) {
        std::cout << "Column not found." << std::endl;
        return;
    }

    std::unordered_set<std::string> seen;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            seen.insert(row[columnIndex]);
        }
    }

    for (const std::string& value : seen) {
        std::cout << value << std::endl;
    }
}

void Table::avgGroupByHaving(const std::string& valueColumn,
                             const std::string& groupColumn,
                             const std::string& op,
                             double threshold) const {
    int valueIndex = getColumnIndex(valueColumn);
    int groupIndex = getColumnIndex(groupColumn);

    if(valueIndex == -1 || groupIndex == -1){
        std::cout << "Column not found." << std::endl;
        return;
    }

    std::unordered_map<std::string, double> sums;
    std::unordered_map<std::string, int> counts;

    for(const Page& page : pages){
        for(const auto& row : page.getRows()){
            std::string group = row[groupIndex];
            sums[group] += std::stod(row[valueIndex]);
            counts[group]++;
        }
    }

    for(const auto& pair : sums){
        std::string group = pair.first;
        double avg = sums[group] / counts[group];

        bool pass = false;

        if(op == ">") pass = avg > threshold;
        else if(op == "<") pass = avg < threshold;
        else if(op == "=") pass = avg == threshold;

        if(pass){
            std::cout << group << "\t" << avg << std::endl;
        }
    }
}

std::vector<std::string> Table::getColumns() const {
    return columns;
}

std::vector<std::vector<std::string>> Table::getAllRows() const {
    std::vector<std::vector<std::string>> allRows;

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {
            allRows.push_back(row);
        }
    }

    return allRows;
}

void Table::saveBinary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);

    if (!out) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    int columnCount = columns.size();
    out.write(reinterpret_cast<char*>(&columnCount), sizeof(columnCount));

    for (const std::string& column : columns) {
        int len = column.size();

        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(column.c_str(), len);
    }

    int totalRows = 0;

    for (const Page& page : pages) {
        totalRows += page.getRows().size();
    }

    out.write(reinterpret_cast<char*>(&totalRows), sizeof(totalRows));

    for (const Page& page : pages) {
        for (const auto& row : page.getRows()) {

            for (const std::string& value : row) {

                int len = value.size();

                out.write(reinterpret_cast<char*>(&len), sizeof(len));
                out.write(value.c_str(), len);
            }
        }
    }
}

void Table::loadBinary(const std::string& filename) {

    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cout << "File not found." << std::endl;
        return;
    }

    columns.clear();
    pages.clear();

    int columnCount;

    in.read(reinterpret_cast<char*>(&columnCount),
            sizeof(columnCount));

    for (int i = 0; i < columnCount; i++) {

        int len;

        in.read(reinterpret_cast<char*>(&len),
                sizeof(len));

        std::string column(len, ' ');

        in.read(&column[0], len);

        columns.push_back(column);
    }

    int rowCount;

    in.read(reinterpret_cast<char*>(&rowCount),
            sizeof(rowCount));

    for (int i = 0; i < rowCount; i++) {

        std::vector<std::string> row;

        for (int j = 0; j < columnCount; j++) {

            int len;

            in.read(reinterpret_cast<char*>(&len),
                    sizeof(len));

            std::string value(len, ' ');

            in.read(&value[0], len);

            row.push_back(value);
        }

        insertRow(row);
    }
}