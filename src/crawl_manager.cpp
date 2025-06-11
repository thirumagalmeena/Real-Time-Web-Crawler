#include "crawl_manager.hpp"
#include <iostream>

CrawlManager::CrawlManager(Crawler& crawler, Parser& parser, Indexer& indexer)
    : crawler(crawler), parser(parser), indexer(indexer) {}

void CrawlManager::crawlFromSeed(const std::string& seedUrl, int maxPages) {
    toVisit.push(seedUrl);
    visited.insert(seedUrl);

    int pagesCrawled = 0;

    while (!toVisit.empty() && pagesCrawled < maxPages) {
        std::string url = toVisit.front();
        toVisit.pop();

        std::cout << "\nCrawling: " << url << "\n";

        Page page = crawler.fetchPage(url);
        if (page.html.empty()) continue;

        // Extract and index from URL
        auto urlWords = parser.extractKeywordsFromUrl(url);
        auto urlFreq = parser.countKeywords(urlWords);
        indexer.indexKeywords(url, urlFreq);

        // Extract and index from content
        std::string text = parser.extractText(page.html);
        auto tokens = parser.tokenize(text);
        auto contentFreq = parser.countKeywords(tokens);
        indexer.indexKeywords(url, contentFreq);

        // Add new links to queue
        for (const std::string& link : page.links) {
            if (visited.count(link) == 0 && link.find("http") == 0) {
                visited.insert(link);
                toVisit.push(link);
            }
        }

        pagesCrawled++;
    }

    std::cout << "\nCrawling complete. Pages crawled: " << pagesCrawled << "\n";
}
