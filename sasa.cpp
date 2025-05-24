#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
using namespace std;

// AVL Book
struct Book
{
    int bookNumber;
    string title;
    string author;
    string publisher;
    string date;
    string isbn;
    string category;
};

struct AVLNode
{
    Book book;
    AVLNode *left;
    AVLNode *right;
    int height;
};

int getHeight(AVLNode *node)
{
    return node ? node->height : 0;
}

int getBalance(AVLNode *node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

string toLower(const string &str)
{
    string lower = str;
    for (char &c : lower)
        c = tolower(c);
    return lower;
}

AVLNode *createNode(Book book)
{
    AVLNode *node = new AVLNode;
    node->book = book;
    node->left = node->right = nullptr;
    node->height = 1;
    return node;
}

AVLNode *rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode *leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

AVLNode *insert(AVLNode *node, Book book)
{
    if (!node)
        return createNode(book);
    if (toLower(book.title) < toLower(node->book.title))
        node->left = insert(node->left, book);
    else if (toLower(book.title) > toLower(node->book.title))
        node->right = insert(node->right, book);
    else
        return node;
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int balance = getBalance(node);
    if (balance > 1 && toLower(book.title) < toLower(node->left->book.title))
        return rightRotate(node);
    if (balance < -1 && toLower(book.title) > toLower(node->right->book.title))
        return leftRotate(node);
    if (balance > 1 && toLower(book.title) > toLower(node->left->book.title))
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && toLower(book.title) < toLower(node->right->book.title))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode *minValueNode(AVLNode *node)
{
    AVLNode *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

AVLNode *deleteNode(AVLNode *root, string title)
{
    if (!root)
        return root;
    string target = toLower(title);
    string current = toLower(root->book.title);
    if (target < current)
        root->left = deleteNode(root->left, title);
    else if (target > current)
        root->right = deleteNode(root->right, title);
    else
    {
        if (!root->left || !root->right)
        {
            AVLNode *temp = root->left ? root->left : root->right;
            if (!temp)
            {
                delete root;
                return nullptr;
            }
            else
            {
                *root = *temp;
                delete temp;
            }
        }
        else
        {
            AVLNode *temp = minValueNode(root->right);
            root->book = temp->book;
            root->right = deleteNode(root->right, temp->book.title);
        }
    }
    if (!root)
        return root;
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void displayBook(const Book &book)
{
    cout << "-------------------------------\n";
    cout << "Book Number: " << book.bookNumber << "\n";
    cout << "Title: " << book.title << "\n";
    cout << "Author: " << book.author << "\n";
    cout << "Publisher: " << book.publisher << "\n";
    cout << "Date: " << book.date << "\n";
    cout << "ISBN: " << book.isbn << "\n";
    cout << "Category: " << book.category << "\n";
}

void searchBooks(AVLNode *root, const string &keyword, bool &found)
{
    if (!root)
        return;
    string kw = toLower(keyword);
    Book &bk = root->book;
    bool match =
        toLower(bk.title).find(kw) != string::npos ||
        toLower(bk.author).find(kw) != string::npos ||
        toLower(bk.publisher).find(kw) != string::npos ||
        toLower(bk.date).find(kw) != string::npos ||
        toLower(bk.isbn).find(kw) != string::npos ||
        toLower(bk.category).find(kw) != string::npos;
    if (match)
    {
        displayBook(bk);
        found = true;
    }
    searchBooks(root->left, keyword, found);
    searchBooks(root->right, keyword, found);
}

void displayAll(AVLNode *root)
{
    if (!root)
        return;
    displayAll(root->left);
    displayBook(root->book);
    displayAll(root->right);
}

// Interface Functions
void titleScreen()
{
    while (true)
    {
        cout << R"(
                                              |-======================================================-|
                                              |                                                        |
                                              |  ██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗  |
                                              |  ██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝  |
                                              |  ██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝   |
                                              |  ██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝    |
                                              |  ███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║     |
                                              |  ╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝     |
                                              |-======================================================-|)"
             << endl;
        cout << "\t\t\t\t\t      | Created by Respicio | Bartolome | Bernardo | Natividad |" << endl;
        cout << "\t\t\t\t\t      |-======================================================-|" << endl;
        cout << "\t\t\t\t\t\t\t\tEnter [1] to proceed" << endl;
        cout << "\t\t\t\t\t\t\t\tEnter [2] to exit" << endl;
        cout << "\t\t\t\t\t\t\t-=================================-" << endl;
        cout << "\t\t\t\t\t\t\t\tChoose an option: ";
        int titleChoice;
        cin >> titleChoice;
        if (titleChoice == 1)
        {
            cout << "Proceeding to the main menu..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            system("clear");
            return;
        }
        if (titleChoice == 2)
        {
            cout << "Exiting..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            exit(0);
        }
        cout << "Error: Invalid option." << endl;
        cin.clear();
        this_thread::sleep_for(chrono::seconds(2));
        system("clear");
    }
}

void menu(AVLNode *&root)
{
    int choice;
    Book b;
    string keyword;
    cout << "|-======================-|" << endl;
    cout << "        Main Menu" << endl;
    cout << "|-======================-|" << endl;
    cout << "   1. Search a book" << endl;
    cout << "   2. View all books" << endl;
    cout << "   3. Add a book" << endl;
    cout << "   4. Remove a book" << endl;
    cout << "   5. Exit" << endl;
    cout << "|-======================-|" << endl;
    cout << "Choose an option: ";
    cin >> choice;
    cin.ignore();
    switch (choice)
    {
    case 1:
        cout << "Enter any keyword (title, author, publisher, date, ISBN, or category): ";
        getline(cin, keyword);
        {
            bool found = false;
            searchBooks(root, keyword, found);
            if (!found)
                cout << "No matching book found.\n";
        }
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;
    case 2:
        cout << "\nAll Books in Catalog:\n";
        displayAll(root);
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;
    case 3:
        cout << "Enter Book Number: ";
        cin >> b.bookNumber;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, b.title);
        cout << "Enter Author: ";
        getline(cin, b.author);
        cout << "Enter Publisher: ";
        getline(cin, b.publisher);
        cout << "Enter Date: ";
        getline(cin, b.date);
        cout << "Enter ISBN: ";
        getline(cin, b.isbn);
        cout << "Enter Category: ";
        getline(cin, b.category);
        root = insert(root, b);
        cout << "Book added!\n";
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;
    case 4:
        cout << "Enter Title to delete: ";
        getline(cin, keyword);
        root = deleteNode(root, keyword);
        cout << "Book deleted (if it existed).\n";
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;
    case 5:
        cout << "Exiting..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        exit(0);
    default:
        cout << "Invalid option. Try again." << endl;
        cin.clear();
        this_thread::sleep_for(chrono::seconds(2));
        system("clear");
    }
}

int main()
{
    AVLNode *root = nullptr;
    // Initial books
    vector<Book> books = {
        {1, "One Piece", "Eiichiro Oda", "Shueisha", "1997", "9780000001", "Manga"},
        {2, "Naruto", "Masashi Kishimoto", "Shueisha", "1999", "9780000002", "Manga"},
        {3, "Dragon Ball", "Akira Toriyama", "Shueisha", "1984", "9780000003", "Manga"},
        {4, "Fire Force", "Atsushi Ohkubo", "Kodansha", "2015", "9780000004", "Manga"},
        {5, "Frieren", "Kanehito Yamada", "Shogakukan", "2020", "9780000005", "Manga"},
        {6, "Slime", "Fuse", "Kodansha", "2014", "9780000006", "Manga"},
        {7, "Bleach", "Tite Kubo", "Shueisha", "2001", "9780000007", "Manga"}};
    for (Book b : books)
        root = insert(root, b);

    titleScreen();
    while (true)
    {
        menu(root);
        cout << endl;
    }
    return 0;
}
