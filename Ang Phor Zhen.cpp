#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

// Book Structure
struct Book {
    string title;
    string author;
    string ISBN;
    int totalCopies;
    int availableCopies;
};

// Function declarations
void addBook(Book books[], int &count, int maxBooks);
void showAllBooks(Book books[], int count);
void saveBooks(Book books[], int count);
void loadBooks(Book books[], int &count);

int main() {
    const int MAX_BOOKS = 100;
    Book books[MAX_BOOKS];
    int bookCount = 0;
    int choice;
    loadBooks(books, bookCount);

    do {
        cout << "\n===== BOOK MANAGEMENT MENU =====\n";
        cout << "1. Add New Book\n";
        cout << "2. Show All Books\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
        case 1:
            addBook(books, bookCount, MAX_BOOKS);
            saveBooks(books, bookCount);
            break;
        case 2:
            showAllBooks(books, bookCount);
            break;
        case 3:
            cout << "Exiting Book Management Module...\n";
            saveBooks(books, bookCount);
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}


void addBook(Book books[], int &count, int maxBooks) {
    if (count >= maxBooks) {
        cout << "Book list is full!\n";
        return;
    }

    Book newBook;
    cout << "\nEnter Book Title: ";
    getline(cin, newBook.title);
    cout << "Enter Author Name: ";
    getline(cin, newBook.author);
    cout << "Enter ISBN: ";
    getline(cin, newBook.ISBN);
    cout << "Enter Number of Copies: ";
    cin >> newBook.totalCopies;
    cin.ignore();

    newBook.availableCopies = newBook.totalCopies;
    books[count++] = newBook;

    cout << "\nBook added successfully!\n";
}


void showAllBooks(Book books[], int count) {
    if (count == 0) {
        cout << "\nNo books in the system yet.\n";
        return;
    }

    cout << "\nCurrent Book Inventory:\n";
    cout << left << setw(30) << "Title" 
         << setw(20) << "Author" 
         << setw(15) << "ISBN" 
         << setw(10) << "Available" 
         << setw(10) << "Total" << endl;

    cout << string(85, '-') << endl;

    for (int i = 0; i < count; ++i) {
        cout << left << setw(30) << books[i].title
             << setw(20) << books[i].author
             << setw(15) << books[i].ISBN
             << setw(10) << books[i].availableCopies
             << setw(10) << books[i].totalCopies
             << endl;
    }
}

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