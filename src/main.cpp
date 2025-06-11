#include "crawler.hpp"
#include "parser.hpp"
#include "bptree.hpp"
#include "indexer.hpp"
#include "crawl_manager.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

int main() {
    BPTree tree;
    Crawler crawler;
    Parser parser;
    Indexer indexer(tree);
    CrawlManager manager(crawler, parser, indexer);

    std::cout << "Real-Time Web Crawler with B+ Tree Inverted Index\n";
    std::cout << "--------------------------------------------------\n";

    std::string input;
    while (true) {
        std::cout << "\nMenu:\n"
                  << "1. crawl <url> <max_pages>\n"
                  << "2. search <keyword>\n"
                  << "3. print\n"
                  << "4. exit\n"
                  << "5. save <filename>\n"
                  << "6. load <filename>\n"
                  << "\n> ";

        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "crawl") {
            std::string seedUrl;
            int maxPages;
            iss >> seedUrl >> maxPages;
            if (seedUrl.empty() || maxPages <= 0) {
                std::cout << "Usage: crawl <url> <max_pages>\n";
                continue;
            }
            manager.crawlFromSeed(seedUrl, maxPages);
        }

        else if (command == "search") {
            std::string keyword;
            iss >> keyword;
            if (keyword.empty()) {
                std::cout << "Usage: search <keyword>\n";
                continue;
            }

            std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
            auto results = tree.search(keyword);

            if (results.empty()) {
                std::cout << "'" << keyword << "' not found in index.\n";
            } else {
                std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
                    return b.second < a.second;  // descending by frequency
                });
                std::cout << "'" << keyword << "' found in:\n";
                for (const auto& [url, freq] : results) {
                    std::cout << " - " << url << " (x" << freq << ")\n";
                }
            }
        }

        else if (command == "print") {
            std::cout << "\nB+ Tree Index:\n";
            tree.print();
        }

        else if (command == "exit") {
            std::cout << "\nEXITED\n";
            break;
        }
        else if (command == "save") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                std::cout << "Usage: save <filename>\n";
            } else {
                tree.saveToFile(filename);
            }
        }

        else if (command == "load") {
            std::string filename;
             iss >> filename;
            if (filename.empty()) {
                std::cout << "Usage: load <filename>\n";
            } else {
                tree.loadFromFile(filename);
            }
        }


        else {
            std::cout << "Unknown command. Try again.\n";
        }
    }

    return 0;
}
