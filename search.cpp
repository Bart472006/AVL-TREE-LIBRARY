#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// what the hik
struct Book
{
    string title, author;
    int year;
    string isbn;
    bool available;

    Book(string t, string a, int y, string i = "", bool av = true)
        : title(t), author(a), year(y), isbn(i), available(av) {}
};

// AVL Tree Node
struct Node
{
    Book book;
    int height;
    Node *left, *right;

    Node(Book b) : book(b), height(1), left(nullptr), right(nullptr) {}
};

class LibrarySystem
{
private:
    Node *root;
    vector<Book *> searchResults;

    // Helper functions for AVL tree
    int height(Node *n)
    {
        return n ? n->height : 0;
    }

    int getBalance(Node *n)
    {
        return n ? height(n->left) - height(n->right) : 0;
    }

    Node *rightRotate(Node *y)
    {
        Node *x = y->left;
        Node *T = x->right;

        x->right = y;
        y->left = T;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node *leftRotate(Node *x)
    {
        Node *y = x->right;
        Node *T = y->left;

        y->left = x;
        x->right = T;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node *insertNode(Node *node, Book book)
    {
        if (!node)
            return new Node(book);

        if (book.title < node->book.title)
            node->left = insertNode(node->left, book);
        else if (book.title > node->book.title)
            node->right = insertNode(node->right, book);
        else
            return node; // Duplicate titles not allowed

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && book.title < node->left->book.title)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && book.title > node->right->book.title)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && book.title > node->left->book.title)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && book.title < node->right->book.title)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Book *searchNode(Node *node, string title)
    {
        if (!node)
            return nullptr;

        if (node->book.title == title)
            return &node->book;

        if (title < node->book.title)
            return searchNode(node->left, title);
        return searchNode(node->right, title);
    }

    Node *minValueNode(Node *node)
    {
        Node *current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    Node *deleteNode(Node *root, string title)
    {
        if (!root)
            return root;

        if (title < root->book.title)
            root->left = deleteNode(root->left, title);
        else if (title > root->book.title)
            root->right = deleteNode(root->right, title);
        else
        {
            // Node with only one child or no child
            if (!root->left || !root->right)
            {
                Node *temp = root->left ? root->left : root->right;

                // No child case
                if (!temp)
                {
                    temp = root;
                    root = nullptr;
                }
                else               // One child case
                    *root = *temp; // Copy the contents of the non-empty child

                delete temp;
            }
            else
            {
                // Node with two children
                Node *temp = minValueNode(root->right);

                // Copy the data
                root->book = temp->book;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->book.title);
            }
        }

        // If the tree had only one node
        if (!root)
            return root;

        // Update height
        root->height = 1 + max(height(root->left), height(root->right));

        // Check balance
        int balance = getBalance(root);

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

    void inOrder(Node *root, vector<Book *> &books)
    {
        if (!root)
            return;

        inOrder(root->left, books);
        books.push_back(&root->book);
        inOrder(root->right, books);
    }

    void partialSearch(Node *root, string partialTitle, vector<Book *> &results)
    {
        if (!root)
            return;

        partialSearch(root->left, partialTitle, results);

        string lowerTitle = root->book.title;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

        string lowerSearch = partialTitle;
        transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

        if (lowerTitle.find(lowerSearch) != string::npos)
        {
            results.push_back(&root->book);
        }

        partialSearch(root->right, partialTitle, results);
    }

    void clearTree(Node *node)
    {
        if (!node)
            return;

        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:
    LibrarySystem() : root(nullptr) {}

    ~LibrarySystem()
    {
        clearTree(root);
    }

    void addBook(string title, string author, int year, string isbn = "", bool available = true)
    {
        Book newBook(title, author, year, isbn, available);
        root = insertNode(root, newBook);
        cout << "Book added successfully!" << endl;
    }

    bool removeBook(string title)
    {
        Book *book = searchNode(root, title);
        if (!book)
            return false;

        root = deleteNode(root, title);
        return true;
    }

    Book *findBook(string title)
    {
        return searchNode(root, title);
    }

    vector<Book *> findBooks(string partialTitle)
    {
        vector<Book *> results;
        partialSearch(root, partialTitle, results);
        return results;
    }

    vector<Book *> getAllBooks()
    {
        vector<Book *> books;
        inOrder(root, books);
        return books;
    }

    bool updateBook(string title, string newAuthor, int newYear, string newIsbn, bool newAvailable)
    {
        Book *book = searchNode(root, title);
        if (!book)
            return false;

        book->author = newAuthor;
        book->year = newYear;
        book->isbn = newIsbn;
        book->available = newAvailable;

        return true;
    }

    bool toggleAvailability(string title)
    {
        Book *book = searchNode(root, title);
        if (!book)
            return false;

        book->available = !book->available;
        return true;
    }
};

void displayMenu()
{
    cout << "\n========== LIBRARY MANAGEMENT SYSTEM ==========\n";
    cout << "1. Add a Book\n";
    cout << "2. Remove a Book\n";
    cout << "3. Find a Book by Exact Title\n";
    cout << "4. Search Books by Partial Title\n";
    cout << "5. Update Book Information\n";
    cout << "6. Toggle Book Availability\n";
    cout << "7. Display All Books\n";
    cout << "8. Exit\n";
    cout << "Please enter your choice (1-8): ";
}

void displayBook(Book *book)
{
    cout << "\n------------------------------------\n";
    cout << "Title: " << book->title << endl;
    cout << "Author: " << book->author << endl;
    cout << "Year: " << book->year << endl;
    if (!book->isbn.empty())
        cout << "ISBN: " << book->isbn << endl;
    cout << "Status: " << (book->available ? "Available" : "Checked Out") << endl;
    cout << "------------------------------------\n";
}

void displayBooks(const vector<Book *> &books)
{
    if (books.empty())
    {
        cout << "\nNo books found.\n";
        return;
    }

    cout << "\n======== BOOK COLLECTION (" << books.size() << " books) ========\n";
    cout << setw(40) << left << "TITLE"
         << setw(25) << left << "AUTHOR"
         << setw(6) << left << "YEAR"
         << setw(15) << left << "ISBN"
         << "STATUS" << endl;
    cout << string(95, '-') << endl;

    for (const auto &book : books)
    {
        cout << setw(40) << left << (book->title.length() > 37 ? book->title.substr(0, 37) + "..." : book->title)
             << setw(25) << left << (book->author.length() > 22 ? book->author.substr(0, 22) + "..." : book->author)
             << setw(6) << left << book->year
             << setw(15) << left << (book->isbn.empty() ? "N/A" : book->isbn)
             << (book->available ? "Available" : "Checked Out") << endl;
    }
    cout << string(95, '-') << endl;
}

string getInputLine(const string &prompt)
{
    string input;
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, input);
    return input;
}

