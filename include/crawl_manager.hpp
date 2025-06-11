#ifndef CRAWL_MANAGER_HPP
#define CRAWL_MANAGER_HPP

#include "crawler.hpp"
#include "parser.hpp"
#include "indexer.hpp"
#include <set>
#include <queue>
#include <string>

class CrawlManager {
public:
    CrawlManager(Crawler& crawler, Parser& parser, Indexer& indexer);

    // Starts BFS-style crawling from a seed URL
    void crawlFromSeed(const std::string& seedUrl, int maxPages);

private:
    Crawler& crawler;
    Parser& parser;
    Indexer& indexer;

    std::set<std::string> visited;
    std::queue<std::string> toVisit;
};

#endif
