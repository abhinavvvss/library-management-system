#include "../include/Book.h"
#include <sstream>
#include <vector>

Book Book::deserialize(const std::string& data) {
    Book b;
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, '|')) {
        parts.push_back(token);
    }

    if (parts.size() >= 8) {
        b.id             = std::stoi(parts[0]);
        b.title          = parts[1];
        b.author         = parts[2];
        b.isbn           = parts[3];
        b.genre          = parts[4];
        b.year           = std::stoi(parts[5]);
        b.totalCopies    = std::stoi(parts[6]);
        b.availableCopies = std::stoi(parts[7]);
    }
    return b;
}
