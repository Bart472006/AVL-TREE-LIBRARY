#include <iostream>
#include <string>
#include <vector>
using namespace std;
//what s
struct Book {
    string title;
    string author;
    string publisher;
    string date;
    string isbn;
};

struct AVLNode {
    Book book;
    AVLNode* left;
    AVLNode* right;
    int height;
};

int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

string toLower(const string& str) {
    string lower = str;
    for (char& c : lower)
        c = tolower(c);
    return lower;
}

AVLNode* createNode(Book book) {
    AVLNode* node = new AVLNode;
    node->book = book;
    node->left = node->right = nullptr;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* insert(AVLNode* node, Book book) {
    if (!node) return createNode(book);

    if (book.title < node->book.title)
        node->left = insert(node->left, book);
    else if (book.title > node->book.title)
        node->right = insert(node->right, book);
    else
        return node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int balance = getBalance(node);

    if (balance > 1 && book.title < node->left->book.title)
        return rightRotate(node);
    if (balance < -1 && book.title > node->right->book.title)
        return leftRotate(node);
    if (balance > 1 && book.title > node->left->book.title) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && book.title < node->right->book.title) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

AVLNode* deleteNode(AVLNode* root, string title) {
    if (!root) return root;

    string target = toLower(title);
    string current = toLower(root->book.title);

    if (target < current)
        root->left = deleteNode(root->left, title);
    else if (target > current)
        root->right = deleteNode(root->right, title);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                delete root;
                return nullptr;
            } else {
                *root = *temp;
                delete temp;
            }
        } else {
            AVLNode* temp = minValueNode(root->right);
            root->book = temp->book;
            root->right = deleteNode(root->right, temp->book.title);
        }
    }

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void displayBook(const Book& book) {
    cout << "\U0001F4D8 Title: " << book.title << "\n";
    cout << "\U0001F464 Author: " << book.author << "\n";
    cout << "\U0001F3E2 Publisher: " << book.publisher << "\n";
    cout << "\U0001F4C5 Date: " << book.date << "\n";
    cout << "\U0001F522 ISBN: " << book.isbn << "\n";
    cout << "-------------------------------\n";
}

void searchBooks(AVLNode* root, const string& keyword) {
    if (!root) return;

    string kw = toLower(keyword);
    Book& bk = root->book;

    bool match =
        toLower(bk.title).find(kw) != string::npos ||
        toLower(bk.author).find(kw) != string::npos ||
        toLower(bk.publisher).find(kw) != string::npos ||
        toLower(bk.date).find(kw) != string::npos ||
        toLower(bk.isbn).find(kw) != string::npos;

    if (match)
        displayBook(bk);

    searchBooks(root->left, keyword);
    searchBooks(root->right, keyword);
}

void displayAll(AVLNode* root) {
    if (!root) return;
    displayAll(root->left);
    displayBook(root->book);
    displayAll(root->right);
}

void displayGroupedByField(AVLNode* root, const string& field, const string& value) {
    if (!root) return;

    string v = toLower(value);
    Book& b = root->book;

    if ((field == "author" && toLower(b.author) == v) ||
        (field == "publisher" && toLower(b.publisher) == v) ||
        (field == "date" && toLower(b.date) == v)) {
        displayBook(b);
    }

    displayGroupedByField(root->left, field, value);
    displayGroupedByField(root->right, field, value);
}

int main() {
    AVLNode* root = nullptr;
    int choice;
    Book b;
    string keyword;

    vector<Book> books = {
        {"One Piece", "Eiichiro Oda", "Shueisha", "1997", "9780000001"},
        {"Naruto", "Masashi Kishimoto", "Shueisha", "1999", "9780000002"},
        {"Dragon Ball", "Akira Toriyama", "Shueisha", "1984", "9780000003"},
        {"Fire Force", "Atsushi Ohkubo", "Kodansha", "2015", "9780000004"},
        {"Frieren", "Kanehito Yamada", "Shogakukan", "2020", "9780000005"},
        {"Slime", "Fuse", "Kodansha", "2014", "9780000006"},
        {"Bleach", "Tite Kubo", "Shueisha", "2001", "9780000007"}
    };

    for (Book b : books)
        root = insert(root, b);

    while (true) {
        cout << "\n\U0001F4DA LIBRARY CATALOG MENU \U0001F4DA\n";
        cout << "1. Add Book\n";
        cout << "2. Delete Book\n";
        cout << "3. Search Book\n";
        cout << "4. Display All Books\n";
        cout << "5. Display Grouped Results\n";
        cout << "6. Exit\n";
        cout << "Select option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter Title: "; getline(cin, b.title);
                cout << "Enter Author: "; getline(cin, b.author);
                cout << "Enter Publisher: "; getline(cin, b.publisher);
                cout << "Enter Date: "; getline(cin, b.date);
                cout << "Enter ISBN: "; getline(cin, b.isbn);
                root = insert(root, b);
                break;
            case 2:
                cout << "Enter Title to delete: ";
                getline(cin, keyword);
                root = deleteNode(root, keyword);
                break;
            case 3:
                cout << "Enter any keyword (title, author, publisher, date, or ISBN): ";
                getline(cin, keyword);
                searchBooks(root, keyword);
                break;
            case 4:
                cout << "\n\U0001F4D6 All Books in Catalog:\n";
                displayAll(root);
                break;
            case 5: {
                string val;
                cout << "Enter field to group by (author/publisher/date): ";
                getline(cin, keyword);
                cout << "Enter value to search: ";
                getline(cin, val);
                displayGroupedByField(root, keyword, val);
                break;
            }
            case 6:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}