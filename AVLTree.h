//************************************************************************
// ASU CSE310 AVL Tree Honors Project Fall 2022
// Author: James Weber
// ASU ID: 1220618282
// Description: TreeNode struct and AVLTree class which performs insert,
//                  search, and delete while maintaining AVL property
//************************************************************************

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

/* Tree Node */
struct TreeNode
{
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    int balFactor;
    int height;
};


class AVLTree
{
 public:
    AVLTree();
    ~AVLTree();
    void insert(int key);
    bool search(int key);
    TreeNode deleteNode(int key);
    void print();
    void printNode(TreeNode *node);

 private:
    TreeNode *root;
    int nodeHeight(TreeNode *node);
    void updateHeight(TreeNode *node);
    int calculateHeight(int leftHeight, int rightHeight);
    void updateBalFactor(TreeNode *node);
    int calculateBalFactor(int leftHeight, int rightHeight);
    bool isRoot(TreeNode *node);
    bool isLeftChild(TreeNode *child, TreeNode *prev);
    bool isRightChild(TreeNode *child, TreeNode *prev);
    bool isBalanced(TreeNode *node);
    bool isCompletelyBalanced(TreeNode *node);
    bool isLeftHeavy(TreeNode *node);
    bool isRightHeavy(TreeNode *node);
    bool isLeftHeavier(TreeNode *node);
    bool isRightHeavier(TreeNode *node);
    void deleteTree(TreeNode *subRoot);
    int search(int key, TreeNode *subRoot);
    bool insert(int key, TreeNode *subRoot, TreeNode *prev);
    void bstInsertUpdate(TreeNode *subRoot, TreeNode *node);
    bool deleteNode(int key, TreeNode *subRoot, TreeNode *prev, TreeNode *data);
    void balanceTree(TreeNode *subRoot, TreeNode *prev);
    void rotateLeft(TreeNode *subRoot, TreeNode *prev);
    void rotateRight(TreeNode *subRoot, TreeNode *prev);
    void rotateRightLeft(TreeNode *subRoot, TreeNode *prev);
    void rotateLeftRight(TreeNode *subRoot, TreeNode *prev);
    void transplant(TreeNode *oldNode, TreeNode *replaceNode, TreeNode *prev);
    TreeNode* treeMinimum(TreeNode* subRoot);
    void printTree(TreeNode *subRoot);
};

/* Public Methods */

AVLTree::AVLTree() {
    root = NULL;
}

AVLTree::~AVLTree() {
    deleteTree(root);
}

void AVLTree::insert(int key) {
    insert(key, root, NULL);
}
/* return true if found */
bool AVLTree::search(int key) {
    return search(key, root) != -1;
}
/* return deleted node */
TreeNode AVLTree::deleteNode(int key) {
    TreeNode node = TreeNode();
    deleteNode(key, root, NULL, &node);
    return node;
}
void AVLTree::print() {
    printTree(root);
}

void AVLTree::printNode(TreeNode *node) {
    cout << left;
    cout << setw(12) << node->key
         << "(" << node->balFactor << ")" << endl;
}

/* Private Methods */

/* This function searches the tree and tries to find the node with a
key equal to the one passed into the function. If the node is not
found, this method should return -1. Otherwise, this method
should return the key found in the tree which rooted at subRoot. */
int AVLTree::search(int key, TreeNode *subRoot) {
    // Node Not Found - return -1
    if (subRoot == NULL)
        return -1;
    if (key < subRoot->key) // recurse left
        return search(key, subRoot->left);
    if (key > subRoot->key) // recurse right
        return search(key, subRoot->right);
    /* key == subRoot->key */
    /* Node Found: subRoot */
    return subRoot->key;
}

