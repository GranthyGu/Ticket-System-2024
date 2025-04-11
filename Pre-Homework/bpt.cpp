#include <iostream>
#include <string>
#include <fstream>

const int M = 1000;
const int L = 1000;

template<typename T>
class Node {
private:
    int is_leaf;
    int size;
    T[M] key;
    long[M] address_of_children;
    long address_of_the_right_node;
    
};