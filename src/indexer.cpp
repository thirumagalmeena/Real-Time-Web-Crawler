#include "indexer.hpp"

Indexer::Indexer(BPTree& tree) : tree(tree) {}

void Indexer::indexKeywords(const std::string& url, const std::map<std::string, int>& keywordFreq) {
    for (const auto& [keyword, freq] : keywordFreq) {
        // ✅ ONLY the keyword should go in as the key
        tree.insert(keyword, url, freq);
    }
}
