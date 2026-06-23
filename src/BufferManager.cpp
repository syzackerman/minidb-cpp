#include "BufferManager.h"
#include <iostream>

BufferManager::BufferManager(int capacity) {
    this->capacity = capacity;
}

void BufferManager::loadPage(int pageId, const Page& page) {
    if (pageTable.count(pageId)) {
        lruList.remove(pageId);
        lruList.push_front(pageId);
        pageTable[pageId] = page;
        return;
    }

    if (pageTable.size() >= capacity) {
        evictPage();
    }

    pageTable[pageId] = page;
    lruList.push_front(pageId);
}

Page* BufferManager::getPage(int pageId) {
    if (!pageTable.count(pageId)) {
        return nullptr;
    }

    lruList.remove(pageId);
    lruList.push_front(pageId);

    return &pageTable[pageId];
}

void BufferManager::evictPage() {
    if (lruList.empty()) {
        return;
    }

    int leastUsedPageId = lruList.back();
    lruList.pop_back();
    pageTable.erase(leastUsedPageId);

    std::cout << "Evicted page " << leastUsedPageId << std::endl;
}

void BufferManager::printStatus() const {
    std::cout << "Buffer pages: ";

    for (int pageId : lruList) {
        std::cout << pageId << " ";
    }

    std::cout << std::endl;
}