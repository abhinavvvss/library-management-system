#include "../include/Library.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <sys/stat.h>

// ─── Constructor ─────────────────────────────────────────────────────────────
Library::Library() : nextBookId(1), nextMemberId(1), nextTransactionId(1) {
    // Create data directory if not exists
    mkdir("data", 0777);
    loadBooks();
    loadMembers();
    loadTransactions();

    // Determine next IDs from loaded data
    for (const auto& b : books)
        if (b.getId() >= nextBookId) nextBookId = b.getId() + 1;
    for (const auto& m : members)
        if (m.getId() >= nextMemberId) nextMemberId = m.getId() + 1;
    for (const auto& t : transactions)
        if (t.getTransactionId() >= nextTransactionId)
            nextTransactionId = t.getTransactionId() + 1;
}

// ─── Date Helper ─────────────────────────────────────────────────────────────
std::string Library::getCurrentDate() const {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return std::string(buf);
}

// ─── Index Finders ───────────────────────────────────────────────────────────
int Library::findBookIndex(int bookId) const {
    for (int i = 0; i < (int)books.size(); i++)
        if (books[i].getId() == bookId) return i;
    return -1;
}

int Library::findMemberIndex(int memberId) const {
    for (int i = 0; i < (int)members.size(); i++)
        if (members[i].getId() == memberId) return i;
    return -1;
}

int Library::findActiveTransactionIndex(int memberId, int bookId) const {
    for (int i = 0; i < (int)transactions.size(); i++) {
        const auto& t = transactions[i];
        if (t.getMemberId() == memberId && t.getBookId() == bookId && !t.getIsReturned())
            return i;
    }
    return -1;
}

// ─── File I/O ────────────────────────────────────────────────────────────────
void Library::saveBooks() const {
    std::ofstream f(BOOKS_FILE);
    for (const auto& b : books) f << b.serialize() << "\n";
}

void Library::saveMembers() const {
    std::ofstream f(MEMBERS_FILE);
    for (const auto& m : members) f << m.serialize() << "\n";
}

void Library::saveTransactions() const {
    std::ofstream f(TRANSACTIONS_FILE);
    for (const auto& t : transactions) f << t.serialize() << "\n";
}

void Library::loadBooks() {
    std::ifstream f(BOOKS_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) books.push_back(Book::deserialize(line));
}

void Library::loadMembers() {
    std::ifstream f(MEMBERS_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) members.push_back(Member::deserialize(line));
}

void Library::loadTransactions() {
    std::ifstream f(TRANSACTIONS_FILE);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) transactions.push_back(Transaction::deserialize(line));
}

// ─── Book Operations ──────────────────────────────────────────────────────────
bool Library::addBook(const std::string& title, const std::string& author,
                      const std::string& isbn, const std::string& genre,
                      int year, int copies) {
    // Check duplicate ISBN
    for (const auto& b : books)
        if (b.getIsbn() == isbn) {
            std::cout << "Error: Book with ISBN " << isbn << " already exists.\n";
            return false;
        }
    books.emplace_back(nextBookId++, title, author, isbn, genre, year, copies);
    saveBooks();
    return true;
}

bool Library::removeBook(int bookId) {
    int idx = findBookIndex(bookId);
    if (idx == -1) { std::cout << "Book not found.\n"; return false; }
    if (books[idx].getTotalCopies() != books[idx].getAvailableCopies()) {
        std::cout << "Cannot remove: some copies are still borrowed.\n";
        return false;
    }
    books.erase(books.begin() + idx);
    saveBooks();
    return true;
}

bool Library::updateBook(int bookId, const std::string& title, const std::string& author,
                         const std::string& genre, int year, int copies) {
    int idx = findBookIndex(bookId);
    if (idx == -1) { std::cout << "Book not found.\n"; return false; }
    int borrowed = books[idx].getTotalCopies() - books[idx].getAvailableCopies();
    if (copies < borrowed) {
        std::cout << "Cannot set copies below currently borrowed count.\n";
        return false;
    }
    books[idx].setTitle(title);
    books[idx].setAuthor(author);
    books[idx].setGenre(genre);
    books[idx].setYear(year);
    books[idx].setAvailableCopies(copies - borrowed);
    books[idx].setTotalCopies(copies);
    saveBooks();
    return true;
}

