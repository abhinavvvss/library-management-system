#pragma once
#include <string>
#include <iostream>

enum class TransactionType { BORROW, RETURN };

class Transaction {
private:
    int transactionId;
    int memberId;
    int bookId;
    std::string memberName;
    std::string bookTitle;
    std::string borrowDate;
    std::string returnDate;  // empty if not returned
    TransactionType type;
    bool isReturned;

public:
    Transaction() : transactionId(0), memberId(0), bookId(0), isReturned(false),
                    type(TransactionType::BORROW) {}

    Transaction(int tid, int memberId, int bookId,
                const std::string& memberName, const std::string& bookTitle,
                const std::string& borrowDate)
        : transactionId(tid), memberId(memberId), bookId(bookId),
          memberName(memberName), bookTitle(bookTitle),
          borrowDate(borrowDate), isReturned(false),
          type(TransactionType::BORROW) {}

    // Getters
    int getTransactionId() const { return transactionId; }
    int getMemberId() const { return memberId; }
    int getBookId() const { return bookId; }
    std::string getMemberName() const { return memberName; }
    std::string getBookTitle() const { return bookTitle; }
    std::string getBorrowDate() const { return borrowDate; }
    std::string getReturnDate() const { return returnDate; }
    bool getIsReturned() const { return isReturned; }
    TransactionType getType() const { return type; }

    void markReturned(const std::string& date) {
        returnDate = date;
        isReturned = true;
        type = TransactionType::RETURN;
    }

    void display() const {
        std::cout << "Transaction #" << transactionId << "\n"
                  << "Member: " << memberName << " (ID:" << memberId << ")\n"
                  << "Book: " << bookTitle << " (ID:" << bookId << ")\n"
                  << "Borrowed: " << borrowDate << "\n"
                  << "Returned: " << (isReturned ? returnDate : "Not yet") << "\n"
                  << "Status: " << (isReturned ? "RETURNED" : "ACTIVE") << "\n";
    }

    std::string serialize() const;
    static Transaction deserialize(const std::string& data);
};
