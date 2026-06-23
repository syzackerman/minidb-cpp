#include "Page.h"
#include <iostream>

Page::Page(int maxRows) {
    this->maxRows = maxRows;
}

bool Page::isFull() const {
    return rows.size() >= maxRows;
}

void Page::addRow(const std::vector<std::string>& row) {
    rows.push_back(row);
}

void Page::print() const {
    for (const auto& row : rows) {
        for (const std::string& value : row) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<std::string>>& Page::getRows() {
    return rows;
}

const std::vector<std::vector<std::string>>& Page::getRows() const {
    return rows;
}