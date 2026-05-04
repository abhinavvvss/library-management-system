#pragma once
#include <string>
#include <vector>
#include <iostream>

class Member {
private:
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    std::string membershipDate;
    int maxBorrowLimit;
    std::vector<int> borrowedBookIds;  // book IDs currently borrowed

public:
    Member() : id(0), maxBorrowLimit(3) {}

    Member(int id, const std::string& name, const std::string& email,
           const std::string& phone, const std::string& address,
           const std::string& membershipDate)
        : id(id), name(name), email(email), phone(phone),
          address(address), membershipDate(membershipDate), maxBorrowLimit(3) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    std::string getMembershipDate() const { return membershipDate; }
    int getMaxBorrowLimit() const { return maxBorrowLimit; }
    std::vector<int> getBorrowedBookIds() const { return borrowedBookIds; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setEmail(const std::string& e) { email = e; }
    void setPhone(const std::string& p) { phone = p; }
    void setAddress(const std::string& a) { address = a; }
    void setMaxBorrowLimit(int l) { maxBorrowLimit = l; }

    bool canBorrow() const {
        return (int)borrowedBookIds.size() < maxBorrowLimit;
    }

    int getBorrowedCount() const { return (int)borrowedBookIds.size(); }

    bool addBorrowedBook(int bookId) {
        if (!canBorrow()) return false;
        borrowedBookIds.push_back(bookId);
        return true;
    }

    bool removeBorrowedBook(int bookId) {
        for (auto it = borrowedBookIds.begin(); it != borrowedBookIds.end(); ++it) {
            if (*it == bookId) {
                borrowedBookIds.erase(it);
                return true;
            }
        }
        return false;
    }

    bool hasBorrowed(int bookId) const {
        for (int id : borrowedBookIds) {
            if (id == bookId) return true;
        }
        return false;
    }

    void display() const {
        std::cout << "ID: " << id << "\n"
                  << "Name: " << name << "\n"
                  << "Email: " << email << "\n"
                  << "Phone: " << phone << "\n"
                  << "Address: " << address << "\n"
                  << "Member Since: " << membershipDate << "\n"
                  << "Borrowed: " << borrowedBookIds.size() << "/" << maxBorrowLimit << "\n";
    }

    std::string serialize() const;
    static Member deserialize(const std::string& data);
};
