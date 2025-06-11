#include "parser.hpp"
#include <regex>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

std::unordered_set<std::string> urlNoise = {
    "http", "https", "www", "com", "org", "net", "edu", "gov", "io", 
    "html", "htm", "php", "asp", "aspx", "jsp", "cgi", "index",
    "http", "https", "ftp", "mailto", "tel", "javascript"
};

std::unordered_set<std::string> cssNoise = {
    "html", "head", "body", "div", "span", "p", "a", "img", "table", "tr", "td",
    "ul", "ol", "li", "h1", "h2", "h3", "h4", "h5", "h6", "form", "input", "button",
    
    "color", "background", "font", "size", "width", "height", "margin", "padding", 
    "border", "display", "position", "float", "clear", "visibility", "opacity",
    "text", "align", "decoration", "transform", "transition", "animation",
    "flex", "grid", "box", "shadow", "radius", "overflow", "zindex",
    
    "px", "em", "rem", "vh", "vw", "vmin", "vmax", "rgb", "rgba", "hsl", "hsla",
    "calc", "var", "url", "linear-gradient", "radial-gradient",
    
    "arial", "helvetica", "times", "courier", "verdana", "georgia", "palatino",
    "blinkmacsystemfont", "system", "ui", "segoe", "roboto", "monospace",
    
    "hover", "active", "focus", "visited", "link", "before", "after", "first", "last",
    "nth", "child", "type", "class", "id", "src", "href", "alt", "title", "style",
    
    "none", "block", "inline", "hidden", "visible", "auto", "inherit", "initial",
    "unset", "static", "relative", "absolute", "fixed", "sticky", "left", "right",
    "top", "bottom", "center", "middle", "justify", "bold", "italic", "underline"
};

std::unordered_set<std::string> htmlEntities = {
    "nbsp", "amp", "lt", "gt", "quot", "copy", "reg", "trade", "mdash", "ndash"
};

std::string Parser::extractText(const std::string& html) {
    std::string noComments = std::regex_replace(
        html,
        std::regex("<!--.*?-->", std::regex_constants::icase),
        ""
    );

    std::string noScript = std::regex_replace(
        noComments,
        std::regex("<script[^>]*>(.|\\s)*?</script>", std::regex_constants::icase),
        ""
    );

    std::string noStyle = std::regex_replace(
        noScript,
        std::regex("<style[^>]*>(.|\\s)*?</style>", std::regex_constants::icase),
        ""
    );

    std::string noTags = std::regex_replace(
        noStyle,
        std::regex("<[^>]+>"),
        " "
    );

    std::string noEntities = std::regex_replace(
        noTags,
        std::regex("&[^;]+;"),
        " "
    );

    std::string cleanText = std::regex_replace(
        noEntities,
        std::regex("\\s+"),
        " "
    );

    cleanText.erase(0, cleanText.find_first_not_of(" \t\n\r\f\v"));
    cleanText.erase(cleanText.find_last_not_of(" \t\n\r\f\v") + 1);

    return cleanText;
}

std::vector<std::string> Parser::extractKeywordsFromUrl(const std::string& url) {
    std::string cleanUrl = std::regex_replace(url, std::regex("^https?://(www\\.)?"), "");
    cleanUrl = std::regex_replace(cleanUrl, std::regex("\\?.*$"), "");
    
    cleanUrl = std::regex_replace(cleanUrl, std::regex("[/\\?&=#_\\-\\.:+%]"), " ");
    
    std::vector<std::string> tokens;
    std::istringstream iss(cleanUrl);
    std::string word;
    
    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c) {
            return std::ispunct(c);
        }), word.end());

        if (word.length() >= 3 && 
            std::all_of(word.begin(), word.end(), ::isalpha) &&
            urlNoise.find(word) == urlNoise.end()) {
            tokens.push_back(word);
        }
    }
    
    return tokens;
}

std::vector<std::string> Parser::tokenize(const std::string& text) {
    std::unordered_set<std::string> stopWords = {
        "the", "and", "is", "in", "at", "of", "a", "to", "on", "for", "with", "by",
        "as", "this", "that", "it", "an", "be", "are", "or", "was", "if", "from", "but",
        "not", "they", "we", "you", "he", "she", "his", "her", "their", "our", "my", "your",
        "them", "us", "me", "him", "has", "have", "had", "do", "does", "did", "can", "could",
        "would", "should", "will", "shall", "may", "might", "must", "here", "there", "when",
        "where", "why", "how", "what", "which", "who", "whom", "whose", "these", "those"
    };

    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c) {
            return !std::isalpha(c);
        }), word.end());

        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (word.length() >= 3 &&
            stopWords.find(word) == stopWords.end() &&
            cssNoise.find(word) == cssNoise.end() &&
            word.find("webkit") == std::string::npos &&
            word.find("moz") == std::string::npos &&
            word.find("ms") == std::string::npos) {
            tokens.push_back(word);
        }
    }

    return tokens;
}

std::map<std::string, int> Parser::countKeywords(const std::vector<std::string>& words) {
    std::map<std::string, int> freq;
    for (const std::string& word : words) {
        freq[word]++;
    }
    return freq;
}