James Weber
jaweber7@asu.edu
CSE 310 AVL Tree Honors Project

Code Files:
Main.cpp
AVLTree.h

Compile:
g++ -std=c++11 -o prog *.cpp *.h

Run:
./prog

Example Input:

I
500             # Insert(500)
P               # Print Tree
I       
250             # Insert(250)
P               # Print Tree
I       
100             # Insert(100) - Unbalanced Case 1 (Left)
P               # Print Tree
I       
50              # Insert(50)
P               # Print Tree
I       
75              # Insert(75)  - Unbalanced Case 2 (Left)
P               # Print Tree
D       
500             # Delete(500) - Unbalanced Case 1 (Left)
P               # Print Tree
D       
50              # Delete(50)  - Unbalanced Case 2 (Right)
P               # Print Tree
D       
100     
P               # Print Tree
D       
250     
P               # Print Tree
D       
75      
P               # Print Tree
Q               # Quit
