#ifndef INDEXER_HPP
#define INDEXER_HPP

#include "bptree.hpp"
#include <string>
#include <map>

class Indexer {
public:
    Indexer(BPTree& tree);
    void indexKeywords(const std::string& url, const std::map<std::string, int>& keywordFreq);

private:
    BPTree& tree;
};

#endif
