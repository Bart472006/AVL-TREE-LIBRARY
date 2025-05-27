#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
using namespace std;


// AVL Book
struct Book
{
    string title;
    string author;
    string publisher;
    string month;      // e.g. "January"
    string day;        // e.g. "01"
    string year;       // e.g. "2025"
    string isbn;
    string category;
    string callNumber; // call number is now last
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
    cout << "\t\t\t\t\t\t\t-------------------------------\n";
    cout << "\t\t\t\t\t\t\tTitle: " << book.title << "\n";
    cout << "\t\t\t\t\t\t\tAuthor: " << book.author << "\n";
    cout << "\t\t\t\t\t\t\tPublisher: " << book.publisher << "\n";
    cout << "\t\t\t\t\t\t\tDate: " << book.month << " " << book.day << ", " << book.year << "\n";
    cout << "\t\t\t\t\t\t\tISBN: " << book.isbn << "\n";
    cout << "\t\t\t\t\t\t\tCategory: " << book.category << "\n";
    cout << "\t\t\t\t\t\t\tCall Number: " << book.callNumber << "\n";  // last displayed
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
        toLower(bk.month).find(kw) != string::npos ||
        toLower(bk.day).find(kw) != string::npos ||
        toLower(bk.year).find(kw) != string::npos ||
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
        cout << "\t\t\t\t\t\t\t|-=================================-|" << endl;
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


void menu(AVLNode*& root) {
    const int BOX_WIDTH = 60;
    int choice;
    Book b;
    string keyword;

    cout << R"(
                                              |-======================================================-|
                                              |                                                        |
                                              |         ███╗   ███╗███████╗███╗   ██╗██╗   ██╗         |
                                              |         ████╗ ████║██╔════╝████╗  ██║██║   ██║         |
                                              |         ██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║         |
                                              |         ██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║         |
                                              |         ██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝         |
                                              |         ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝          |
                                              |                                                        |
                                              |-======================================================-|
    )" << endl;

    cout << "\t\t\t\t\t\t\t\t1. Search a book" << endl;
    cout << "\t\t\t\t\t\t\t\t2. View all books" << endl;
    cout << "\t\t\t\t\t\t\t\t3. Add a book" << endl;
    cout << "\t\t\t\t\t\t\t\t4. Remove a book" << endl;
    cout << "\t\t\t\t\t\t\t\t5. Exit" << endl;
    cout << "\t\t\t\t\t\t\t|-=================================-|" << endl;
    cout << "\t\t\t\t\t\t\t\tChoose an option: ";
    cin >> choice;
    cin.ignore();
    switch (choice) {
         case 1:

cout << endl;
cout << "\t\t\t\t\t╔══════════════════════════════════════════════════════════════════════╗" << endl;
cout << "\t\t\t\t\t║ 🔍 Enter any keyword (title, author, publisher, date, ISBN, etc.):   ║" << endl;
cout << "\t\t\t\t\t╟──────────────────────────────────────────────────────────────────────╢" << endl;
cout << "\t\t\t\t\t ";
getline(cin, keyword);

// Trim or pad input for a clean box
if (keyword.length() > BOX_WIDTH)
    keyword = keyword.substr(0, BOX_WIDTH);


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
cout << "Enter Title: ";
getline(cin, b.title);

cout << "Enter Author: ";
getline(cin, b.author);

cout << "Enter Publisher: ";
getline(cin, b.publisher);

cout << "Enter Month (January - December): ";
getline(cin, b.month);

cout << "Enter Day (01 - 31): ";
getline(cin, b.day);

cout << "Enter Year: ";
getline(cin, b.year);

cout << "Enter ISBN: ";
getline(cin, b.isbn);

cout << "Enter Category: ";
getline(cin, b.category);

cout << "Enter Call Number: ";
getline(cin, b.callNumber);  // last input


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
            break;
    }
}

int main()
{
    AVLNode *root = nullptr;
    // Initial books
   vector<Book> books = {
    {"One Piece", "Eiichiro Oda", "Shueisha", "July", "22", "1997", "9780000001", "Manga", "QA76.73.C15"},
    {"Naruto", "Masashi Kishimoto", "Shueisha", "September", "21", "1999", "9780000002", "Manga", "QA76.73.C16"}
};
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
