//************************************************************************
// ASU CSE310 AVL Tree Honors Project Fall 2022
// Author: James Weber
// ASU ID: 1220618282
// Description: Driver program which allows user input to run commands
//                  insert, search, delete, and print on an AVL tree
//************************************************************************

#include "AVLTree.h"
#include <sstream>

int main() {

    /* Instantiate AVLTree */
    AVLTree *tree = new AVLTree();

    /* User Input Variables */
    char command;
    int key;

    /* Available User Commands */
    const char INSERT = 'I';
    const char SEARCH = 'S';
    const char DELETE = 'D';
    const char PRINT = 'P';
    const char QUIT = 'Q';

    /* Run loop asking for user input */
    do {
        /* Receive Command */
        // prompt user command
        cout << "Commands:" << endl
            << left << INSERT << right << setw(12) << "Insert" << endl
            << left << SEARCH << right << setw(12) << "Search" << endl
            << left << DELETE << right << setw(12) << "Delete" << endl
            << left << PRINT << right << setw(12) << "Print" << endl
            << left << QUIT << right << setw(12) << "Quit" << endl
            << endl;
        cout << "Please Enter a Command: ";
        // accept user command
        cin >> command;
        command = toupper(command); // uppercase

        if (command == INSERT) {
            // prompt Insert key
            cout << "Please Enter a Key to Insert: ";
            // accept Insert key
            cin >> key;

            /* Insert Node */
            tree->insert(key);
            cout << "Inserted Node with key " << key << endl;
        } else if (command == SEARCH) {
            // prompt Search key
            cout << "Please Enter a Key to Search for: ";
            // accept Search key
            cin >> key;

            /* Search for Node */
            bool found = tree->search(key);
            cout << "Node with key " << key << " is ";
            if (found) {
                cout << "found" << endl;
            } else {
                cout << "NOT found" << endl;
            }
        } else if (command == DELETE) {
            // prompt Delete key
            cout << "Please Enter a Key to Delete: ";
            // accept Delete key
            cin >> key;

            /* Delete Node */
            TreeNode deleted = tree->deleteNode(key);
            if (deleted.key == key) {
                cout << "Deleted" << endl;
                tree->printNode(&deleted);
            } else {
                cout << "Node with key " << key << " is NOT found" << endl;
            }

        } else if (command == PRINT) {
            tree->print();
        }

        cout << endl;
    } while (command != QUIT);

    /* Delete Tree */
    cout << "Deleting Tree" << endl;
    tree->print();
    delete tree;

    return 0;
}