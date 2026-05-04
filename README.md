# LibraCore — Library Management System

A full-featured Library Management System built with **C++** for the backend core and **HTML/CSS/JS** for the frontend interface.

---

## 📁 Project Structure

```
library_management/
├── include/
│   ├── Book.h            # Book class with OOP encapsulation
│   ├── Member.h          # Member class with borrow tracking
│   ├── Transaction.h     # Transaction/borrow-record class
│   └── Library.h         # Core Library management class
├── src/
│   ├── Book.cpp          # Book serialization/deserialization
│   ├── Member.cpp        # Member serialization/deserialization
│   ├── Transaction.cpp   # Transaction serialization/deserialization
│   ├── Library.cpp       # Full business logic (CRUD, search, stats)
│   └── main.cpp          # CLI menu-driven interface
├── data/                 # Auto-created: books.txt, members.txt, transactions.txt
├── index.html            # HTML frontend interface
├── Makefile
└── README.md
```

---

## 🚀 Build & Run (C++ CLI)

```bash
# Compile
make

# Run
./library_system

# Or both
make run

# Clean
make clean
```

**Requirements:** g++ with C++17 support

---

## ✨ Features

### Book Management
- Add, edit, delete books with duplicate ISBN detection
- Track total copies vs. available copies
- Search by title, author, or genre (case-insensitive)
- Filter available books

### Member Management
- Register members with unique email enforcement
- Track up to 3 simultaneous borrows per member
- Edit member details; safe delete (blocks if active loans)

### Borrow / Return Workflow
- Validates: member exists, book available, quota not exceeded, not duplicate borrow
- Marks return date; restores copy count
- Full history preserved

### File-Based Persistence (C++)
- All data auto-saved to `data/` folder in pipe-delimited format
- Loaded on startup — data survives program restarts
- Mirrors a real database without external dependencies

### Statistics
- Genre distribution
- Most-borrowed books ranking
- Active loan count, available copies, totals

---

## 🗄️ Data Format (File Persistence)

**books.txt:**  `id|title|author|isbn|genre|year|totalCopies|availableCopies`

**members.txt:** `id|name|email|phone|address|joinDate|maxBorrow|borrowedBookIds`

**transactions.txt:** `id|memberId|bookId|memberName|bookTitle|borrowDate|returnDate|isReturned`

---

## 🏗️ OOP Design

| Class | Responsibility |
|---|---|
| `Book` | Encapsulates book data, copy tracking, serialization |
| `Member` | Member data, borrow list management, quota checks |
| `Transaction` | Immutable borrow/return record with state |
| `Library` | Orchestrates all operations, file I/O, search, stats |

---

## 🌐 HTML Frontend

Open `index.html` in any browser. Features:
- Dashboard with live statistics and charts
- Full CRUD for books and members
- Borrow/return forms with validation previews
- Transaction history with filters
- Global search

---

## 🔧 Tech Stack

- **C++17** — Core logic, OOP, file handling
- **STL** — `vector`, `map`, `algorithm`, `fstream`
- **DSA** — Linear search, sorting for rankings, map for genre grouping
- **HTML5 / CSS3 / Vanilla JS** — Frontend interface
- **Google Fonts** — Playfair Display, DM Sans, DM Mono