Book* Library::findBookById(int bookId) {
    int idx = findBookIndex(bookId);
    return idx == -1 ? nullptr : &books[idx];
}

// Case-insensitive substring search helper
static bool containsCI(const std::string& haystack, const std::string& needle) {
    std::string h = haystack, n = needle;
    std::transform(h.begin(), h.end(), h.begin(), ::tolower);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    return h.find(n) != std::string::npos;
}

std::vector<Book> Library::searchBooksByTitle(const std::string& keyword) const {
    std::vector<Book> res;
    for (const auto& b : books)
        if (containsCI(b.getTitle(), keyword)) res.push_back(b);
    return res;
}

std::vector<Book> Library::searchBooksByAuthor(const std::string& keyword) const {
    std::vector<Book> res;
    for (const auto& b : books)
        if (containsCI(b.getAuthor(), keyword)) res.push_back(b);
    return res;
}

std::vector<Book> Library::searchBooksByGenre(const std::string& genre) const {
    std::vector<Book> res;
    for (const auto& b : books)
        if (containsCI(b.getGenre(), genre)) res.push_back(b);
    return res;
}

std::vector<Book> Library::getAvailableBooks() const {
    std::vector<Book> res;
    for (const auto& b : books)
        if (b.isAvailable()) res.push_back(b);
    return res;
}

// ─── Member Operations ────────────────────────────────────────────────────────
bool Library::addMember(const std::string& name, const std::string& email,
                        const std::string& phone, const std::string& address) {
    for (const auto& m : members)
        if (m.getEmail() == email) {
            std::cout << "Member with email " << email << " already exists.\n";
            return false;
        }
    members.emplace_back(nextMemberId++, name, email, phone, address, getCurrentDate());
    saveMembers();
    return true;
}

bool Library::removeMember(int memberId) {
    int idx = findMemberIndex(memberId);
    if (idx == -1) { std::cout << "Member not found.\n"; return false; }
    if (members[idx].getBorrowedCount() > 0) {
        std::cout << "Cannot remove member with active loans.\n";
        return false;
    }
    members.erase(members.begin() + idx);
    saveMembers();
    return true;
}

bool Library::updateMember(int memberId, const std::string& name,
                           const std::string& email, const std::string& phone,
                           const std::string& address) {
    int idx = findMemberIndex(memberId);
    if (idx == -1) { std::cout << "Member not found.\n"; return false; }
    members[idx].setName(name);
    members[idx].setEmail(email);
    members[idx].setPhone(phone);
    members[idx].setAddress(address);
    saveMembers();
    return true;
}

Member* Library::findMemberById(int memberId) {
    int idx = findMemberIndex(memberId);
    return idx == -1 ? nullptr : &members[idx];
}

std::vector<Member> Library::searchMembersByName(const std::string& keyword) const {
    std::vector<Member> res;
    for (const auto& m : members)
        if (containsCI(m.getName(), keyword)) res.push_back(m);
    return res;
}

// ─── Borrow / Return ──────────────────────────────────────────────────────────
bool Library::borrowBook(int memberId, int bookId) {
    int midx = findMemberIndex(memberId);
    int bidx = findBookIndex(bookId);

    if (midx == -1) { std::cout << "Member not found.\n"; return false; }
    if (bidx == -1) { std::cout << "Book not found.\n"; return false; }

    Member& m = members[midx];
    Book&   b = books[bidx];

    if (!m.canBorrow()) {
        std::cout << "Member has reached max borrow limit (" << m.getMaxBorrowLimit() << ").\n";
        return false;
    }
    if (!b.isAvailable()) {
        std::cout << "No copies available for this book.\n";
        return false;
    }
    if (m.hasBorrowed(bookId)) {
        std::cout << "Member already borrowed this book.\n";
        return false;
    }

    b.borrowCopy();
    m.addBorrowedBook(bookId);

    transactions.emplace_back(nextTransactionId++, memberId, bookId,
                              m.getName(), b.getTitle(), getCurrentDate());
    saveBooks();
    saveMembers();
    saveTransactions();
    std::cout << "Book borrowed successfully!\n";
    return true;
}

