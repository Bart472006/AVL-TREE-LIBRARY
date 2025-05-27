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
    cout << "\t\t\t\t\t\t\t\t5. Exit" << endl;
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
    {7, "Bleach", "Tite Kubo", "Shueisha", "2001", "9780000007", "Manga"},
    {8, "Wuthering Heights", "Emily Bronte", "Union Square & Co.", "2023", "9781454952961", "Classic"},
    {9, "Don Quixote", "Miguel de Cervantes", "Rupa & Co.", "2023", "9789355209948", "Classic"},
    {10, "The Story of the Stone, Vol. 1", "Cao Xueqin", "Penguin Classics", "1974", "9780140442939", "Classic"},
    {11, "Moby Dick", "Herman Melville", "Wordsworth Editions Ltd.", "1999", "9781853260087", "Classic"},
    {12, "The Odyssey", "Homer", "Fingerprint! Publishing", "2015", "9788175993051", "Classic"},
    {13, "Demian", "Hermann Hesse", "Martino Fine Books", "2011", "9781614270263", "Classic"},
    {14, "The Divine Comedy", "Dante Aligheiri", "Canterbury Classics", "2013", "9781607109914", "Classic"},
    {15, "Crime and Punishment", "Fyodor Dostoevsky", "Vintage", "1993", "9780679734505", "Classic"},
    {16, "Goethe’s Faust", "Johann Wolfgang von Goethe", "Vintage", "1962", "9780385031141", "Classic"},
    {17, "The Stranger", "Albert Camus", "Vintage", "1989", "9780679720201", "Classic"},
    {18, "I’m Glad My Mom Died", "Jennette McCurdy", "Simon & Schuster", "2022", "9781982185824", "Memoir"},
    {19, "Gordon Ramsay’s Ultimate Home Cooking", "Gordon Ramsay", "Hodder & Stoughton", "2013", "9781444780789", "Cooking"},
    {20, "Noli Me Tangere", "Jose Rizal", "Penguin Classics", "2006", "9780143039693", "Classic"},
    {21, "The Art of War", "Sun Tzu", "Filiquarian", "2007", "9781599869773", "Philosophy"},
    {22, "Dog Man: A Tale of Two Kitties", "Dav Pilkey", "Graphix", "2021", "9781338741056", "Children"},
    {23, "Excel for Dummies", "Greg Harvey", "For Dummies", "1994", "9781568840505", "Educational"},
    {24, "Infinity Crusade, Vol. 1", "Jim Starlin", "Marvel Universe", "2012", "9780785131274", "Comics"},
    {25, "Studio Ghibli: The Complete Works", "Studio Ghibli", "Vertical", "2022", "9781647291495", "Art"},
    {26, "The Very Hungry Caterpillar", "Eric Carle", "World of Eric Carle", "1994", "9780399226908", "Children"},
    {27, "The Housemaid", "Freida McFadden", "Grand Central Publishing", "2022", "9781538742570", "Thriller"},
    {28, "No Longer Human", "Osamu Dazai", "New Directions", "1973", "9780811204811", "Classic"},
    {29, "Bridge to Terabithia", "Katherine Paterson", "HarperCollins", "2008", "9780060734015", "Fiction"},
    {30, "Runner", "Robert Newton", "Penguin Australia Pty Ltd.", "2005", "9780143302070", "Fiction"},
    {31, "Paradise Lost", "John Milton", "Penguin Classics", "2003", "9780140424393", "Classic"},
    {32, "The Three Musketeers", "Alexandre Dumas", "HarperCollins", "2020", "9780007902156", "Classic"},
    {33, "Ars Goetia", "Unknown Author", "Secret Fire Publishing", "2020", "9781999524227", "Occult"},
    {34, "Hell Screen", "Ryunosuke Akutagawa", "Penguin Classics", "2011", "9780141195728", "Classic"},
    {35, "Jane Eyre", "Charlotte Bronte", "Wordsworth Editions Ltd", "1997", "9781853260209", "Classic"},
    {36, "Frankenstein", "Mary Shelley", "Union Square & Co.", "2024", "9781454952893", "Classic"},
    {37, "Dracula", "Bram Stoker", "Union Square & Co.", "2024", "9781454952879", "Classic"},
    {38, "Data Structures: A Pseudocode Approach with C", "Richard F. Gilberg, Behrouz A. Forouzan", "Cengage Learning", "2004", "9780534390808", "Education"},
    {39, "C Interfaces and Implementations", "David R. Hanson", "Addison-Wesley Professional", "1996", "9780201498417", "Education"},
    {40, "Software Engineering: A Practitioner’s Approach", "Roger S. Pressman", "McGraw-Hill", "1996", "9780070521827", "Education"},
    {41, "Fundamentals of Software Engineering", "Rajib Mall", "Prentice Hall India", "2014", "9788120348981", "Education"},
    {42, "Software Theory", "Federica Frabetti", "Rowman & Littlefield", "2014", "9781783481972", "Education"},
    {43, "Discrete q-Distributions", "Charalambos A. Charalambides", "Wiley", "2016", "9781119119104", "Education"},
    {44, "Information Visualization", "Colin Ware", "Morgan Kaufmann", "2012", "9780123814647", "Education"},
    {45, "Algorithmic Game Theory", "Noam Nisan", "Cambridge University Press", "2007", "9780521872829", "Education"},
    {46, "Cellular Automata", "Joel L. Schiff", "Wiley-Interscience", "2008", "9780470168790", "Education"},
    {47, "Schaum’s Outline of Calculus", "Elliott Mendelson, Frank Ayres", "McGraw-Hill", "1999", "9780070419735", "Education"},
    {48, "Thomas’ Calculus", "George B. Thomas", "Addison-Wesley", "2000", "9780201441413", "Education"},
    {49, "Elements of Compiler Design", "Alexander Meduna", "Auerbach Publications", "2007", "9781420063257", "Education"},
    {50, "Web Coding & Development All-in-One", "Paul McFedries", "For Dummies", "2024", "9781394197026", "Education"},
    {51, "Languages and Machines", "Thomas A. Sudkamp", "Addison Wesley", "1996", "9780201821369", "Education"},
    {52, "Computer Software Applications in Chemistry", "Peter C. Jurs", "Wiley-Interscience", "1996", "9780471105879", "Education"},
    {53, "Chemical Process and Thermodynamics", "B.G. Kyle", "Prentice Hall PTR", "1999", "9780130812445", "Education"},
    {54, "3000 Solved Problems in Organic Chemistry", "Estelle K. Meislich, Herbert Meislich, Joseph Sharefkin", "McGraw-Hill", "1994", "9780070531079", "Education"},
    {55, "Introduction to Semimicro Qualitative Analysis", "Theodore Brown", "Pearson", "2004", "9780130462169", "Education"},
    {56, "Quantum Mechanics", "Eugen Merzbacher", "Wiley", "1997", "9780471887027", "Education"},
    {57, "Introductory Nuclear Physics", "Samuel S.M. Wong", "Wiley-VCH", "1999", "9780471239734", "Education"},
    {58, "A First Course in Fluid Dynamics", "A.R. Paterson", "Cambridge University Press", "1984", "9780521274241", "Education"},
    {59, "Thermodynamics", "William Z. Black, James G. Hartley", "Pearson", "1997", "9780673996480", "Education"},
    {60, "Solid State Physics", "C.M. Kachhava", "New Age International Publisher", "2003", "9788122415001", "Education"},
    {61, "Cioffari’s Experiments in College Physics", "Dean S. Edmonds, Jr.", "D.C Heath and Company", "1988", "9780669148534", "Education"},
    {62, "Heat and Power Thermodynamics", "James Kamm", "Delmar Pub", "1996", "9780827372573", "Education"},
    {63, "Designing Complex Systems", "Erik W. Aslaksen", "Auerbach Publications", "2016", "9781420087543", "Education"},
    {64, "Occupational Ergonomics", "Waldemar Karwowski, William S. Marras", "CRC Press", "2003", "9780203010457", "Education"},
    {65, "Human Factors Engineering and Ergonomics", "Stephen J. Guastello", "CRC Press", "2017", "9781138411487", "Education"},
    {66, "Introduction to Control Engineering", "Ajit K. Mandal", "New Age International Pvt. Ltd.", "2010", "9788122433906", "Education"},
    {67, "Logistics Engineering and Management", "Benjamin Blanchard", "Pearson", "2003", "9780131429154", "Education"}
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
