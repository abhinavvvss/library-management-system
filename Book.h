#pragma once
#include <string>
#include <iostream>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    std::string isbn;
    std::string genre;
    int year;
    int totalCopies;
    int availableCopies;

public:
    Book() : id(0), year(0), totalCopies(0), availableCopies(0) {}

    Book(int id, const std::string& title, const std::string& author,
         const std::string& isbn, const std::string& genre,
         int year, int totalCopies)
        : id(id), title(title), author(author), isbn(isbn),
          genre(genre), year(year), totalCopies(totalCopies),
          availableCopies(totalCopies) {}

    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getIsbn() const { return isbn; }
    std::string getGenre() const { return genre; }
    int getYear() const { return year; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }

    // Setters
    void setTitle(const std::string& t) { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setIsbn(const std::string& i) { isbn = i; }
    void setGenre(const std::string& g) { genre = g; }
    void setYear(int y) { year = y; }
    void setTotalCopies(int c) { totalCopies = c; }
    void setAvailableCopies(int c) { availableCopies = c; }

    bool isAvailable() const { return availableCopies > 0; }

    bool borrowCopy() {
        if (availableCopies > 0) {
            availableCopies--;
            return true;
        }
        return false;
    }

    void returnCopy() {
        if (availableCopies < totalCopies) {
            availableCopies++;
        }
    }

    void display() const {
        std::cout << "ID: " << id << "\n"
                  << "Title: " << title << "\n"
                  << "Author: " << author << "\n"
                  << "ISBN: " << isbn << "\n"
                  << "Genre: " << genre << "\n"
                  << "Year: " << year << "\n"
                  << "Available: " << availableCopies << "/" << totalCopies << "\n";
    }

    // Serialization
    std::string serialize() const {
        return std::to_string(id) + "|" + title + "|" + author + "|" +
               isbn + "|" + genre + "|" + std::to_string(year) + "|" +
               std::to_string(totalCopies) + "|" + std::to_string(availableCopies);
    }

    static Book deserialize(const std::string& data);
};
