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


// Function declarations
void createAccount();
void login();
void resetPassword();

void borrowerManagement(const string& adminID);
void addBorrower(Borrower borrowers[], int &count, int maxBorrowers);
void showAllBorrowers(Borrower borrowers[], int count);
void loadBorrowers(Borrower borrowers[], int &count);
void saveBorrowers(Borrower borrowers[], int count);
void bookManagement();
void addBook(Book books[], int &count, int maxBooks);
void showAllBooks(Book books[], int count);



int main() {
    string input;
    int choice;
    do {
        cout << "\n| Books Borrowing System |\n";
        cout << "1. Create Admin Account\n";
        cout << "2. Login\n";
        cout << "3. Reset Password\n";
        cout << "4. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2:
            login();
            break;
        case 3:
            resetPassword();
            break;
        case 4:
            cout << "Closing!...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}

// Create Admin Account
void createAccount() {
    string id, pass;
    ofstream file("admins.txt", ios::app); 

    cout << "\n| Register Admin Account |\n";
    cout << "Enter new Admin ID: ";
    getline(cin, id);
    cout << "Enter new Password: ";
    getline(cin, pass);

    file << id << " " << pass << endl;
    file.close();

    cout << "Admin Account Created!\n";
}

// Admin Login
void login() {
    string id, pass, line, storedId, storedPass;
    bool found = false;

    ifstream file("admins.txt");
    if (!file) {
        cout << "Account Not found!\n";
        return;
    }

    cout << "\n| Admin Login |\n";
    cout << "Enter ID: ";
    getline(cin, id);
    cout << "Enter Password: ";
    getline(cin, pass);

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> storedId >> storedPass;
        if (id == storedId && pass == storedPass) {
            found = true;
            break;
        }
    }
    file.close();

    if (found) { //Login Success
        cout << "\nLogin successful! Welcome, " << id << "!\n";
        borrowerManagement(id);
    } else {
        cout << "Invalid ID or Password.\n";
    }
}

// --- Reset Password ---
void resetPassword() {
    string id, newPass, confirmPass, line, storedId, storedPass;
    bool found = false;
    ifstream file("admins.txt");

    if (!file) {
        cout << "No admin account found. Please create one first.\n";
        return;
    }

    cout << "\n--- Reset Password ---\n";
    cout << "Enter Admin ID: ";
    getline(cin, id);

    string tempFile = "temp.txt";
    ofstream outFile(tempFile);

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> storedId >> storedPass;

        if (id == storedId) {
            found = true;
            cout << "Enter new password: ";
            getline(cin, newPass);
            cout << "Confirm new password: ";
            getline(cin, confirmPass);

            if (newPass == confirmPass) {
                outFile << storedId << " " << newPass << endl;
                cout << "Password reset successfully!\n";
            } else {
                outFile << storedId << " " << storedPass << endl;
                cout << "Passwords do not match. Password not changed.\n";
            }
        } else {
            outFile << storedId << " " << storedPass << endl;
        }
    }

    file.close();
    outFile.close();

    remove("admins.txt");
    rename(tempFile.c_str(), "admins.txt");

    if (!found) {
        cout << "Admin ID not found.\n";
    }
}

// --- Borrower Management ---
void borrowerManagement(const string& adminID) {
    const int MAX_BORROWERS = 100;
    static Borrower borrowers[MAX_BORROWERS];  
    static int borrowerCount = 0;

    loadBorrowers(borrowers, borrowerCount);

    int choice;

    do {
        cout << "\n===== BORROWER MANAGEMENT =====\n";
        cout << "Welcome, " << adminID << "!\n";
        cout << "1. Add New Borrower\n";
        cout << "2. Show All Borrowers\n";
        cout << "3. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            addBorrower(borrowers, borrowerCount, MAX_BORROWERS);
            saveBorrowers(borrowers, borrowerCount);
            break;
        case 2:
            showAllBorrowers(borrowers, borrowerCount);
            break;
        case 3:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
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

        getline(ss, b.name, '|');
        getline(ss, b.address, '|');
        getline(ss, b.contact, '|');

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
        file << borrowers[i].name << "|"
             << borrowers[i].address << "|"
             << borrowers[i].contact << "|"
             << borrowers[i].borrowedCount << "\n";

        for (int j = 0; j < borrowers[i].borrowedCount; j++) {
            file << borrowers[i].borrowedBooks[j] << "\n";
        }
    }
}

void addBorrower(Borrower borrowers[], int &count, int maxBorrowers) {
    if (count >= maxBorrowers) {
        cout << "\nBorrower list is full!\n";
        return;
    }

    Borrower newBorrower;

    cout << "\nEnter Borrower Name: ";
    getline(cin, newBorrower.name);

    cout << "Enter Address: ";
    getline(cin, newBorrower.address);

    cout << "Enter Contact Number: ";
    getline(cin, newBorrower.contact);

    newBorrower.borrowedCount = 0; // no books borrowed yet

    borrowers[count++] = newBorrower;

    cout << "\nBorrower added successfully!\n";
}

void showAllBorrowers(Borrower borrowers[], int count) {
    if (count == 0) {
        cout << "\nNo borrowers found.\n";
        return;
    }

    cout << "\n===== Borrower List =====\n";

    for (int i = 0; i < count; i++) {
        cout << "\nBorrower #" << (i + 1) << ":\n";
        cout << "Name: " << borrowers[i].name << endl;
        cout << "Address: " << borrowers[i].address << endl;
        cout << "Contact: " << borrowers[i].contact << endl;

        cout << "Borrowed Books: ";

        if (borrowers[i].borrowedCount == 0) {
            cout << "None\n";
        } else {
            cout << endl;
            for (int j = 0; j < borrowers[i].borrowedCount; j++) {
                cout << "  - " << borrowers[i].borrowedBooks[j] << endl;
            }
        }
    }
}

// --- Book Management ---
void bookManagement() {
    const int MAX_BOOKS = 100;
    Book books[MAX_BOOKS];
    int bookCount = 0;
    int choice;

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
            break;
        case 2:
            showAllBooks(books, bookCount);
            break;
        case 3:
            cout << "Exiting Book Management Module...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return;
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

// -------- Borrow / Return --------
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
