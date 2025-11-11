//1


//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <windows.h>
//using namespace std;
//
//class FileManager {
//public:
//    void showFiles() { system("dir"); }
//
//    void createFolder(const string& name) {
//        string cmd = "mkdir " + name;
//        system(cmd.c_str());
//    }
//
//    void createFile(const string& name) {
//        string cmd = "echo.> " + name;
//        system(cmd.c_str());
//    }
//
//    void deleteItem(const string& name) {
//        string cmd = "rmdir /s /q " + name + " 2>nul || del /q " + name;
//        system(cmd.c_str());
//    }
//
//    void renameItem(const string& oldName, const string& newName) {
//        string cmd = "rename " + oldName + " " + newName;
//        system(cmd.c_str());
//    }
//
//    void copyItem(const string& src, const string& dest) {
//        string cmd = "xcopy " + src + " " + dest + " /E /I /Y";
//        system(cmd.c_str());
//    }
//
//    void moveItem(const string& src, const string& dest) {
//        string cmd = "move " + src + " " + dest;
//        system(cmd.c_str());
//    }
//
//    void search(const string& mask) {
//        string cmd = "dir /s /b " + mask;
//        system(cmd.c_str());
//    }
//
//    void getSize(const string& name) {
//        string cmd = "dir " + name + " /s";
//        system(cmd.c_str());
//    }
//};
//
//int main() {
//    SetConsoleOutputCP(1251);
//    FileManager fm;
//    int choice;
//    string name, name2;
//
//    while (true) {
//        cout << "\n=== ФАЙЛОВИЙ МЕНЕДЖЕР ===\n";
//        cout << "1. Показати вміст теки\n";
//        cout << "2. Створити папку\n";
//        cout << "3. Створити файл\n";
//        cout << "4. Видалити елемент\n";
//        cout << "5. Перейменувати\n";
//        cout << "6. Копіювати\n";
//        cout << "7. Перемістити\n";
//        cout << "8. Пошук файлів\n";
//        cout << "9. Розмір папки/файлу\n";
//        cout << "0. Вихід\nВаш вибір: ";
//        cin >> choice;
//        cin.ignore();
//
//        switch (choice) {
//        case 1:
//            fm.showFiles();
//            break;
//        case 2:
//            cout << "Назва папки: ";
//            getline(cin, name);
//            fm.createFolder(name);
//            break;
//        case 3:
//            cout << "Назва файлу: ";
//            getline(cin, name);
//            fm.createFile(name);
//            break;
//        case 4:
//            cout << "Ім’я для видалення: ";
//            getline(cin, name);
//            fm.deleteItem(name);
//            break;
//        case 5:
//            cout << "Старе ім’я: ";
//            getline(cin, name);
//            cout << "Нове ім’я: ";
//            getline(cin, name2);
//            fm.renameItem(name, name2);
//            break;
//        case 6:
//            cout << "Джерело: ";
//            getline(cin, name);
//            cout << "Призначення: ";
//            getline(cin, name2);
//            fm.copyItem(name, name2);
//            break;
//        case 7:
//            cout << "Джерело: ";
//            getline(cin, name);
//            cout << "Призначення: ";
//            getline(cin, name2);
//            fm.moveItem(name, name2);
//            break;
//        case 8:
//            cout << "Маска пошуку (*.txt): ";
//            getline(cin, name);
//            fm.search(name);
//            break;
//        case 9:
//            cout << "Назва файлу або папки: ";
//        }
// }


//2

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

struct Expense {
    string category;
    double amount;
    string date;
};

class Wallet {
protected:
    string name;
    double balance;
    vector<Expense> expenses;

public:
    Wallet(const string& n, double b = 0) : name(n), balance(b) {}

    virtual void addMoney(double amount) {
        balance += amount;
    }

    virtual void spendMoney(double amount, const string& category) {
        if (amount > balance) {
            cout << "Недостатньо коштів!\n";
            return;
        }
        balance -= amount;

        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buf[11];
        sprintf(buf, "%02d.%02d.%04d", ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year);

        expenses.push_back({ category, amount, buf });
    }

    double getBalance() const { return balance; }
    string getName() const { return name; }

    const vector<Expense>& getExpenses() const { return expenses; }

    void showExpenses() const {
        if (expenses.empty()) {
            cout << "Немає витрат.\n";
            return;
        }
        for (auto& e : expenses) {
            cout << e.date << " | " << e.category << " | " << e.amount << " грн\n";
        }
    }
};

