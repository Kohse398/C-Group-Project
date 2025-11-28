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
    int borrowedListCount = 0;
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

void mainMenu(const string& adminID);

void borrowerManagement(const string& adminID);
void addBorrower(Borrower borrowers[], int &count, int maxBorrowers);
void showAllBorrowers(Borrower borrowers[], int count);
void loadBorrowers(Borrower borrowers[], int &count);
void saveBorrowers(Borrower borrowers[], int count);

void bookManagement();
void addBook(Book books[], int &count, int maxBooks);
void showAllBooks(Book books[], int count);
void saveBooks(Book books[], int count);
void loadBooks(Book books[], int &count);

void borrowBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount);
void returnBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount);



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
        mainMenu(id);
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
// --- Main Menu Functions ---
void mainMenu(const string& adminID) {
    static const int MAX_BOOKS = 100;
    static const int MAX_BORROWERS = 100;

    static Book books[MAX_BOOKS];
    static int bookCount = 0;

    static Borrower borrowers[MAX_BORROWERS];
    static int borrowerCount = 0;

    loadBorrowers(borrowers, borrowerCount);

    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n" << "1. Borrowers Management\n" << "2. Books Management\n" << "3. Borrow Books\n" << "4. Return Books\n" << "5. Logouts\n" << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 
        switch (choice) {
        case 1:
            borrowerManagement(adminID);
            break;
        case 2:
            bookManagement();
            break;
        case 3:
            borrowBook(books, bookCount, borrowers, borrowerCount);
            saveBorrowers(borrowers, borrowerCount);
            break;
        case 4:
            returnBook(books, bookCount, borrowers, borrowerCount);
            saveBorrowers(borrowers, borrowerCount);
            break;
        case 5:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice. Please only use 1-5.\n";
        }
    } while (choice != 5);
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
            cout << "borrowers.txt is saved\n";
            cout << "Returning to the Main Menu...\n";
            break;
        default:
            cout << "Invalid choice. Please only use 1-3.\n";
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

        getline(ss, b.name, ' ');
        getline(ss, b.address, ' ');
        getline(ss, b.contact, ' ');

        ss >> b.borrowedListCount;
        ss.ignore();

        for (int i = 0; i < b.borrowedListCount; i++) {
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
             << borrowers[i].borrowedListCount << "\n";

        for (int j = 0; j < borrowers[i].borrowedListCount; j++) {
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

    newBorrower.borrowedListCount = 0;

    borrowers[count++] = newBorrower;

    cout << "\nBorrower " << newBorrower.name << " added successfully!\n";
}

void showAllBorrowers(Borrower borrowers[], int count) {
    if (count == 0) {
        cout << "\nNo borrowers found in the system.\n";
        return;
    }

    cout << "\n===== Borrower List =====\n";


    for (int i = 0; i < count; i++) {
        cout << left << setw(20) << "Name" << setw(35) << "Address" << setw(20) << "Contact" << setw(20) << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(20) << borrowers[i].name << setw(35) << borrowers[i].address << setw(20) << borrowers[i].contact << endl;

        cout << "Borrowed Books List: ";

        if (borrowers[i].borrowedListCount == 0) {
            cout << "None\n";
        } else {
            cout << endl;
            for (int j = 0; j < borrowers[i].borrowedListCount; j++) {
                cout << "  - " << borrowers[i].borrowedBooks[j] << endl;
            }
        }
        
        cout << "\n";
    }
}

// --- Book Management ---
void bookManagement() {
    const int MAX_BOOKS = 100;
    Book books[MAX_BOOKS];
    int bookCount = 0;
    int choice;
    loadBooks(books, bookCount);

    do {
        cout << "\n===== BOOK MANAGEMENT MENU =====\n";
        cout << "1. Add New Book\n";
        cout << "2. Show All Books\n";
        cout << "3. Main Menu\n";
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
            cout << "books.txt is saved\n";
            cout << "Returning to Main Menu...\n";
            saveBooks(books, bookCount);
            break;
        default:
            cout << "Invalid choice. Please only use 1-3.\n";
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

    cout << "\nBook " << newBook.title << " successfully added to the system!\n";
}


void showAllBooks(Book books[], int count) {
    if (count == 0) {
        cout << "\nNo books found in the system yet.\n";
        return;
    }

    cout << "\nCurrent Book Inventory:\n";
    cout << left << setw(30) << "Title" 
         << setw(24) << "Author" 
         << setw(24) << "ISBN" 
         << setw(12) << "Available" 
         << setw(8) << "Total" << endl;

    cout << string(98, '-') << endl;

    for (int i = 0; i < count; ++i) {
        cout << left << setw(30) << books[i].title
             << setw(24) << books[i].author
             << setw(24) << books[i].ISBN
             << setw(12) << books[i].availableCopies
             << setw(8) << books[i].totalCopies
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
        stringstream sss(line);
        string totalCopiesStr, availableCopiesStr;
        Book btxt;

        getline(sss, btxt.title, ';');
        getline(sss, btxt.author, ';');
        getline(sss, btxt.ISBN, ';');
        getline(sss, totalCopiesStr, ';');
        btxt.totalCopies = stoi(totalCopiesStr);
        getline(sss, availableCopiesStr, ';');
        btxt.availableCopies = stoi(availableCopiesStr);
        books[count++] = btxt;
    }

    file.close();
}


// -------- Borrow / Return --------
void borrowBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount) {
    string borrowerName, bookTitle;
    cout << "\nEnter the borrower name: "; 
    getline(cin, borrowerName);
    cout << "Enter book title " << borrowerName << " want to borrow: "; 
    getline(cin, bookTitle);
    loadBooks(books, bookCount);

    int bookIndex = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == bookTitle) { bookIndex = i; break; }
    }
    if (bookIndex == -1) { cout << "Book " << bookTitle << " not found.\n"; return; }
    if (books[bookIndex].availableCopies == 0) { cout << "No copies of " << bookTitle << " available.\n"; return; }

    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == borrowerName) { borrowerIndex = i; break; }
    }
    if (borrowerIndex == -1) { cout << "Borrower " << borrowerName << " not found.\n"; return; }

    borrowers[borrowerIndex].borrowedBooks[borrowers[borrowerIndex].borrowedListCount] = bookTitle;
    borrowers[borrowerIndex].borrowedListCount++;
    books[bookIndex].availableCopies--;
    cout << "Book borrowed by " << borrowerName << " successfully.\n";
    
    saveBooks(books, bookCount);
}

