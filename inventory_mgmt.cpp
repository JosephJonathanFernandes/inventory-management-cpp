#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <iomanip>
#include <random>

using namespace std;

class Product {
private:
    string name;
    int number;
    double price;
    string company;
    string newCompany;

public:
    Product(const string &n, int num, double p, const string &comp)
        : name(n), number(num), price(p), company(comp) {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    string getCompany() const { return company; }

    string getFullCompanyString() const {
        return (newCompany.empty() || newCompany == company) ? company : company + " (NewCompany: " + newCompany + ")";
    }

    void display() const {
        cout << "-------------------------------------------" << endl;
        cout << left << setw(15) << "Name:" << name << endl;
        cout << left << setw(15) << "Number:" << number << endl;
        cout << left << setw(15) << "Price:" << "$" << price << endl;
        cout << left << setw(15) << "Company:" << getFullCompanyString() << endl;
        cout << "-------------------------------------------" << endl;
    }

    void setNewCompany(const string &newComp) {
        newCompany = newComp;
    }

    int getNumber() const { return number; }
    void setNumber(int newNumber) { number = newNumber; }
};

constexpr int MAX_PRODUCTS = 100;

ostream &menuFormat(ostream &os) {
    os << "==========================================\n";
    os << "|        Inventory Management System       |\n";
    os << "==========================================\n";
    os << "| Developed by: Krutika, Jonus, Krish, Manthan, Hrishikesh |\n";
    os << "==========================================\n";
    return os;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayMainMenu() {
    clearScreen();
    cout << menuFormat;
    cout << "| 1. Enter item details and create a file  |\n";
    cout << "| 2. Edit item number and display updated content |\n";
    cout << "| 3. Append company name to an item file and display appended content |\n";
    cout << "| 4. Display all items                    |\n";
    cout << "| 5. Search for a product by number       |\n";
    cout << "| 6. Exit                                 |\n";
    cout << menuFormat;
    cout << "Enter your choice: ";
}

list<Product> products;

int generateRandomProductNumber() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(1000, 9999);
    return dis(gen);
}

auto findProductByNumber(int number) {
    return find_if(products.begin(), products.end(), [number](const Product &product) {
        return product.getNumber() == number;
    });
}

bool isProductNumberUnique(int number) {
    return findProductByNumber(number) == products.end();
}

void createItemFile(const Product &product) {
    ofstream file(to_string(product.getNumber()) + ".txt");
    if (file.is_open()) {
        file << "Name: " << product.getName() << endl;
        file << "Number: " << product.getNumber() << endl;
        file << "Price: " << product.getPrice() << endl;
        file << "Company: " << product.getCompany() << endl;
        file.close();
        cout << "Item details saved to file." << endl;

        ifstream readFile(to_string(product.getNumber()) + ".txt");
        string line;
        while (getline(readFile, line)) {
            cout << line << endl;
        }
        readFile.close();
    } else {
        cout << "Failed to create the item file." << endl;
    }
}

void editItemNumber(int oldNumber, int newNumber) {
    string oldFileName = to_string(oldNumber) + ".txt";
    ifstream oldFile(oldFileName);

    if (oldFile.is_open()) {
        string newName = to_string(newNumber) + ".txt";
        ofstream newFile(newName);

        string line;
        while (getline(oldFile, line)) {
            if (line.substr(0, 7) == "Number:") {
                newFile << "Number: " << newNumber << endl;
            } else {
                newFile << line << endl;
            }
        }

        oldFile.close();
        newFile.close();
        remove(oldFileName.c_str());
        cout << "Item number updated." << endl;

        auto it = find_if(products.begin(), products.end(), [oldNumber](const Product &product) {
            return product.getNumber() == oldNumber;
        });

        if (it != products.end()) {
            it->setNumber(newNumber);
        }

        ifstream readFile(newName);
        while (getline(readFile, line)) {
            cout << line << endl;
        }
        readFile.close();
    } else {
        cout << "Item not found." << endl;
    }
}

void appendCompanyName(int number, const string &company) {
    string fileName = to_string(number) + ".txt";
    ifstream fileCheck(fileName);

    if (fileCheck.good()) {
        fileCheck.close();

        auto it = findProductByNumber(number);
        if (it != products.end()) {
            it->setNewCompany(company);
        }

        ofstream file(fileName, ios::app);
        if (file.is_open()) {
            file << "Company: " << company << endl;
            file.close();
            cout << "Company name appended to the item file." << endl;

            ifstream readFile(fileName);
            string line;
            while (getline(readFile, line)) {
                cout << line << endl;
            }
            readFile.close();
        } else {
            cout << "Failed to open the item file for appending." << endl;
        }
    } else {
        cout << "Item file not found." << endl;
    }
}

void displayProduct(const Product &product) {
    cout << "-------------------------------------------" << endl;
    cout << left << setw(15) << "Name:" << product.getName() << endl;
    cout << left << setw(15) << "Number:" << product.getNumber() << endl;
    cout << left << setw(15) << "Price:" << "$" << product.getPrice() << endl;
    cout << left << setw(15) << "Company:" << product.getFullCompanyString() << endl;
    cout << "-------------------------------------------" << endl;
}

void searchByNumber(int numberToFind) {
    auto it = findProductByNumber(numberToFind);
    if (it != products.end()) {
        cout << "Product found:" << endl;
        displayProduct(*it);
    } else {
        cout << "Product not found." << endl;
    }
}

int main() {
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            if (products.size() < MAX_PRODUCTS) {
                string name, company;
                double price;

                clearScreen();

                cout << "Enter item details:" << endl;
                cout << "Name: ";
                cin.ignore();
                getline(cin, name);

                int number;
                do {
                    number = generateRandomProductNumber();
                } while (!isProductNumberUnique(number));

                cout << "Generated Product Number: " << number << endl;

                cout << "Price: $";
                cin >> price;
                cout << "Company: ";
                cin.ignore();
                getline(cin, company);

                products.push_back(Product(name, number, price, company));
                createItemFile(products.back());
            } else {
                clearScreen();
                cout << "Maximum product limit reached (" << MAX_PRODUCTS << " products)." << endl;
            }
            break;
        }
        case 2: {
            int oldNumber, newNumber;
            clearScreen();
            cout << "Edit item number:" << endl;
            cout << "Enter the old item number: ";
            cin >> oldNumber;
            cout << "Enter the new item number: ";
            cin >> newNumber;
            if (isProductNumberUnique(newNumber)) {
                editItemNumber(oldNumber, newNumber);
            } else {
                clearScreen();
                cout << "New item number already in use. Item number not updated." << endl;
            }
            break;
        }
        case 3: {
            int number;
            string company;
            clearScreen();
            cout << "Append company name to an item file:" << endl;
            cout << "Enter the item number: ";
            cin >> number;
            cout << "Enter the company name to append: ";
            cin.ignore();
            getline(cin, company);
            appendCompanyName(number, company);
            break;
        }
        case 4: {
            clearScreen();
            cout << "==========================================\n";
            cout << "|          Displaying all items           |\n";
            cout << "==========================================\n";
            for (const Product &product : products) {
                displayProduct(product);
            }
            break;
        }
        case 5: {
            int numberToFind;
            clearScreen();
            cout << "Search for a product by number:" << endl;
            cout << "Enter the item number to search for: ";
            cin >> numberToFind;
            searchByNumber(numberToFind);
            break;
        }
        case 6: {
            clearScreen();
            cout << "Exiting the program." << endl;
            return 0;
        }
        default: {
            clearScreen();
            cout << "Invalid choice. Please select a valid option." << endl;
        }
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
