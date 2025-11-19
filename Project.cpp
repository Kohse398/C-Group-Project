#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

// Function declarations
void createAccount();
void login();
void resetPassword();
void borrowerManagement(const string& adminID); // After Login

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
        getline(cin, input); // read entire line
        cin >> choice;
        cin.ignore(); // Clear newline character from input buffer

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
    cout << "\n===== Borrower Management Section =====\n";
    cout << "Welcome, " << adminID << "!\n";
}

struct Book {
    string title;
    string author;
    string ISBN;
    int totalCopies;
    int availableCopies;
};


void addBook(Book books[], int &count, int maxBooks);
void showAllBooks(Book books[], int count);

int main() {
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
