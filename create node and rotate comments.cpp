#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <algorithm>
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

AVLNode *createNode(Book book) //Creates a node holding a Book object
{
    AVLNode *node = new AVLNode;
    node->book = book;
    node->left = node->right = nullptr;
    node->height = 1;
    return node;
}

AVLNode *rightRotate(AVLNode *y) //Right rotation to balance tree, recalculates height
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode *leftRotate(AVLNode *x) //Left rotation to balance tree, heights are updated to reflect new structure
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

// Insert a new book into the AVL tree, maintaining balance
AVLNode *insert(AVLNode *node, Book book)
{
    // Standard BST insert by title (case-insensitive)
    if (!node)
        return createNode(book);
    if (toLower(book.title) < toLower(node->book.title))
        node->left = insert(node->left, book);
    else if (toLower(book.title) > toLower(node->book.title))
        node->right = insert(node->right, book);
    else
        return node; // Duplicate titles not allowed

    // Update height of this ancestor node
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    // Get the balance factor to check if this node became unbalanced
    int balance = getBalance(node);

    // If unbalanced, there are 4 cases

    // Left Left Case
    if (balance > 1 && toLower(book.title) < toLower(node->left->book.title))
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && toLower(book.title) > toLower(node->right->book.title))
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && toLower(book.title) > toLower(node->left->book.title))
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && toLower(book.title) < toLower(node->right->book.title))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

AVLNode *minValueNode(AVLNode *node)
{
    AVLNode *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Delete a book by title from the AVL tree, maintaining balance
AVLNode *deleteNode(AVLNode *root, string title)
{
    if (!root)
        return root;

    // Compare titles (case-insensitive) to find the node to delete
    string target = toLower(title);
    string current = toLower(root->book.title);
    if (target < current)
        root->left = deleteNode(root->left, title);
    else if (target > current)
        root->right = deleteNode(root->right, title);
    else
    {
        // Node with only one child or no child
        if (!root->left || !root->right)
        {
            AVLNode *temp = root->left ? root->left : root->right;
            if (!temp)
            {
                // No child case
                delete root;
                return nullptr;
            }
            else
            {
                // One child case
                *root = *temp;
                delete temp;
            }
        }
        else
        {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            AVLNode *temp = minValueNode(root->right);
            // Copy the inorder successor's data to this node
            root->book = temp->book;
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->book.title);
        }
    }

    // If the tree had only one node then return
    if (!root)
        return root;

    // Update height
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    // Get balance factor
    int balance = getBalance(root);

    // Balance the tree if needed

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


void collectBooks(AVLNode* root, vector<Book>& books) {
    if (!root) return;
    collectBooks(root->left, books);
    books.push_back(root->book);
    collectBooks(root->right, books);
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

// Display all books sorted by author using bubble sort
void displayAllByAuthor(AVLNode* root) {
    vector<Book> books;
    collectBooks(root, books);

    // Bubble sort: repeatedly step through the list, compare adjacent elements and swap if out of order
    for (size_t i = 0; i < books.size(); ++i) {
        for (size_t j = 0; j < books.size() - i - 1; ++j) {
            // Compare authors case-insensitively
            if (toLower(books[j].author) > toLower(books[j + 1].author)) {
                swap(books[j], books[j + 1]);
            }
        }
    }

    // Display sorted books
    for (const Book& b : books) {
        displayBook(b);
    }
}

// Linear search for books by author (case-insensitive)
// Displays author outside the box, then the rest of the info in a box
void searchByAuthor(AVLNode* root, const string& author, bool& found) {
    if (!root) return;
    // Check if the current node's author matches the search term
    if (toLower(root->book.author) == toLower(author)) {
        found = true;
        // Author outside the box
        cout << "\n\t\t\t\t\t\t\tAuthor: " << root->book.author << endl;
        // Boxed book info (excluding author)
        cout << "\t\t\t\t\t\t\t-------------------------------\n";
        cout << "\t\t\t\t\t\t\tTitle: " << root->book.title << "\n";
        cout << "\t\t\t\t\t\t\tPublisher: " << root->book.publisher << "\n";
        cout << "\t\t\t\t\t\t\tDate: " << root->book.month << " " << root->book.day << ", " << root->book.year << "\n";
        cout << "\t\t\t\t\t\t\tISBN: " << root->book.isbn << "\n";
        cout << "\t\t\t\t\t\t\tCategory: " << root->book.category << "\n";
        cout << "\t\t\t\t\t\t\tCall Number: " << root->book.callNumber << "\n";
    }
    // Recursively search left and right subtrees
    searchByAuthor(root->left, author, found);
    searchByAuthor(root->right, author, found);
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

void menu(AVLNode *&root)
{
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
    cout << "\t\t\t\t\t\t\t\t5. View all books sorted by author" << endl;
    cout << "\t\t\t\t\t\t\t\t6. Enter author to search:" << endl;
    cout << "\t\t\t\t\t\t\t\t7. Exit" << endl;
    cout << "\t\t\t\t\t\t\t|-=================================-|" << endl;
    cout << "\t\t\t\t\t\t\t\tChoose an option: ";
    cin >> choice;
    cin.ignore();
    switch (choice)
    {
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
        cout << "\nAll Books Sorted by Author:\n";
        displayAllByAuthor(root);
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;

    case 6:
        cout << "\nEnter author to search: ";
        getline(cin, keyword);
        {
            bool found = false;
            searchByAuthor(root, keyword, found);
            if (!found)
                cout << "No books found for that author.\n";
        }
        cout << "Press Enter to continue.";
        cin.get();
        system("clear");
        break;

    case 7:
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
