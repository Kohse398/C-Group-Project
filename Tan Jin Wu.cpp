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

void loadBorrowers(Borrower borrowers[], int &count);
void saveBorrowers(Borrower borrowers[], int count);
void addBorrower(Borrower borrowers[], int &count, int maxBorrowers);
void showAllBorrowers(Borrower borrowers[], int count);

int main() {
    const int MAX_BORROWERS = 100;
    static Borrower borrowers[MAX_BORROWERS];  
    static int borrowerCount = 0;

    loadBorrowers(borrowers, borrowerCount);

    int choice;

    do {
        cout << "\n===== BORROWER MANAGEMENT =====\n";
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
