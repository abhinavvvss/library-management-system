#include "../include/Transaction.h"
#include <sstream>
#include <vector>

std::string Transaction::serialize() const {
    return std::to_string(transactionId) + "|" +
           std::to_string(memberId) + "|" +
           std::to_string(bookId) + "|" +
           memberName + "|" +
           bookTitle + "|" +
           borrowDate + "|" +
           returnDate + "|" +
           (isReturned ? "1" : "0");
}

Transaction Transaction::deserialize(const std::string& data) {
    Transaction t;
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, '|')) {
        parts.push_back(token);
    }

    if (parts.size() >= 8) {
        t.transactionId = std::stoi(parts[0]);
        t.memberId      = std::stoi(parts[1]);
        t.bookId        = std::stoi(parts[2]);
        t.memberName    = parts[3];
        t.bookTitle     = parts[4];
        t.borrowDate    = parts[5];
        t.returnDate    = parts[6];
        t.isReturned    = (parts[7] == "1");
        t.type          = t.isReturned ? TransactionType::RETURN : TransactionType::BORROW;
    }
    return t;
}