class DebitCard : public Wallet {
public:
    DebitCard(const string& n, double b = 0) : Wallet(n, b) {}
};

class CreditCard : public Wallet {
    double creditLimit;
public:
    CreditCard(const string& n, double b = 0, double limit = 1000)
        : Wallet(n, b), creditLimit(limit) {
    }

    void spendMoney(double amount, const string& category) override {
        if (amount > balance + creditLimit) {
            cout << "Перевищено кредитний ліміт!\n";
            return;
        }
        balance -= amount;
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buf[11];
        sprintf(buf, "%02d.%02d.%04d", ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year);
        expenses.push_back({ category, amount, buf });
    }
};

class FinanceSystem {
    vector<Wallet*> wallets;

public:
    ~FinanceSystem() {
        for (auto w : wallets) delete w;
    }

    void addWallet(Wallet* w) {
        wallets.push_back(w);
    }

    void listWallets() {
        if (wallets.empty()) {
            cout << "Немає гаманців або карт.\n";
            return;
        }
        for (int i = 0; i < wallets.size(); i++)
            cout << i + 1 << ". " << wallets[i]->getName() << " | Баланс: " << wallets[i]->getBalance() << " грн\n";
    }

    Wallet* chooseWallet() {
        listWallets();
        if (wallets.empty()) return nullptr;
        cout << "Виберіть номер: ";
        int i;
        cin >> i;
        cin.ignore();
        if (i < 1 || i > wallets.size()) return nullptr;
        return wallets[i - 1];
    }

    void saveReport() {
        ofstream file("report.txt");
        for (auto w : wallets) {
            file << "=== " << w->getName() << " ===\n";
            for (auto& e : w->getExpenses()) {
                file << e.date << " | " << e.category << " | " << e.amount << " грн\n";
            }
            file << "Баланс: " << w->getBalance() << " грн\n\n";
        }
        file.close();
        cout << "Звіт збережено у файл report.txt\n";
    }

    void showTop3() {
        vector<Expense> all;
        for (auto w : wallets)
            for (auto& e : w->getExpenses())
                all.push_back(e);

        sort(all.begin(), all.end(), [](auto& a, auto& b) {
            return a.amount > b.amount;
            });

        cout << "\nТОП-3 витрати:\n";
        for (int i = 0; i < 3 && i < all.size(); i++)
            cout << i + 1 << ". " << all[i].category << " - " << all[i].amount << " грн (" << all[i].date << ")\n";
    }
};

int main() {
    FinanceSystem fs;
    int choice;

    while (true) {
        cout << "\n=== СИСТЕМА УПРАВЛІННЯ ФІНАНСАМИ ===\n";
        cout << "1. Додати гаманець\n";
        cout << "2. Додати дебетову карту\n";
        cout << "3. Додати кредитну карту\n";
        cout << "4. Поповнити баланс\n";
        cout << "5. Додати витрату\n";
        cout << "6. Переглянути витрати\n";
        cout << "7. ТОП-3 витрат\n";
        cout << "8. Зберегти звіт у файл\n";
        cout << "0. Вихід\nВаш вибір: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1: {
            string name;
            cout << "Назва гаманця: ";
            getline(cin, name);
            fs.addWallet(new Wallet(name));
            break;
        }
        case 2: {
            string name;
            cout << "Назва дебетової карти: ";
            getline(cin, name);
            fs.addWallet(new DebitCard(name));
            break;
        }
        case 3: {
            string name;
            cout << "Назва кредитної карти: ";
            getline(cin, name);
            fs.addWallet(new CreditCard(name));
            break;
        }
        case 4: {
            Wallet* w = fs.chooseWallet();
            if (!w) break;
            double sum;
            cout << "Сума поповнення: ";
            cin >> sum;
            cin.ignore();
            w->addMoney(sum);
            cout << "Баланс оновлено.\n";
            break;
        }
        case 5: {
            Wallet* w = fs.chooseWallet();
            if (!w) break;
            double sum;
            string cat;
            cout << "Сума витрати: ";
            cin >> sum;
            cin.ignore();
            cout << "Категорія: ";
            getline(cin, cat);
            w->spendMoney(sum, cat);
            break;
        }
        case 6: {
            Wallet* w = fs.chooseWallet();
            if (!w) break;
            w->showExpenses();
            break;
        }
        case 7:
            fs.showTop3();
            break;
        case 8:
            fs.saveReport();
            break;
        default:
            cout << "Невірний вибір.\n";
        }
    }

    cout << "Роботу завершено.\n";
}