void returnBook(Book books[], int bookCount, Borrower borrowers[], int borrowerCount) {
    string borrowerName, bookTitle;
    loadBooks(books, bookCount);
    cout << "\nEnter borrower name: "; getline(cin, borrowerName);
    cout << "Enter book title " << borrowerName << " wants to return: "; getline(cin, bookTitle);
    

    int borrowerIndex = -1;
    for (int i = 0; i < borrowerCount; i++) {
        if (borrowers[i].name == borrowerName) { borrowerIndex = i; break; }
    }
    if (borrowerIndex == -1) { cout << "Borrower " << borrowerName << " not found.\n"; return; }

    int pos = -1;
    for (int i = 0; i < borrowers[borrowerIndex].borrowedListCount; i++) {
        if (borrowers[borrowerIndex].borrowedBooks[i] == bookTitle) { pos = i; break; }
    }
    if (pos == -1) { cout << "Borrower " << borrowerName << " did not borrow the book " << bookTitle << ".\n"; return; }

    for (int i = pos; i < borrowers[borrowerIndex].borrowedListCount - 1; i++) {
        borrowers[borrowerIndex].borrowedBooks[i] = borrowers[borrowerIndex].borrowedBooks[i + 1];
    }
    borrowers[borrowerIndex].borrowedListCount--;

    for (int i = 0; i < bookCount; i++) {
        if (books[i].title == bookTitle) { books[i].availableCopies++; break; }
    }

    cout << "Book returned by " << borrowerName << " successfully.\n";
    saveBooks(books, bookCount);
}
