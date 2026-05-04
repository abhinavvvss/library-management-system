#include "../include/Library.h"
#include <iostream>
#include <string>
#include <limits>

Library lib;

// ─── Helpers ──────────────────────────────────────────────────────────────────
void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int readInt(const std::string& prompt) {
    int n;
    std::cout << prompt;
    std::cin >> n;
    clearInput();
    return n;
}

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// ─── Menus ────────────────────────────────────────────────────────────────────
void bookMenu() {
    while (true) {
        std::cout << "\n===== BOOK MANAGEMENT =====\n"
                  << "1. Add Book\n"
                  << "2. Remove Book\n"
                  << "3. Update Book\n"
                  << "4. View All Books\n"
                  << "5. Search Books\n"
                  << "6. View Available Books\n"
                  << "0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        if (ch == 0) break;
        else if (ch == 1) {
            std::string title  = readLine("Title: ");
            std::string author = readLine("Author: ");
            std::string isbn   = readLine("ISBN: ");
            std::string genre  = readLine("Genre: ");
            int year           = readInt("Year: ");
            int copies         = readInt("Copies: ");
            lib.addBook(title, author, isbn, genre, year, copies)
                ? std::cout << "Book added!\n"
                : std::cout << "Failed.\n";
        }
        else if (ch == 2) {
            int id = readInt("Book ID to remove: ");
            lib.removeBook(id);
        }
        else if (ch == 3) {
            int id             = readInt("Book ID to update: ");
            std::string title  = readLine("New Title: ");
            std::string author = readLine("New Author: ");
            std::string genre  = readLine("New Genre: ");
            int year           = readInt("New Year: ");
            int copies         = readInt("New Total Copies: ");
            lib.updateBook(id, title, author, genre, year, copies)
                ? std::cout << "Updated!\n"
                : std::cout << "Failed.\n";
        }
        else if (ch == 4) {
            lib.displayAllBooks();
        }
        else if (ch == 5) {
            std::cout << "Search by: 1.Title  2.Author  3.Genre\nChoice: ";
            int s; std::cin >> s; clearInput();
            std::string kw = readLine("Keyword: ");
            std::vector<Book> results;
            if (s == 1) results = lib.searchBooksByTitle(kw);
            else if (s == 2) results = lib.searchBooksByAuthor(kw);
            else results = lib.searchBooksByGenre(kw);
            std::cout << results.size() << " result(s):\n";
            for (auto& b : results) { b.display(); std::cout << "---\n"; }
        }
        else if (ch == 6) {
            auto avail = lib.getAvailableBooks();
            std::cout << avail.size() << " available:\n";
            for (auto& b : avail) { b.display(); std::cout << "---\n"; }
        }
        pause();
    }
}

void memberMenu() {
    while (true) {
        std::cout << "\n===== MEMBER MANAGEMENT =====\n"
                  << "1. Add Member\n"
                  << "2. Remove Member\n"
                  << "3. Update Member\n"
                  << "4. View All Members\n"
                  << "5. Search Members\n"
                  << "0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        if (ch == 0) break;
        else if (ch == 1) {
            std::string name    = readLine("Name: ");
            std::string email   = readLine("Email: ");
            std::string phone   = readLine("Phone: ");
            std::string address = readLine("Address: ");
            lib.addMember(name, email, phone, address)
                ? std::cout << "Member added!\n"
                : std::cout << "Failed.\n";
        }
        else if (ch == 2) {
            int id = readInt("Member ID to remove: ");
            lib.removeMember(id);
        }
        else if (ch == 3) {
            int id              = readInt("Member ID to update: ");
            std::string name    = readLine("New Name: ");
            std::string email   = readLine("New Email: ");
            std::string phone   = readLine("New Phone: ");
            std::string address = readLine("New Address: ");
            lib.updateMember(id, name, email, phone, address)
                ? std::cout << "Updated!\n"
                : std::cout << "Failed.\n";
        }
        else if (ch == 4) {
            lib.displayAllMembers();
        }
        else if (ch == 5) {
            std::string kw = readLine("Name keyword: ");
            auto results = lib.searchMembersByName(kw);
            std::cout << results.size() << " result(s):\n";
            for (auto& m : results) { m.display(); std::cout << "---\n"; }
        }
        pause();
    }
}

void loanMenu() {
    while (true) {
        std::cout << "\n===== BORROW / RETURN =====\n"
                  << "1. Borrow Book\n"
                  << "2. Return Book\n"
                  << "3. View Active Loans\n"
                  << "4. Member Loan History\n"
                  << "5. Book Loan History\n"
                  << "0. Back\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        if (ch == 0) break;
        else if (ch == 1) {
            int mid = readInt("Member ID: ");
            int bid = readInt("Book ID: ");
            lib.borrowBook(mid, bid);
        }
        else if (ch == 2) {
            int mid = readInt("Member ID: ");
            int bid = readInt("Book ID: ");
            lib.returnBook(mid, bid);
        }
        else if (ch == 3) {
            auto active = lib.getActiveTransactions();
            std::cout << active.size() << " active loan(s):\n";
            for (auto& t : active) { t.display(); std::cout << "---\n"; }
        }
        else if (ch == 4) {
            int mid = readInt("Member ID: ");
            auto hist = lib.getMemberTransactions(mid);
            std::cout << hist.size() << " transaction(s):\n";
            for (auto& t : hist) { t.display(); std::cout << "---\n"; }
        }
        else if (ch == 5) {
            int bid = readInt("Book ID: ");
            auto hist = lib.getBookTransactions(bid);
            std::cout << hist.size() << " transaction(s):\n";
            for (auto& t : hist) { t.display(); std::cout << "---\n"; }
        }
        pause();
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "╔══════════════════════════════════╗\n"
              << "║   LIBRARY MANAGEMENT SYSTEM      ║\n"
              << "╚══════════════════════════════════╝\n";

    while (true) {
        std::cout << "\n===== MAIN MENU =====\n"
                  << "1. Book Management\n"
                  << "2. Member Management\n"
                  << "3. Borrow / Return\n"
                  << "4. Statistics\n"
                  << "5. All Transactions\n"
                  << "0. Exit\n"
                  << "Choice: ";
        int ch; std::cin >> ch; clearInput();

        if      (ch == 0) { std::cout << "Goodbye!\n"; break; }
        else if (ch == 1) bookMenu();
        else if (ch == 2) memberMenu();
        else if (ch == 3) loanMenu();
        else if (ch == 4) { lib.displayStats(); pause(); }
        else if (ch == 5) { lib.displayAllTransactions(); pause(); }
        else std::cout << "Invalid choice.\n";
    }
    return 0;
}
