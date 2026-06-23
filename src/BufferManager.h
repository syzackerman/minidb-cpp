#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include "Page.h"
#include <list>
#include <unordered_map>

class BufferManager {
private:
    int capacity;

    std::unordered_map<int, Page> pageTable;
    std::list<int> lruList;

public:
    BufferManager(int capacity = 3);

    void loadPage(int pageId, const Page& page);
    Page* getPage(int pageId);
    void evictPage();
    void printStatus() const;
};

#endif