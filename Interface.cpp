//██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗ 
//██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝ 
//██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝  
//██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝   
//███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║    
//╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    
//=====================================================

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

void titleScreen() {
    while (true) {
        cout << R"(
                                              |-======================================================-|
                                              |                                                        |
                                              |  ██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗   ██╗  |
                                              |  ██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚██╗ ██╔╝  |
                                              |  ██║     ██║██████╔╝██████╔╝███████║██████╔╝ ╚████╔╝   |
                                              |  ██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗  ╚██╔╝    |
                                              |  ███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║   ██║     |
                                              |  ╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝     |
                                              |-======================================================-|)" << endl;
        cout << "\t\t\t\t\t      | Created by Respicio | Bartolome | Bernardo | Natividad |" << endl;
        cout << "\t\t\t\t\t      |-======================================================-|" << endl;
        cout << "\t\t\t\t\t\t\t\tEnter [1] to proceed" << endl;
        cout << "\t\t\t\t\t\t\t\tEnter [2] to exit" << endl;
        cout << "\t\t\t\t\t\t\t-=================================-" << endl;
        cout << "\t\t\t\t\t\t\t\tChoose an option: ";
        int titleChoice;
        cin >> titleChoice;
        switch (titleChoice) {
            case 1:
                cout << "Proceeding to the main menu..." << endl;
                this_thread::sleep_for(chrono::seconds(2));
                system("clear");
                return;
            case 2:
                cout << "Exiting..." << endl;
                this_thread::sleep_for(chrono::seconds(2));
                exit(0);
            default:
                cout << "Error: Invalid option." << endl;
                cin.clear();
                this_thread::sleep_for(chrono::seconds(2));
                system("clear");
        }
    }
}

void menu() {
    int choice;
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
    switch (choice) {
        case 1:
            cout << "(Call Search Function)" << endl;
            // Search function go here // I also assume there's a loop here
            cin.ignore();
            cout << "Press any key to continue." << endl;
            cin.get();
            system("clear");
            break;
        case 2:
            cout << "(Call View Function)" << endl;
            // View function go here
            cin.ignore();
            cout << "Press any key to continue." << endl;
            cin.get();
            system("clear");
            break;
        case 3:
            cout << "(Call Add Function)" << endl;
            // Add function go here // I also assume there's a loop here
            cin.ignore();
            cout << "Press any key to continue." << endl;
            cin.get();
            system("clear");
            break;
        case 4:
            cout << "(Call Remove Function)" << endl;
            // Remove function go here // I also assume there's a loop here
            cin.ignore();
            cout << "Press any key to continue." << endl;
            cin.get();
            system("clear");
            break;
        case 5:
            cout << "Exiting..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            exit(0);
            break;
        default:
            cout << "Invalid option. Try again." << endl;
            cin.clear();
            this_thread::sleep_for(chrono::seconds(2));
            system("clear");
            break;
    }
}

int main() {
    titleScreen();
    while (true) {
        menu();
        cout << endl;
    }
    return 0;
}

// ok so i dunno if we can do descriptions of the books here cus it'll be long af
// like imagine putting "When Mr. Earnshaw, master of Wuthering Heights, returns from a trip with an unkempt orphan in tow, 
// he announces that the child, Heathcliff, is now a member of the family. 
// While young Catherine Earnshaw becomes close with Heathcliff, her older brother Hindley sinks into bitter resentment. 
// As Catherine and Heathcliff mature, and their affection blossoms into desire, Hindley's resentment boils over into hatred, 
// setting the stage for a tragic drama whose aftermath will shake the foundations of their world."

// I also assume the view books will list em as
// =============
// 1. Book Title
// Book Author
// Edition
// Publication Date
// Publisher
// ISBN
// =============
// =============
// 2. Book Title
// Book Author
// Edition
// Publication Date
// Publisher
// ISBN
// =============
