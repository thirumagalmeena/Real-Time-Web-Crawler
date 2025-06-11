#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <string>
#include <vector>

struct Page {
    std::string url;
    std::string html;
    std::vector<std::string> links;
};

class Crawler {
public:
    Page fetchPage(const std::string& url);
};

#endif  // CRAWLER_HPP
