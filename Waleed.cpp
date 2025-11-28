#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

// Structure
struct Borrower {
    string name;
    string address;
    string contact;
    string borrowedBooks[10];
    int borrowedCount = 0;
};

struct Book {
    string title;
    string author;
    string ISBN;
    int totalCopies;
    int availableCopies;
};

void borrowBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount);
void returnBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount);
void loadBooks(Book books[], int &count);
void saveBooks(Book books[], int count);
void loadBorrowers(Borrower borrowers[], int &count);
void saveBorrowers(Borrower borrowers[], int count);

int main() {
    const int MAX_BOOKS = 100;
    const int MAX_BORROWERS = 100;
    Book books[MAX_BOOKS];
    Borrower borrowers[MAX_BORROWERS];
    int bookCount = 0;
    int borrowerCount = 0;
    loadBooks(books, bookCount);
    loadBorrowers(borrowers, borrowerCount);
    int choice;
    
    do {
    cout << "\n======= Borrow / Return ========\n" 
    << "1. Borrow Book\n" 
    << "2. Return Book\n" 
    << "3. Quit\n" 
    << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); 
    switch (choice) {
        case 1:
            borrowBook(books, bookCount, borrowers, borrowerCount);
            saveBooks(books, bookCount);
            saveBorrowers(borrowers, borrowerCount);
            break;
        case 2:
            returnBook(books, bookCount, borrowers, borrowerCount);
            saveBooks(books, bookCount);
            saveBorrowers(borrowers, borrowerCount);
            break;
        case 3:
            cout << "Exiting Borrow/Return Module...\n";
            return 0;
        default: cout << "Invalid choice. Try again.\n";
    }
    } while (choice != 3);  
    return 0;
}   




void borrowBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount) {
    string borrowerName, bookTitle;
    cout << "\nEnter borrower name: "; getline(cin, borrowerName);
    cout << "Enter book title to borrow: "; getline(cin, bookTitle);

    int bookIndex = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == bookTitle) { bookIndex = i; break; }
    }
    if (bookIndex == -1) { cout << "Book not found.\n"; return; }
    if (books[bookIndex].availableCopies == 0) { cout << "No copies available.\n"; return; }

    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == borrowerName) { borrowerIndex = i; break; }
    }
    if (borrowerIndex == -1) { cout << "Borrower not found.\n"; return; }

    borrowers[borrowerIndex].borrowedBooks[borrowers[borrowerIndex].borrowedCount] = bookTitle;
    borrowers[borrowerIndex].borrowedCount++;
    books[bookIndex].availableCopies--;
    cout << "Book borrowed successfully.\n";
}

void returnBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount) {
    string borrowerName, bookTitle;
    cout << "\nEnter borrower name: "; getline(cin, borrowerName);
    cout << "Enter book title to return: "; getline(cin, bookTitle);

    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == borrowerName) { borrowerIndex = i; break; }
    }
    if (borrowerIndex == -1) { cout << "Borrower not found.\n"; return; }

    int pos = -1;
    for (int i = 0; i < borrowers[borrowerIndex].borrowedCount; i++) {
        if (borrowers[borrowerIndex].borrowedBooks[i] == bookTitle) { pos = i; break; }
    }
    if (pos == -1) { cout << "This borrower did not borrow that book.\n"; return; }

    for (int i = pos; i < borrowers[borrowerIndex].borrowedCount - 1; i++) {
        borrowers[borrowerIndex].borrowedBooks[i] = borrowers[borrowerIndex].borrowedBooks[i + 1];
    }
    borrowers[borrowerIndex].borrowedCount--;

    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == bookTitle) { books[i].availableCopies++; break; }
    }

    cout << "Book returned successfully.\n";
}



//copied from group mate


void saveBooks(Book books[], int count) {
    ofstream file("books.txt");
    if (!file) {
        cout << "Error. books.txt cannot be open\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        file << books[i].title << ";"
             << books[i].author << ";"
             << books[i].ISBN << ";"
             << books[i].totalCopies << ";"
             << books[i].availableCopies << "\n";
    }

    file.close();
}

void loadBooks(Book books[], int &count) {
    ifstream file("books.txt");
    if (!file) {
        cout << "opening new a book.txt file\n";
        return;
    }

    string line;
    count = 0;

    while (getline(file, line)) {
        stringstream s(line);
        string totalCopiesStr, availableCopiesStr;
        Book b;

        getline(s, b.title, ';');
        getline(s, b.author, ';');
        getline(s, b.ISBN, ';');
        getline(s, totalCopiesStr, ';');
        b.totalCopies = stoi(totalCopiesStr);
        getline(s, availableCopiesStr, ';');
        b.availableCopies = stoi(availableCopiesStr);
        books[count++] = b;
    }

    file.close();
}




void loadBorrowers(Borrower borrowers[], int &count) {
    ifstream file("borrowers.txt");
    if (!file) return;

    string line;
    count = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Borrower b;

        getline(ss, b.name, ' ');
        getline(ss, b.address, ' ');
        getline(ss, b.contact, ' ');

        ss >> b.borrowedCount;
        ss.ignore();

        for (int i = 0; i < b.borrowedCount; i++) {
            getline(file, b.borrowedBooks[i]);
        }

        borrowers[count++] = b;
    }
}

void saveBorrowers(Borrower borrowers[], int count) {
    ofstream file("borrowers.txt");

    for (int i = 0; i < count; i++) {
        file << borrowers[i].name << " "
             << borrowers[i].address << " "
             << borrowers[i].contact << " "
             << borrowers[i].borrowedCount << "\n";

        for (int j = 0; j < borrowers[i].borrowedCount; j++) {
            file << borrowers[i].borrowedBooks[j] << "\n";
        }
    }
}