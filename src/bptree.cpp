#include "bptree.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

BPTree::BPTree() {
    root = std::make_shared<BPTreeNode>(true);
}

void BPTree::insert(const std::string& key, const std::string& url, int frequency) {
    std::string normalizedKey = key;
    std::transform(normalizedKey.begin(), normalizedKey.end(), normalizedKey.begin(), ::tolower);
    insertInternal(normalizedKey, url, frequency, root);
}

void BPTree::insertInternal(const std::string& key, const std::string& url, int frequency, std::shared_ptr<BPTreeNode> node) {
    if (node->isLeaf) {
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
        int index = it - node->keys.begin();

        if (it != node->keys.end() && *it == key) {
            // Check if URL already exists for this key
            bool urlExists = false;
            for (auto& pair : node->values[index]) {
                if (pair.first == url) {
                    pair.second += frequency;
                    urlExists = true;
                    break;
                }
            }
            if (!urlExists) {
                node->values[index].emplace_back(url, frequency);
            }
        } else {
            node->keys.insert(it, key);
            node->values.insert(node->values.begin() + index, { {url, frequency} });
        }

        if (node->keys.size() > ORDER)
            splitLeaf(node);
    } else {
        // Handle internal node traversal for insertion
        int i = 0;
        while (i < node->keys.size() && key >= node->keys[i]) i++;
        insertInternal(key, url, frequency, node->children[i]);
    }
}

void BPTree::splitLeaf(std::shared_ptr<BPTreeNode> node) {
    auto newLeaf = std::make_shared<BPTreeNode>(true);
    int mid = (ORDER + 1) / 2;

    newLeaf->keys.assign(node->keys.begin() + mid, node->keys.end());
    newLeaf->values.assign(node->values.begin() + mid, node->values.end());

    node->keys.resize(mid);
    node->values.resize(mid);

    newLeaf->next = node->next;
    node->next = newLeaf;

    if (node == root) {
        auto newRoot = std::make_shared<BPTreeNode>(false);
        newRoot->keys.push_back(newLeaf->keys[0]);
        newRoot->children.push_back(node);
        newRoot->children.push_back(newLeaf);
        root = newRoot;
    } else {
        // Propagate the split to the parent
        // (Implementation depends on your B+ tree structure)
    }
}

std::vector<std::pair<std::string, int>> BPTree::search(const std::string& key) const {
    std::string normalizedKey = key;
    std::transform(normalizedKey.begin(), normalizedKey.end(), normalizedKey.begin(), ::tolower);
    
    std::cout << "[DEBUG] Searching for normalized key: '" << normalizedKey << "'\n";
    
    auto node = root;
    while (!node->isLeaf) {
        std::cout << "[DEBUG] Internal node keys: ";
        for (const auto& k : node->keys) std::cout << k << " ";
        std::cout << "\n";
        
        int i = 0;
        while (i < node->keys.size() && normalizedKey >= node->keys[i]) i++;
        node = node->children[i];
    }

    std::cout << "[DEBUG] Leaf node keys: ";
    for (const auto& k : node->keys) std::cout << k << " ";
    std::cout << "\n";

    for (size_t i = 0; i < node->keys.size(); ++i) {
        if (node->keys[i] == normalizedKey) {
            std::cout << "[DEBUG] Found matching key at position " << i << "\n";
            return node->values[i];
        }
    }
    
    std::cout << "[DEBUG] Key not found in leaf node\n";
    return {};
}

void BPTree::print() const {
    auto node = root;
    while (!node->isLeaf) node = node->children[0];

    while (node) {
        for (size_t i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys[i] << ": ";
            for (const auto& [url, freq] : node->values[i])
                std::cout << "[" << url << ": " << freq << "] ";
            std::cout << "| ";
        }
        std::cout << "\n";
        node = node->next;
    }
}

void BPTree::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open file for saving.\n";
        return;
    }

    auto node = root;
    while (!node->isLeaf) node = node->children[0];

    while (node) {
        for (size_t i = 0; i < node->keys.size(); ++i) {
            for (const auto& [url, freq] : node->values[i]) {
                out << node->keys[i] << ":" << url << ":" << freq << "\n";
            }
        }
        node = node->next;
    }

    out.close();
    std::cout << "Index saved to '" << filename << "'\n";
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

void BPTree::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Failed to open file for loading.\n";
        return;
    }

    // Clear existing tree properly
    root = std::make_shared<BPTreeNode>(true);

    std::string line;
    int lineNum = 0;
    while (std::getline(in, line)) {
        lineNum++;
        line = trim(line);
        if (line.empty()) continue;

        size_t lastColon = line.rfind(':');
        size_t secondLastColon = line.rfind(':', lastColon - 1);

        if (lastColon == std::string::npos || secondLastColon == std::string::npos) {
            std::cerr << "Line " << lineNum << ": Skipping malformed line (missing colons): " << line << "\n";
            continue;
        }

        std::string keyword = trim(line.substr(0, secondLastColon));
        std::string url = trim(line.substr(secondLastColon + 1, lastColon - secondLastColon - 1));
        std::string freqStr = trim(line.substr(lastColon + 1));

        // Normalize keyword to lowercase
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

        int freq = 0;
        try {
            freq = std::stoi(freqStr);
        } catch (...) {
            std::cerr << "Line " << lineNum << ": Invalid frequency format: " << freqStr << "\n";
            continue;
        }

        if (keyword.empty() || url.empty()) {
            std::cerr << "Line " << lineNum << ": Skipping line with empty keyword or URL\n";
            continue;
        }

        if (freq <= 0) {
            std::cerr << "Line " << lineNum << ": Skipping line with non-positive frequency\n";
            continue;
        }

        insert(keyword, url, freq);
    }

    in.close();
    std::cout << "Index loaded from '" << filename << "' (" << lineNum << " lines processed)\n";
}
