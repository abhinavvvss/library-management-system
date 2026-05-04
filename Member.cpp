#include "../include/Member.h"
#include <sstream>

std::string Member::serialize() const {
    std::string borrowed = "";
    for (size_t i = 0; i < borrowedBookIds.size(); i++) {
        if (i > 0) borrowed += ",";
        borrowed += std::to_string(borrowedBookIds[i]);
    }
    return std::to_string(id) + "|" + name + "|" + email + "|" + phone + "|" +
           address + "|" + membershipDate + "|" + std::to_string(maxBorrowLimit) + "|" + borrowed;
}

Member Member::deserialize(const std::string& data) {
    Member m;
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, '|')) {
        parts.push_back(token);
    }

    if (parts.size() >= 7) {
        m.id             = std::stoi(parts[0]);
        m.name           = parts[1];
        m.email          = parts[2];
        m.phone          = parts[3];
        m.address        = parts[4];
        m.membershipDate = parts[5];
        m.maxBorrowLimit = std::stoi(parts[6]);

        if (parts.size() >= 8 && !parts[7].empty()) {
            std::stringstream ss2(parts[7]);
            std::string bid;
            while (std::getline(ss2, bid, ',')) {
                if (!bid.empty()) {
                    m.borrowedBookIds.push_back(std::stoi(bid));
                }
            }
        }
    }
    return m;
}
