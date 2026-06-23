#include "BPlusTree.h"
#include <iostream>

void BPlusTree::insert(
    const std::string& key,
    const std::vector<std::string>& row)
{
    tree[key].push_back(row);
}

std::vector<std::vector<std::string>>
BPlusTree::search(
    const std::string& key) const
{
    auto it = tree.find(key);

    if(it == tree.end()){
        return {};
    }

    return it->second;
}

void BPlusTree::print() const
{
    for(const auto& pair : tree){

        std::cout << pair.first << ": ";

        for(const auto& row : pair.second){

            std::cout << "[";

            for(const std::string& value : row){
                std::cout << value << " ";
            }

            std::cout << "]";
        }

        std::cout << std::endl;
    }
}