bool Library::returnBook(int memberId, int bookId) {
    int midx = findMemberIndex(memberId);
    int bidx = findBookIndex(bookId);
    int tidx = findActiveTransactionIndex(memberId, bookId);

    if (midx == -1) { std::cout << "Member not found.\n"; return false; }
    if (bidx == -1) { std::cout << "Book not found.\n"; return false; }
    if (tidx == -1) { std::cout << "No active loan found for this member/book.\n"; return false; }

    books[bidx].returnCopy();
    members[midx].removeBorrowedBook(bookId);
    transactions[tidx].markReturned(getCurrentDate());

    saveBooks();
    saveMembers();
    saveTransactions();
    std::cout << "Book returned successfully!\n";
    return true;
}

// ─── Transaction Queries ──────────────────────────────────────────────────────
std::vector<Transaction> Library::getActiveTransactions() const {
    std::vector<Transaction> res;
    for (const auto& t : transactions)
        if (!t.getIsReturned()) res.push_back(t);
    return res;
}

std::vector<Transaction> Library::getMemberTransactions(int memberId) const {
    std::vector<Transaction> res;
    for (const auto& t : transactions)
        if (t.getMemberId() == memberId) res.push_back(t);
    return res;
}

std::vector<Transaction> Library::getBookTransactions(int bookId) const {
    std::vector<Transaction> res;
    for (const auto& t : transactions)
        if (t.getBookId() == bookId) res.push_back(t);
    return res;
}

// ─── Statistics ───────────────────────────────────────────────────────────────
int Library::getActiveLoansCount() const {
    int c = 0;
    for (const auto& t : transactions)
        if (!t.getIsReturned()) c++;
    return c;
}

int Library::getTotalCopies() const {
    int c = 0;
    for (const auto& b : books) c += b.getTotalCopies();
    return c;
}

int Library::getTotalAvailableCopies() const {
    int c = 0;
    for (const auto& b : books) c += b.getAvailableCopies();
    return c;
}

std::map<std::string, int> Library::getGenreDistribution() const {
    std::map<std::string, int> dist;
    for (const auto& b : books) dist[b.getGenre()]++;
    return dist;
}

std::vector<std::pair<Book, int>> Library::getMostBorrowedBooks(int topN) const {
    // Count borrows per book
    std::map<int, int> borrowCount;
    for (const auto& t : transactions) borrowCount[t.getBookId()]++;

    std::vector<std::pair<Book, int>> result;
    for (const auto& b : books) {
        int count = borrowCount.count(b.getId()) ? borrowCount[b.getId()] : 0;
        result.push_back({b, count});
    }

    std::sort(result.begin(), result.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    if ((int)result.size() > topN) result.resize(topN);
    return result;
}

void Library::displayStats() const {
    std::cout << "\n========== LIBRARY STATISTICS ==========\n";
    std::cout << "Total Books (titles):    " << getTotalBooks() << "\n";
    std::cout << "Total Copies:            " << getTotalCopies() << "\n";
    std::cout << "Available Copies:        " << getTotalAvailableCopies() << "\n";
    std::cout << "Total Members:           " << getTotalMembers() << "\n";
    std::cout << "Active Loans:            " << getActiveLoansCount() << "\n";
    std::cout << "Total Transactions:      " << getTotalTransactions() << "\n";

    std::cout << "\n--- Genre Distribution ---\n";
    for (const auto& [genre, count] : getGenreDistribution())
        std::cout << "  " << genre << ": " << count << " title(s)\n";

    std::cout << "\n--- Top 5 Most Borrowed Books ---\n";
    int rank = 1;
    for (const auto& [book, count] : getMostBorrowedBooks(5))
        std::cout << "  " << rank++ << ". " << book.getTitle()
                  << " by " << book.getAuthor()
                  << " [" << count << " borrow(s)]\n";
    std::cout << "=========================================\n";
}

void Library::displayAllBooks() const {
    std::cout << "\n========== ALL BOOKS ==========\n";
    for (const auto& b : books) { b.display(); std::cout << "-----\n"; }
}

void Library::displayAllMembers() const {
    std::cout << "\n========== ALL MEMBERS ==========\n";
    for (const auto& m : members) { m.display(); std::cout << "-----\n"; }
}

void Library::displayAllTransactions() const {
    std::cout << "\n========== ALL TRANSACTIONS ==========\n";
    for (const auto& t : transactions) { t.display(); std::cout << "-----\n"; }
}