int getInputInt(const string &prompt)
{
    int input;
    string line;
    bool valid = false;

    do
    {
        cout << prompt;
        getline(cin, line);

        try
        {
            input = stoi(line);
            valid = true;
        }
        catch (...)
        {
            cout << "Invalid input. Please enter a number.\n";
        }
    } while (!valid);

    return input;
}

int main()
{
    LibrarySystem library;
    int choice;
    string title, author, isbn;
    int year;
    bool running = true;

    // Add some sample books
    library.addBook("Harry Potter and the Philosopher's Stone", "J.K. Rowling", 1997, "9780747532743");
    library.addBook("The Hobbit", "J.R.R. Tolkien", 1937, "9780618260300");
    library.addBook("1984", "George Orwell", 1949, "9780451524935");
    library.addBook("To Kill a Mockingbird", "Harper Lee", 1960, "9780446310789");
    library.addBook("Pride and Prejudice", "Jane Austen", 1813, "9780141439518");

    cout << "Welcome to the Library Management System!" << endl;
    cout << "Sample books have been added to get you started." << endl;

    while (running)
    {
        displayMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
        { // Add a Book
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- ADD A NEW BOOK --\n";
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter publication year: ";
            cin >> year;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter ISBN (optional): ";
            getline(cin, isbn);

            library.addBook(title, author, year, isbn);
            break;
        }
        case 2:
        { // Remove a Book
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- REMOVE A BOOK --\n";
            cout << "Enter the exact title of the book to remove: ";
            getline(cin, title);

            if (library.removeBook(title))
            {
                cout << "Book '" << title << "' successfully removed." << endl;
            }
            else
            {
                cout << "Book not found. Please check the title and try again." << endl;
            }
            break;
        }
        case 3:
        { // Find Book by Exact Title
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- FIND A BOOK BY EXACT TITLE --\n";
            cout << "Enter the exact title to search for: ";
            getline(cin, title);

            Book *book = library.findBook(title);
            if (book)
            {
                displayBook(book);
            }
            else
            {
                cout << "Book not found. Please check the title and try again." << endl;
            }
            break;
        }
        case 4:
        { // Search Books by Partial Title
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- SEARCH BOOKS BY PARTIAL TITLE --\n";
            cout << "Enter search term: ";
            getline(cin, title);

            vector<Book *> results = library.findBooks(title);
            if (!results.empty())
            {
                cout << "Found " << results.size() << " matching books:" << endl;
                displayBooks(results);
            }
            else
            {
                cout << "No books found matching '" << title << "'." << endl;
            }
            break;
        }
        case 5:
        { // Update Book Information
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- UPDATE BOOK INFORMATION --\n";
            cout << "Enter the exact title of the book to update: ";
            getline(cin, title);

            Book *book = library.findBook(title);
            if (book)
            {
                displayBook(book);

                cout << "\nEnter new information (leave blank to keep current value):" << endl;

                cout << "New author [" << book->author << "]: ";
                getline(cin, author);
                if (author.empty())
                    author = book->author;

                string yearStr;
                cout << "New year [" << book->year << "]: ";
                getline(cin, yearStr);
                year = yearStr.empty() ? book->year : stoi(yearStr);

                cout << "New ISBN [" << (book->isbn.empty() ? "N/A" : book->isbn) << "]: ";
                getline(cin, isbn);
                if (isbn.empty())
                    isbn = book->isbn;

                string availStr;
                cout << "Availability (y/n) [" << (book->available ? "y" : "n") << "]: ";
                getline(cin, availStr);
                bool avail = availStr.empty() ? book->available : (availStr == "y" || availStr == "Y");

                if (library.updateBook(title, author, year, isbn, avail))
                {
                    cout << "Book information updated successfully!" << endl;
                }
                else
                {
                    cout << "Error updating book information." << endl;
                }
            }
            else
            {
                cout << "Book not found. Please check the title and try again." << endl;
            }
            break;
        }
        case 6:
        { // Toggle Book Availability
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n-- TOGGLE BOOK AVAILABILITY --\n";
            cout << "Enter the exact title of the book: ";
            getline(cin, title);

            if (library.toggleAvailability(title))
            {
                Book *book = library.findBook(title);
                cout << "Book '" << title << "' is now "
                     << (book->available ? "available" : "checked out") << "." << endl;
            }
            else
            {
                cout << "Book not found. Please check the title and try again." << endl;
            }
            break;
        }
        case 7:
        { // Display All Books
            vector<Book *> allBooks = library.getAllBooks();
            displayBooks(allBooks);
            break;
        }
        case 8:
        { // Exit
            cout << "Thank you for using the Library Management System. Goodbye!" << endl;
            running = false;
            break;
        }
        default:
        {
            cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
            break;
        }
        }
    }

    return 0;
}