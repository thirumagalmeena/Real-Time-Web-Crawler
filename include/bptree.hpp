#ifndef BPTREE_HPP
#define BPTREE_HPP

#include <vector>
#include <string>
#include <memory>

const int ORDER = 4;


struct BPTreeNode {
    bool isLeaf;
    std::vector<std::string> keys;
    std::vector<std::vector<std::pair<std::string, int>>> values;
    std::vector<std::shared_ptr<BPTreeNode>> children;
    std::shared_ptr<BPTreeNode> next;

    BPTreeNode(bool isLeaf) : isLeaf(isLeaf), next(nullptr) {}
};

class BPTree {
private:
    std::shared_ptr<BPTreeNode> root;
    void insertInternal(const std::string& key, const std::string& url, int frequency, std::shared_ptr<BPTreeNode> node);
    void splitLeaf(std::shared_ptr<BPTreeNode> node);

public:
    BPTree();
    void insert(const std::string& key, const std::string& url, int frequency);
    std::vector<std::pair<std::string, int>> search(const std::string& key) const;
    void print() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

};

#endif
