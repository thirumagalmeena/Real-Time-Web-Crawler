# Real-Time Web Crawler with B+ Tree Inverted Index

A system-level C++ project that crawls web pages in real-time, extracts keywords, and builds a searchable inverted index using a custom-built B+ Tree. It mimics core functionality of a search engine like keyword-based indexing, ranking, and persistent search.

---

## Features

- Real-time web crawling using `libcurl`  
- Custom B+ Tree implementation (for indexing)  
- HTML parsing + keyword extraction (including from URLs)  
- Inverted index: `keyword → list of (URL, frequency)`  
- CLI menu to `crawl`, `search`, `print`, `save`, `load`  
- Supports persistence via `index.txt`  
- Frequency-based ranking in search results  

---

## Tech Stack

- **Language:** C++17  
- **Libraries:** [libcurl](https://curl.se/libcurl/) for web requests, STL for parsing  
- **Data Structure:** B+ Tree
- **Build Tool:** g++  

---

## Setup & Build

### Prerequisites

Install `libcurl` if not already:

\`\`\`bash
sudo apt update
sudo apt install libcurl4-openssl-dev
\`\`\`

### Build the project

\`\`\`bash
g++ -std=c++17 -Iinclude src/*.cpp -lcurl -o crawler
\`\`\`

### Run the crawler

\`\`\`bash
./crawler
\`\`\`

---

## CLI Menu

\`\`\`bash
1. crawl <url> <max_pages>    # Crawl and index pages
2. search <keyword>           # Search for a word across pages
3. print                      # View all indexed keywords and URLs
4. exit                       # Quit the program
5. save <filename>            # Save current index to file
6. load <filename>            # Load index from file
\`\`\`

---

## Example Session

\`\`\`bash
> crawl https://example.com 3
Crawling: https://example.com
Crawling: https://www.iana.org/domains/example

> search example
'example' found in:
 - https://example.com (x1)
 - https://www.iana.org/domains/example (x1)

> save index.txt
Index saved to 'index.txt'

> exit

--- (restart) ---

> load index.txt
Index loaded from 'index.txt'

> search example
'example' found in:
 - https://example.com (x1)
 - https://www.iana.org/domains/example (x1)

# License
- This project is licensed for educational purposes only.

---

## Contributor

- **Thirumagal Meena A ** 
  Applied Mathematics and Computational Sciences
  Psg College of Technology