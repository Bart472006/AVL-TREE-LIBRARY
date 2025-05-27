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
    string month;      
    string day;       
    string year;     
    string isbn;
    string category;
    string callNumber; 
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
    {"The Logic and Design of Computer Programs", "Jim Messinger", "Pearson", "October", "15", "2004", "9781576761304", "Computer Science", "QA 76.6 M47 2005"},
    {"C Interfaces and Implementations", "David R. Hanson", "Addison-Wesley Professional", "August", "20", "1996", "9780201498417", "Computer Science", "QA 76.73 C15H37 1997"},
    {"Software Engineering: A Practitioner’s Approach", "Roger S. Pressman", "McGraw-Hill Companies", "January", "1", "1996", "9780070521827", "Computer Science", "QA 76.6 P72 1997"},
    {"Fundamentals of Software Engineering", "Rajib Mall", "Prentice Hall India Learning Private Limited", "January", "1", "2014", "9788120348981", "Computer Science", "QA 76.758 F86 2014"},
    {"Contemporary Chemical Analysis", "Judith F. Rubinson, Kenneth A. Rubinson", "Pearson College Div", "January", "1", "1998", "9780135193310", "Chemistry", "QD 75.2 R82 1998"},
    {"Discrete q-Distributions", "Charalambos A. Charalambides", "Wiley", "February", "11", "2016", "9781119119104", "Mathematics", "QA 273.6 C42 2016"},
    {"Modern Thermodynamics: From Heat Engines to Dissipative Structures", "Dilip Kondepudi, Ilya Prigogine", "John Wiley & Sons", "August", "17", "1998", "9780471973942", "Physics", "QC 311 K66 1998"},
    {"Algorithmic Game Theory", "Noam Nisan", "Cambridge University Press", "September", "24", "2007", "9780521872829", "Computer Science", "QA 269 A43 2007"},
    {"Cellular Automata: A Discrete View of the World", "Joel L. Schiff", "Wiley-Interscience", "January", "6", "2008", "9780470168790", "Computer Science", "QA 267.5 C45S34 2008"},
    {"Schaum’s Outline of Calculus", "Elliott Mendelson, Frank Ayres", "McGraw-Hill", "June", "28", "1999", "9780070419735", "Mathematics", "GQ 303 A97 2000"},
    {"Thomas’ Calculus", "George B. Thomas", "Addison-Wesley", "July", "24", "2000", "9780201441413", "Mathematics", "QA 303 F56 2001"},
    {"Elements of Compiler Design", "Alexander Meduna", "Auerbach Publications", "December", "3", "2007", "9781420063257", "Computer Science", "QA 76.6 M42 2008"},
    {"Web Coding & Development All-in-One For Dummies", "Paul McFedries", "For Dummies", "January", "31", "2024", "9781394197026", "Computer Science", "QA 76.6225 M32 2018"},
    {"Conceptual Physics", "Paul G. Hewitt", "Addison-Wesley", "November", "5", "2009", "9780321568090", "Physics", "QC 23.22 H48 010"},
    {"Schaum’s Outline of Theory and Problems of Theoretical Mechanics", "Murray R. Spiegel", "McGraw-Hill", "September", "16", "1980", "9780070990258", "Physics", "QC 125.2 S64 1982"},
    {"Introduction to High Energy Physics", "Donald H. Perkins", "Cambridge University Press", "April", "24", "2000", "9780521621960", "Physics", "QC 93.2 P47 2000"},
    {"Computer Graphics and Chemical Structures", "Stanley V. Kasparek", "Wiley-Interscience", "January", "29", "1990", "9780471628224", "Chemistry", "QD 471 K37 1990"},
    {"Statistics for Science and Engineering", "John Kinney", "Pearson", "November", "13", "2001", "9780201437201", "Mathematics", "QA 276.12 K56 2003"},
    {"Quantum Mechanics", "Eugen Merzbacher", "Wiley", "December", "1", "1997", "9780471887027", "Physics", "QC 174.12 M47 1998"},
    {"Introductory Nuclear Physics", "Samuel S.M. Wong", "Wiley-VCH", "April", "15", "1999", "9780471239734", "Physics", "QC 173 W65 1998"},
    {"Mathematical Statistics with Applications", "Dennis Wackerly", "Duxbury Press", "March", "11", "1996", "9780534209186", "Mathematics", "QA 276 W32 1996"},
    {"Thermodynamics", "William Z. Black, James G. Hartley", "Pearson", "January", "23", "1997", "9780673996480", "Physics", "QC 311 B52 1996"},
    {"Solid State Physics", "C.M. Kachhava", "New Age International Publisher", "January", "1", "2003", "9788122415001", "Physics", "QC 176 S35 2017"},
    {"Cioffari’s Experiments in College Physics", "Dean S. Edmonds, Jr.", "D.C Heath and Company", "January", "1", "1988", "9780669148534", "Physics", "QC 32 E35 1993"},
    {"Heat and Power Thermodynamics", "James Kamm", "Delmar Pub", "October", "15", "1996", "9780827372573", "Physics", "QC 311 K35 1997"},
    {"Designing Complex Systems: Foundations of Design in the Functional Domain", "Erik W. Aslaksen", "Auerbach Publications", "April", "19", "2016", "9781420087543", "Engineering", "TA 168 A74 2009"},
    {"Occupational Ergonomics: Design and Management of Work Systems", "Waldemar Karwowski, William S. Marras", "CRC Press", "March", "26", "2003", "9780203010457", "Engineering", "TA 166 O222 2003"},
    {"Human Factors Engineering and Ergonomics", "Stephen J. Guastello", "CRC Press", "June", "30", "2017", "9781138411487", "Engineering", "TA 166 G82 2014"},
    {"Introduction to Control Engineering", "Ajit K. Mandal", "New Age International Pvt. Ltd.", "August", "22", "2010", "9788122433906", "Engineering", "TA 165 M36 2006"},
    {"Logistics Engineering and Management", "Benjamin Blanchard", "Pearson", "September", "8", "2003", "9780131429154", "Engineering", "TA 168 B5 2004"},
    {"Analytic Geometry and Calculus", "Lewis Parker Siceloff", "Merchant Books", "June", "8", "2007", "1603860185", "Mathematics", "QA 551 W88"},
    {"Introduction to Modern Algebra and Matrix Theory", "Otto Schreier and Emanuel Sperner", "Dover Publications", "July", "19", "2011", "0486482200", "Mathematics", "QA 551 S374"},
    {"Elementary Statistics: A Step by Step Approach", "Allan G. Bluman", "Wm. C. Brown Publishers", "October", "1", "1994", "0697243486", "Mathematics", "QA 276.12 D67 1990"},
    {"Trigonometry: Enhanced with Graphing Utilities (4th Edition)", "Michael Sullivan", "Pearson Prentice Hall", "May", "11", "2005", "0131527266", "Mathematics", "QA531 .S94 2006"},
    {"Building Adaptation", "James Douglas", "Butterworth-Heinemann", "March", "15", "2006", "0750666676", "Architecture", "TH 3401 .D68 2006"},
    {"Facilities Change Management", "Edward Finch", "Wiley-Blackwell", "November", "14", "2011", "9781405153461", "Architecture", "TH 3411 .R5 Finch 2015"},
    {"Construction Technology 3: The Technology of Refurbishment and Maintenance", "Mike Riley; Alison Cotgrave", "Red Globe Press (Bloomsbury)", "April", "28", "2011", "9780230290143", "Architecture", "TH 4311 .S87 2011"},
    {"Sustainable Retrofitting of Commercial Buildings", "Simon Burton", "John Wiley & Sons", "October", "3", "2014", "9780415834247", "Architecture", "TH 4311 .S87 2015"},
    {"The Construction of Houses", "Duncan Marshall; Derek Worthing; Nigel Dunn; Roger Heath", "Estates Gazette (Taylor & Francis)", "April", "3", "2013", "9780080971001", "Architecture", "TH 4811 .C68 2013"},
    {"Residential Building Design and Construction", "Jack H. Willenbrock; Harvey B. Manbeck; Michael G. Suchar", "Pearson College Div", "January", "1", "1997", "9780133758740", "Architecture", "TH 4811 .W54"},
    {"Basic Engineering Calculations for Contractors", "August W. Domel", "McGraw-Hill", "December", "22", "1996", "9780070180024", "Architecture", "TH 4812 .S7375"},
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
