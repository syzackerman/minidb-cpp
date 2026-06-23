#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <map>
#include <string>
#include <vector>

class BPlusTree {
private:
    std::map<std::string,
             std::vector<std::vector<std::string>>> tree;

public:
    void insert(const std::string& key,
                const std::vector<std::string>& row);

    std::vector<std::vector<std::string>>
    search(const std::string& key) const;

    void print() const;
};

#endif