/* This function actually does the insertion work. It should not only
insert a node into the tree, but should make sure the tree is still a
valid AVL tree after the insertion. To achieve this objective, this
function may need to call the balanceTree() function. The
boolean value returned is used to indicate if the subTree rooted at
subRoot changed height. */
bool AVLTree::insert(int key, TreeNode *subRoot, TreeNode *prev) {
    /* Track Orginal Height */
    int originalHeight = nodeHeight(subRoot);

    /* Create Node */
    TreeNode *node = new TreeNode();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    updateBalFactor(node);
    updateHeight(node);

    /* BST Insert and update Balance and Height of nodes in path */
    bstInsertUpdate(subRoot, node);

    /* Fix AVL Property */
    balanceTree(node, node->parent);

    /* New Height */
    int newHeight = nodeHeight(subRoot);

    /* return true iff height did not change */
    return originalHeight == newHeight;
}

/* Insert node into subtree, and update balance and loadFactor of each tree in path */
void AVLTree::bstInsertUpdate(TreeNode *subRoot, TreeNode *node) {
    if (subRoot == NULL) { /* Special Case: Tree is Empty */
        root = node;
        return;
    }
    if (node->key < subRoot->key) { // node is left of subRoot
        if (subRoot->left == NULL) {// base case
            subRoot->left = node;   // subRoot→left = node
            node->parent = subRoot;
        } else {    // recurse left
            bstInsertUpdate(subRoot->left, node);
        }
    } else {                            // node is right of subRoot
        if (subRoot->right == NULL) {   // base case
            subRoot->right = node;      // subRoot→right = node
            node->parent = subRoot;
        } else {    // recurse right
            bstInsertUpdate(subRoot->right, node);
        }
    }
    // update balance and height
    updateBalFactor(subRoot);
    updateHeight(subRoot);
}

/* This function actually does the deletion work. It should not only
delete a node from the tree, but should make sure the tree is still a
valid AVL tree after the delete. To achieve this objective, this
function may need to call the balanceTree() function. The
boolean value returned is used to indicate if the subTree rooted at
subRoot changed height. The data parameter passed in is where
the deleted data should be placed. This data will then be returned
to the user. If the key is not found in the tree, this data variable
should not be changed at all. */
bool AVLTree::deleteNode(int key, TreeNode *subRoot, TreeNode *prev, TreeNode *data) {
    /* Track Orginal Height */
    int originalHeight = nodeHeight(subRoot);

    /* Find Node */
    // iterator node
    TreeNode *node = subRoot;
    while (node != NULL) {
        if (node->key == key) {
            /* Found Node */
            /* Delete From Tree */
            if (node->left == NULL) {
                transplant(node, node->right, prev);
            } else if (node->right == NULL) {
                transplant(node, node->left, prev);
            } else {
                TreeNode *y = treeMinimum(node->right);
                if (y->parent != node) {
                    transplant(y, y->right, y->parent);
                    y->right = node->right;
                    y->right->parent = y;
                }
                transplant(node, y, prev);
                y->left = node->left;
                y->left->parent = y;
            }

            *data = *node;
            delete node;

            if (prev != NULL)
                balanceTree(prev, prev->parent);

            /* New Height */
            int newHeight = nodeHeight(subRoot);
            /* return true iff height did not change */
            return originalHeight == newHeight;
        } else {
            /* recurse */
            prev = node;
            if (key < node->key) {
                /* recurse left */
                node = node->left;
            } else {
                /* recurse right */
                node = node->right;
            }
        }
    }

    /* Node Not Found */
    return false;
}

/* This function checks to see if the subRoot is right heavy
(balFactor == 2) or left heavy (balFactor == -2). It then calls the
appropriate rotate functions to re-balance the tree. */
void AVLTree::balanceTree(TreeNode *subRoot, TreeNode *prev) {
    if (subRoot == NULL) {
        if (prev != NULL) {
            balanceTree(prev, prev->parent);
        }
        return;
    }

    if (isLeftHeavy(subRoot)) {
        if (isLeftHeavier(subRoot->left) || isCompletelyBalanced(subRoot->left)) {
            rotateRight(subRoot, prev);
        } else {
            rotateLeftRight(subRoot, prev);
        }
    } else if (isRightHeavy(subRoot)) {
        if (isRightHeavier(subRoot->right) || isCompletelyBalanced(subRoot->right)) {
            rotateLeft(subRoot, prev);
        } else {
            rotateRightLeft(subRoot, prev);
        }
    }
    updateBalFactor(subRoot);
    updateHeight(subRoot);
    if (prev != NULL) {
        updateBalFactor(prev);
        updateHeight(prev);
        /* Recurse Upward */
        balanceTree(prev, prev->parent);
    }
}

