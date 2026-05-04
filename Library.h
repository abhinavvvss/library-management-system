#pragma once
#include "Book.h"
#include "Member.h"
#include "Transaction.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Transaction> transactions;

    int nextBookId;
    int nextMemberId;
    int nextTransactionId;

    // File paths
    const std::string BOOKS_FILE = "data/books.txt";
    const std::string MEMBERS_FILE = "data/members.txt";
    const std::string TRANSACTIONS_FILE = "data/transactions.txt";

    // Helper: find index
    int findBookIndex(int bookId) const;
    int findMemberIndex(int memberId) const;
    int findActiveTransactionIndex(int memberId, int bookId) const;

    // File I/O helpers
    void saveBooks() const;
    void saveMembers() const;
    void saveTransactions() const;
    void loadBooks();
    void loadMembers();
    void loadTransactions();

    // Get current date as string
    std::string getCurrentDate() const;

public:
    Library();

    // Book operations
    bool addBook(const std::string& title, const std::string& author,
                 const std::string& isbn, const std::string& genre,
                 int year, int copies);
    bool removeBook(int bookId);
    bool updateBook(int bookId, const std::string& title, const std::string& author,
                    const std::string& genre, int year, int copies);
    Book* findBookById(int bookId);
    std::vector<Book> searchBooksByTitle(const std::string& keyword) const;
    std::vector<Book> searchBooksByAuthor(const std::string& keyword) const;
    std::vector<Book> searchBooksByGenre(const std::string& genre) const;
    std::vector<Book> getAllBooks() const { return books; }
    std::vector<Book> getAvailableBooks() const;

    // Member operations
    bool addMember(const std::string& name, const std::string& email,
                   const std::string& phone, const std::string& address);
    bool removeMember(int memberId);
    bool updateMember(int memberId, const std::string& name,
                      const std::string& email, const std::string& phone,
                      const std::string& address);
    Member* findMemberById(int memberId);
    std::vector<Member> searchMembersByName(const std::string& keyword) const;
    std::vector<Member> getAllMembers() const { return members; }

    // Borrow / Return
    bool borrowBook(int memberId, int bookId);
    bool returnBook(int memberId, int bookId);

    // Transactions
    std::vector<Transaction> getAllTransactions() const { return transactions; }
    std::vector<Transaction> getActiveTransactions() const;
    std::vector<Transaction> getMemberTransactions(int memberId) const;
    std::vector<Transaction> getBookTransactions(int bookId) const;

    // Statistics
    int getTotalBooks() const { return (int)books.size(); }
    int getTotalMembers() const { return (int)members.size(); }
    int getTotalTransactions() const { return (int)transactions.size(); }
    int getActiveLoansCount() const;
    int getTotalCopies() const;
    int getTotalAvailableCopies() const;
    std::map<std::string, int> getGenreDistribution() const;
    std::vector<std::pair<Book, int>> getMostBorrowedBooks(int topN = 5) const;

    void displayStats() const;
    void displayAllBooks() const;
    void displayAllMembers() const;
    void displayAllTransactions() const;
};
