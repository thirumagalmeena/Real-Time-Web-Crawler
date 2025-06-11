#include "crawler.hpp"
#include <iostream>
#include <curl/curl.h>
#include <regex>

// Callback function for libcurl to write data into string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Page Crawler::fetchPage(const std::string& url) {
    Page page;
    page.url = url;

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
        return page;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Follow redirects

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Curl failed: " << curl_easy_strerror(res) << "\n";
    } else {
        page.html = readBuffer;

        // Very simple regex-based anchor extraction
        std::regex hrefRegex("<a\\s+(?:[^>]*?\\s+)?href=[\"']([^\"']*)[\"']");
        std::smatch match;
        std::string::const_iterator searchStart(readBuffer.cbegin());

        while (std::regex_search(searchStart, readBuffer.cend(), match, hrefRegex)) {
            std::string link = match[1];
            if (!link.empty()) {
                page.links.push_back(link);
            }
            searchStart = match.suffix().first;
        }
    }

    curl_easy_cleanup(curl);
    return page;
}
