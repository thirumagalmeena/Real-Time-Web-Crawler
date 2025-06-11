#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <map>
#include <unordered_set>

class Parser {
public:
    // Extract visible text from HTML (removes tags, scripts, styles)
    std::string extractText(const std::string& html);

    // Extract keywords from URL
    std::vector<std::string> extractKeywordsFromUrl(const std::string& url);

    // Tokenize text into lowercase words (excluding stop words)
    std::vector<std::string> tokenize(const std::string& text);

    // Build a frequency map of words
    std::map<std::string, int> countKeywords(const std::vector<std::string>& words);
};

#endif  // PARSER_HPP