/* This function replaces subRoot with its right child and makes the
subRoot the left child of the child that replaced it. */
void AVLTree::rotateLeft(TreeNode *subRoot, TreeNode *prev) {
    if (subRoot->right == NULL) {
        throw invalid_argument("Left-Rotate must be called on a node with a non-NULL right child.");
    }
    // y
    TreeNode* newSubtreeRoot = subRoot->right;    // Not NULL
    // x→r = β
    subRoot->right = newSubtreeRoot->left;
    if (subRoot->right != NULL)  // β→p = x
        subRoot->right->parent = subRoot;
    updateHeight(subRoot);
    updateBalFactor(subRoot);
    // y→l = x
    newSubtreeRoot->left = subRoot;
    newSubtreeRoot->left->parent = newSubtreeRoot; // x→p = y
    updateHeight(newSubtreeRoot);
    updateBalFactor(newSubtreeRoot);

    // link to tree
    // prev→child = y
    if (isRoot(subRoot)) { // special case: new tree root
        root = newSubtreeRoot;
        newSubtreeRoot->parent = NULL;  // root→p = NULL
    } else {
        if (isLeftChild(subRoot, prev)) {
            prev->left = newSubtreeRoot;
        } else {
            prev->right = newSubtreeRoot;
        }
        newSubtreeRoot->parent = prev;  // y→p = prev
        updateHeight(prev);
        updateBalFactor(prev);
    }
}
/* This function replaces subRoot with its left child and makes the
subRoot the right child of the child that replaced it. */
void AVLTree::rotateRight(TreeNode *subRoot, TreeNode *prev) {
    if (subRoot->left == NULL) {
        throw invalid_argument("Right-Rotate must be called on a node with a non-NULL left child.");
    }
    // x
    TreeNode* newSubtreeRoot = subRoot->left;    // Not NULL
    // y→l = β
    subRoot->left = newSubtreeRoot->right;
    if (subRoot->left != NULL)  // β→p = y
        subRoot->left->parent = subRoot;
    updateHeight(subRoot);
    updateBalFactor(subRoot);
    // x→r = y
    newSubtreeRoot->right = subRoot;
    newSubtreeRoot->right->parent = newSubtreeRoot; // y→p = x
    updateHeight(newSubtreeRoot);
    updateBalFactor(newSubtreeRoot);

    // link to tree
    // prev→child = x
    if (isRoot(subRoot)) { // special case: new tree root
        root = newSubtreeRoot;
        newSubtreeRoot->parent = NULL;  // root→p = NULL
    } else {
        if (isLeftChild(subRoot, prev)) {
            prev->left = newSubtreeRoot;
        } else {
            prev->right = newSubtreeRoot;
        }
        newSubtreeRoot->parent = prev;  // x→p = prev
        updateHeight(prev);
        updateBalFactor(prev);
    }
}
/* This function first calls rotateRight() with subRoot's right child. It
then calls rotateLeft( ) on the subRoot itself. */
void AVLTree::rotateRightLeft(TreeNode *subRoot, TreeNode *prev) {
    rotateRight(subRoot->right, subRoot);
    rotateLeft(subRoot, prev);
}
/* This function first calls rotateLeft( ) with subRoot's left child. It
then calls rotateRight( ) on the subRoot itself. */
void AVLTree::rotateLeftRight(TreeNode *subRoot, TreeNode *prev) {
    rotateLeft(subRoot->left, subRoot);
    rotateRight(subRoot, prev);
}

