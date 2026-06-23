#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>

class Page {
private:
    std::vector<std::vector<std::string>> rows;
    int maxRows;

public:
    Page(int maxRows = 3);

    bool isFull() const;
    void addRow(const std::vector<std::string>& row);
    void print() const;

    std::vector<std::vector<std::string>>& getRows();
    const std::vector<std::vector<std::string>>& getRows() const;
};

#endif