void AVLTree::transplant(TreeNode *oldNode, TreeNode *replaceNode, TreeNode *prev) {
    if (prev == NULL) {
        /* New Root */
        root = replaceNode;
    } else if (oldNode == prev->left) {
        /* left child */
        prev->left = replaceNode;
    } else {
        /* right child */
        prev->right = replaceNode;
    }
    if (replaceNode != NULL)
        replaceNode->parent = prev;

    /* Update Height and BalFactor */
    if (replaceNode != NULL) {
        updateHeight(replaceNode);
        updateBalFactor(replaceNode);
    }
    if (prev != NULL) {
        updateHeight(prev);
        updateBalFactor(prev);
    }
}

TreeNode* AVLTree::treeMinimum(TreeNode* subRoot) {
    if (subRoot == NULL || subRoot->left == NULL)
        return subRoot;
    return treeMinimum(subRoot->left);
}

/* This function simply prints out the AVL tree rooted at subRoot.
For each node, it should show the data and the balance factor for
the node (this is put in parenthesis after the data). */
void AVLTree::printTree(TreeNode *subRoot) {
    if (subRoot == NULL)
        return;
    /* in-order */
    // left→node→right
    printTree(subRoot->left);
    printNode(subRoot);
    printTree(subRoot->right);
}

/* Helper Methods */

/* Delete Tree rooted at subRoot from memory */
void AVLTree::deleteTree(TreeNode *subRoot) {
    if (subRoot == NULL)
        return;
    deleteTree(subRoot->left);
    deleteTree(subRoot->right);
    delete subRoot;
}

int AVLTree::nodeHeight(TreeNode *node) {
    if (node == NULL)
        return -1;
    return node->height;
}

void AVLTree::updateHeight(TreeNode *node) {
    node->height = calculateHeight(nodeHeight(node->left), nodeHeight(node->right));
}

int AVLTree::calculateHeight(int leftHeight, int rightHeight) {
    // 1 + max(left,right)
    return 1 + max(leftHeight, rightHeight);
}

/* calculate and update balFactor of all nodes in tree rooted at subroot */
/* return node height */
void AVLTree::updateBalFactor(TreeNode *node) {
    node->balFactor = calculateBalFactor(nodeHeight(node->left), nodeHeight(node->right));
}

int AVLTree::calculateBalFactor(int leftHeight, int rightHeight) {
    return rightHeight - leftHeight;
}

bool AVLTree::isRoot(TreeNode *node) {
    return node == root;
}

bool AVLTree::isLeftChild(TreeNode *child, TreeNode *prev) {
    return prev->left == child;
}

bool AVLTree::isRightChild(TreeNode *child, TreeNode *prev) {
    return prev->right == child;
}

/* is not unbalanced - left & right differ no more than 1 */
bool AVLTree::isBalanced(TreeNode *node) {
    return node->balFactor >= -1 && node->balFactor <= 1;
}

/* neither left or right is heavier */
bool AVLTree::isCompletelyBalanced(TreeNode *node) {
    return node->balFactor == 0;
}

/* Unbalanced towards Left */
bool AVLTree::isLeftHeavy(TreeNode *node) {
    if (node == NULL)
        return false;
    return node->balFactor < -1;
}

/* Unbalanced towards Right */
bool AVLTree::isRightHeavy(TreeNode *node) {
    if (node == NULL)
        return false;
    return node->balFactor > 1;
}

/* Left is heavier than right; not necessarily unbalanced */
bool AVLTree::isLeftHeavier(TreeNode *node) {
    if (node == NULL)
        return false;
    return node->balFactor < 0;
}

/* Right is heavier than left; not necessarily unbalanced */
bool AVLTree::isRightHeavier(TreeNode *node) {
    if (node == NULL)
        return false;
    return node->balFactor > 0;
}