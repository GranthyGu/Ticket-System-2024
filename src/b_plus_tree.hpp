// Created by GranthyGu on 2025/5/8

#ifndef B_PLUS_TREE
#define B_PLUS_TREE

/*
 * File: b_plus_tree.hpp
 * --------------------
 * This file exports a map class that has functions insert/delete/find
 * with a key (class T).
 * 
 * - Way to use: User can construct a B plus tree by 
 *      B_plus_tree<T, M, L> bpt;
 *   where,
 *      T -> the class of the key-value;
 *      M -> size of the node;
 *      L -> size of the leaf.
 * 
 * - Functions to use: 
 *      bpt.insert(T value);  
 *      which means insert a key-value into the bpt, return false if bpt has had the value.
 *      bpt.remove(T value);
 *      which means remove a key-value into the bpt, return false if bpt does not have the value.
 *      bpt.find(T minimal, T maximal);
 *      return to the vector contains all the element between minimal and maximal (closing).
 */

#include <iostream>
#include <string>
#include <fstream>
#include "STL/vector.hpp"

template<typename T, const int M, const int L>
class Node {
public:
    int is_leaf;
    int size;
    T key[M + 1] = {};
    long address_of_children[M + 1] = {-1};
    long address_of_parent;
    Node() : is_leaf(0), size(0), address_of_parent(-1) {}
    Node(int leaf, int size, T* key_, long* add, long add_parent) : is_leaf(leaf), size(size), address_of_parent(add_parent) {
        for (int i = 0; i < M; i++) {
            key[i] = key_[i];
            address_of_children[i] = add[i];
        }
    }
    Node<T, M, L>& operator=(const Node<T, M, L>& other) {
        this->is_leaf = other.is_leaf;
        this->size = other.size;
        this->address_of_parent = other.address_of_parent;
        for (int i = 0; i <= M; i++) {
            this->key[i] = other.key[i];
            this->address_of_children[i] = other.address_of_children[i];
        }
        return *this;
    }
    void write_to_file(std::fstream& File) {
        if (!File) {return;}
        File.write(reinterpret_cast<char*> (&is_leaf), sizeof(int));
        File.write(reinterpret_cast<char*> (&size), sizeof(int));
        File.write(reinterpret_cast<char*> (key), (M + 1)* sizeof(T));
        File.write(reinterpret_cast<char*> (address_of_children), (M + 1) * sizeof(long));
        File.write(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
    void read_from_file(std::fstream& File) {
        if (!File) {return;}
        File.read(reinterpret_cast<char*> (&is_leaf), sizeof(int));
        File.read(reinterpret_cast<char*> (&size), sizeof(int));
        File.read(reinterpret_cast<char*> (key), (M + 1) * sizeof(T));
        File.read(reinterpret_cast<char*> (address_of_children), (M + 1) * sizeof(long));
        File.read(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
};
template<typename T, typename V, const int M, const int L>
class leaf_Node {
public:
    int size;
    T key[L + 1] = {};
    V value[L + 1] = {};
    long address_of_parent;
    long address_of_right_node;
    leaf_Node() : size(0), address_of_right_node(-1), address_of_parent(-1) {}
    leaf_Node(int size, T* key_, V* value_, long* add, long add_, long add_parent) : size(size), address_of_right_node(add_), address_of_parent(add_parent) {
        for (int i = 0; i <= L; i++) {
            key[i] = key_[i];
            value[i] = value_[i];
        }
    }
    leaf_Node<T, V, M, L>& operator=(const leaf_Node<T, V, M, L>& other) {
        this->size = other.size;
        this->address_of_right_node = other.address_of_right_node;
        this->address_of_parent = other.address_of_parent;
        for (int i = 0; i <= L; i++) {
            this->key[i] = other.key[i];
            this->value[i] = other.value[i];
        }
        return *this;
    }
    void write_to_file(std::fstream& File) {
        if (!File) {return;}
        File.write(reinterpret_cast<char*> (&size), sizeof(int));
        File.write(reinterpret_cast<char*> (key), (L + 1)* sizeof(T));
        File.write(reinterpret_cast<char*> (value), (L + 1)* sizeof(V));
        File.write(reinterpret_cast<char*> (&address_of_right_node), sizeof(long));
        File.write(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
    void read_from_file(std::fstream& File) {
        if (!File) {return;}
        File.read(reinterpret_cast<char*> (&size), sizeof(int));
        File.read(reinterpret_cast<char*> (key), (L + 1) * sizeof(T));
        File.read(reinterpret_cast<char*> (value), (L + 1) * sizeof(V));
        File.read(reinterpret_cast<char*> (&address_of_right_node), sizeof(long));
        File.read(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
};
template<typename T, typename V, const int M, const int L>
class B_plus_tree {
private:
    std::string file_name;
    std::fstream File;
    long address_of_root;
    // To find the index in node.key, which is the first element less than target. If not exist, return -1.
    int binary_find(Node<T, M, L> node, T target) {
        T* arr = node.key;
        int left = 0;
        int right = node.size;
        while (left < right) {
            int mid = (left + right) / 2;
            if (target > arr[mid] || target == arr[mid]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left - 1;
    }
    int binary_find_value(leaf_Node<T, V, M, L> node, T target) {
        T* arr = node.key;
        int left = 0;
        int right = node.size;
        while (left < right) {
            int mid = (left + right) / 2;
            if (target > arr[mid] || target == arr[mid]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left - 1;
    }
    void recall_for_insert(long address, bool flag) {
        if (flag) {
            leaf_Node<T, V, M, L> parent;
            File.seekg(address);
            parent.read_from_file(File);
            Node<T, M, L> parent_parent;
            File.seekg(parent.address_of_parent);
            parent_parent.read_from_file(File);
            int index;
            for (int i = 0; i <= parent_parent.size; i++) {
                if (parent_parent.address_of_children[i] == address) {
                    index = i;
                    break;
                }
            }
            long address_parent = parent.address_of_parent;
            split_node(address_parent, index);
        } else {
            Node<T, M, L> parent;
            File.seekg(address);
            parent.read_from_file(File);
            if (parent.address_of_parent == -1) {   // Which means that the parent node is the root.
                Node<T, M, L> new_root;
                new_root.address_of_children[0] = address;
                new_root.address_of_parent = -1;
                File.seekp(0, std::ios::end);
                long root_address = File.tellp();
                new_root.write_to_file(File);
                address_of_root = root_address;
                parent.address_of_parent = root_address;
                File.seekp(address);
                parent.write_to_file(File);
                split_node(root_address, 0);
            } else {
                Node<T, M, L> parent_parent;
                File.seekg(parent.address_of_parent);
                parent_parent.read_from_file(File);
                int index;
                for (int i = 0; i <= parent_parent.size; i++) {
                    if (parent_parent.address_of_children[i] == address) {
                        index = i;
                        break;
                    }
                }
                long address_parent = parent.address_of_parent;
                split_node(address_parent, index);
            }
        }
    }
    // Split the ith child of the parent node.
    void split_node(long address, int k) {
        File.seekg(address);
        Node<T, M, L> parent;
        parent.read_from_file(File);
        if (parent.is_leaf == 1) {
            leaf_Node<T, V, M, L> child;
            File.seekg(parent.address_of_children[k]);
            child.read_from_file(File);
            leaf_Node<T, V, M, L> new_node;
            new_node.address_of_parent = child.address_of_parent;
            File.seekp(0, std::ios::end);
            long address_last = File.tellp();
            new_node.address_of_right_node = child.address_of_right_node;
            child.address_of_right_node = address_last;
            new_node.size = child.size / 2;
            child.size = child.size - new_node.size;
            for (int i = 0; i < new_node.size; i++) {
                new_node.key[i] = child.key[i + child.size];
                new_node.value[i] = child.value[i + child.size];
            }
            new_node.write_to_file(File);
            File.seekp(parent.address_of_children[k]);
            child.write_to_file(File);
            for (int i = parent.size; i > k; i--) {
                parent.key[i] = parent.key[i - 1];
                parent.address_of_children[i + 1] = parent.address_of_children[i];
            }
            parent.key[k] = child.key[child.size];
            parent.address_of_children[k + 1] = address_last;
        } else {
            Node<T, M, L> child;
            File.seekg(parent.address_of_children[k]);
            child.read_from_file(File);
            Node<T, M, L> new_node;
            new_node.is_leaf = child.is_leaf;
            new_node.address_of_parent = child.address_of_parent;
            File.seekp(0, std::ios::end);
            long address_last = File.tellp();
            new_node.size = child.size / 2;
            child.size = child.size - new_node.size - 1;
            for (int i = 0; i < new_node.size; i++) {
                new_node.key[i] = child.key[i + child.size + 1];
                new_node.address_of_children[i] = child.address_of_children[i + child.size + 1];
            }
            new_node.address_of_children[new_node.size] = child.address_of_children[new_node.size + child.size + 1];
            new_node.write_to_file(File);
            File.seekp(parent.address_of_children[k]);
            child.write_to_file(File);
            for (int i = 0; i <= new_node.size; i++) {
                if (new_node.is_leaf == 1) {
                    File.seekg(new_node.address_of_children[i]);
                    leaf_Node<T, V, M, L> tmp;
                    tmp.read_from_file(File);
                    tmp.address_of_parent = address_last;
                    File.seekp(new_node.address_of_children[i]);
                    tmp.write_to_file(File);
                } else {
                    File.seekg(new_node.address_of_children[i]);
                    Node<T, M, L> tmp;
                    tmp.read_from_file(File);
                    tmp.address_of_parent = address_last;
                    File.seekp(new_node.address_of_children[i]);
                    tmp.write_to_file(File);
                }
            }
            for (int i = parent.size; i > k; i--) {
                parent.key[i] = parent.key[i - 1];
                parent.address_of_children[i + 1] = parent.address_of_children[i];
            }
            parent.key[k] = child.key[child.size];
            parent.address_of_children[k + 1] = address_last;
        }
        parent.size++;
        File.seekp(address);
        parent.write_to_file(File);
        if (parent.size > M - 1) {
            recall_for_insert(address, false);
        }
        return;
    }
    // Insert the certain value to the certain node(found by the address of itself).
    bool insert_to_bpt(long address, const T& k, const V& v) {
        File.seekg(address);
        leaf_Node<T, V, M, L> node;
        node.read_from_file(File);
        int index = binary_find_value(node, k);
        if (index != -1 && node.key[index] == k) {
            return false;
        }
        for (int i = node.size; i > index + 1; i--) {
            node.key[i] = node.key[i - 1];
            node.value[i] = node.value[i - 1];
        }
        node.key[index + 1] = k;
        node.value[index + 1] = v;
        node.size++;
        File.seekp(address);
        node.write_to_file(File);
        if (node.size >= L) {
            recall_for_insert(address, true);
        }
        return true;
    }
    // For certain T, find its corresponding leaf. Begin the finding from the root, stop at the leaf.
    long find_corresponding_leaf(const T& value) {
        Node<T, M, L> node;
        File.seekg(address_of_root);
        long address = address_of_root;
        node.read_from_file(File);
        while (true) {
            int index = binary_find(node, value);
            File.seekg(node.address_of_children[index + 1]);
            address = node.address_of_children[index + 1];
            if (node.is_leaf == 0) {
                node.read_from_file(File);
            } else {
                break;
            }
        }
        return address;
    }
    // Usually, we will merge the node with the right one. However, for the last node, merge it with the left one.
    void merge_node(long address, int k) {
        File.seekg(address);
        Node<T, M, L> parent;
        parent.read_from_file(File);
        if (k == parent.size) {k--;}
        if (k < 0) {return;}
        if (parent.is_leaf == 1) {
            leaf_Node<T, V, M, L> child;
            File.seekg(parent.address_of_children[k]);
            child.read_from_file(File);
            leaf_Node<T, V, M, L> right_child;
            File.seekg(parent.address_of_children[k + 1]);
            right_child.read_from_file(File);
            int total_size = right_child.size + child.size;
            if (total_size <= L - 1) {
                for (int i = 0; i < right_child.size; i++) {
                    child.value[i + child.size] = right_child.value[i];
                    child.key[i + child.size] = right_child.key[i];
                }
                child.size += right_child.size;
                child.address_of_right_node = right_child.address_of_right_node;
                for (int i = k; i < parent.size - 1; i++) {
                    parent.key[i] = parent.key[i + 1];
                    parent.address_of_children[i + 1] = parent.address_of_children[i + 2];
                }
                parent.size--;
                File.seekp(address);
                parent.write_to_file(File);
                File.seekp(parent.address_of_children[k]);
                child.write_to_file(File);
            } else {
                int left_size = total_size / 2;
                int right_size = total_size - left_size;
                int delta = left_size - child.size;
                if (delta > 0) {
                    for (int i = child.size; i < left_size; i++) {
                        child.value[i] = right_child.value[i - child.size];
                        child.key[i] = right_child.key[i - child.size];
                    }
                    parent.key[k] = right_child.key[delta];
                    for (int i = 0; i < right_size; i++) {
                        right_child.key[i] = right_child.key[i + delta];
                        right_child.value[i] = right_child.value[i + delta];
                    }
                } else {
                    delta = -delta;
                    for (int i = right_child.size - 1; i >= 0; i--) {
                        right_child.key[i + delta] = right_child.key[i];
                        right_child.value[i + delta] = right_child.value[i];
                    }
                    for (int i = 0; i < delta; i++) {
                        right_child.key[i] = child.key[left_size + i];
                        right_child.value[i] = child.value[left_size + i];
                    }
                    parent.key[k] = right_child.key[0];
                }
                child.size = left_size;
                right_child.size = right_size;
                File.seekp(address);
                parent.write_to_file(File);
                File.seekp(parent.address_of_children[k]);
                child.write_to_file(File);
                File.seekg(parent.address_of_children[k + 1]);
                right_child.write_to_file(File);
                return;
            }
        } else {
            Node<T, M, L> child;
            File.seekg(parent.address_of_children[k]);
            child.read_from_file(File);
            Node<T, M, L> right_child;
            File.seekg(parent.address_of_children[k + 1]);
            right_child.read_from_file(File);
            int total_size = right_child.size + child.size;
            if (total_size < M - 1) {
                child.key[child.size] = parent.key[k];
                for (int i = 0; i < right_child.size; i++) {
                    child.key[child.size + i + 1] = right_child.key[i];
                    child.address_of_children[child.size + i + 1] = right_child.address_of_children[i];
                }
                child.address_of_children[child.size + right_child.size + 1] = right_child.address_of_children[right_child.size];
                child.size += right_child.size + 1;
                for (int i = 0; i <= right_child.size; i++) {
                    if (right_child.is_leaf == 1) {
                        File.seekg(right_child.address_of_children[i]);
                        leaf_Node<T, V, M, L> tmp;
                        tmp.read_from_file(File);
                        tmp.address_of_parent = parent.address_of_children[k];
                        File.seekp(right_child.address_of_children[i]);
                        tmp.write_to_file(File);
                    } else {
                        File.seekg(right_child.address_of_children[i]);
                        Node<T, M, L> tmp;
                        tmp.read_from_file(File);
                        tmp.address_of_parent = parent.address_of_children[k];
                        File.seekp(right_child.address_of_children[i]);
                        tmp.write_to_file(File);
                    }
                }
                for (int i = k; i < parent.size - 1; i++) {
                    parent.key[i] = parent.key[i + 1];
                    parent.address_of_children[i + 1] = parent.address_of_children[i + 2];
                }
                parent.size--;
                File.seekp(address);
                parent.write_to_file(File);
                File.seekp(parent.address_of_children[k]);
                child.write_to_file(File);
            } else {
                int left_size = total_size / 2;
                int right_size = total_size - left_size;
                int delta = left_size - child.size;
                if (delta > 0) {
                    child.key[child.size] = parent.key[k];
                    for (int i = child.size + 1; i < left_size; i++) {
                        child.key[i] = right_child.key[i - child.size - 1];
                        child.address_of_children[i] = right_child.address_of_children[i - child.size - 1];
                    }
                    child.address_of_children[left_size] = right_child.address_of_children[left_size - child.size - 1];
                    for (int i = child.size + 1; i <= left_size; i++) {
                        if (child.is_leaf == 1) {
                            File.seekg(child.address_of_children[i]);
                            leaf_Node<T, V, M, L> tmp;
                            tmp.read_from_file(File);
                            tmp.address_of_parent = parent.address_of_children[k];
                            File.seekp(child.address_of_children[i]);
                            tmp.write_to_file(File);
                        } else {
                            File.seekg(child.address_of_children[i]);
                            Node<T, M, L> tmp;
                            tmp.read_from_file(File);
                            tmp.address_of_parent = parent.address_of_children[k];
                            File.seekp(child.address_of_children[i]);
                            tmp.write_to_file(File);
                        }
                    }
                    parent.key[k] = right_child.key[delta - 1];
                    for (int i = 0; i < right_size; i++) {
                        right_child.key[i] = right_child.key[i + delta];
                        right_child.address_of_children[i] = right_child.address_of_children[i + delta];
                    }
                    right_child.address_of_children[right_size] = right_child.address_of_children[right_size + delta];
                } else {
                    delta = -delta;
                    for (int i = right_child.size - 1; i >= 0; i--) {
                        right_child.key[i + delta] = right_child.key[i];
                        right_child.address_of_children[i + delta + 1] = right_child.address_of_children[i + 1];
                    }
                    right_child.address_of_children[delta] = right_child.address_of_children[0];
                    for (int i = 0; i < delta - 1; i++) {
                        right_child.key[i] = child.key[left_size + i + 1];
                        right_child.address_of_children[i] = child.address_of_children[left_size + i + 1];
                    }
                    right_child.key[delta - 1] = parent.key[k];
                    right_child.address_of_children[delta - 1] = child.address_of_children[left_size + delta];
                    for (int i = 0; i < delta; i++) {
                        if (right_child.is_leaf == 1) {
                            File.seekg(right_child.address_of_children[i]);
                            leaf_Node<T, V, M, L> tmp;
                            tmp.read_from_file(File);
                            tmp.address_of_parent = parent.address_of_children[k + 1];
                            File.seekp(right_child.address_of_children[i]);
                            tmp.write_to_file(File);
                        } else {
                            File.seekg(right_child.address_of_children[i]);
                            Node<T, M, L> tmp;
                            tmp.read_from_file(File);
                            tmp.address_of_parent = parent.address_of_children[k + 1];
                            File.seekp(right_child.address_of_children[i]);
                            tmp.write_to_file(File);
                        }
                    }
                    parent.key[k] = child.key[left_size];
                }
                child.size = left_size;
                right_child.size = right_size;
                File.seekp(address);
                parent.write_to_file(File);
                File.seekp(parent.address_of_children[k]);
                child.write_to_file(File);
                File.seekg(parent.address_of_children[k + 1]);
                right_child.write_to_file(File);
                return;
            }
        }
        if (parent.size < (M - 1) / 2 && address != address_of_root && parent.address_of_parent != -1) {
            Node<T, M, L> parent_parent;
            File.seekg(parent.address_of_parent);
            parent_parent.read_from_file(File);
            int index = -1;
            for (int i = 0; i <= parent_parent.size; i++) {
                if (parent_parent.address_of_children[i] == address) {
                    index = i;
                    break;
                }
            }
            long address_parent = parent.address_of_parent;
            merge_node(address_parent, index);
        } else if (parent.size == 0 && address == address_of_root) {
            Node<T, M, L> root;
            File.seekg(address_of_root);
            root.read_from_file(File);
            if (root.is_leaf == 1) {
                return;
            }
            address_of_root = parent.address_of_children[0];
            if (address_of_root == -1) {
                return;
            }
            File.seekg(address_of_root);
            Node<T, M, L> tmp;
            tmp.read_from_file(File);
            tmp.address_of_parent = -1;
            File.seekp(address_of_root);
            tmp.write_to_file(File);
            return;
        }
    }
    bool delete_the_node(long address, const T& value) {
        File.seekg(address);
        leaf_Node<T, V, M, L> node;
        node.read_from_file(File);
        int index = binary_find_value(node, value);
        if (index == -1 || !(node.key[index] == value)) {
            return false;
        }
        for (int i = index; i < node.size - 1; i++) {
            node.key[i] = node.key[i + 1];
            node.value[i] = node.value[i + 1];
        }
        node.size--;
        File.seekp(address);
        node.write_to_file(File);
        if (node.size < (L - 1) / 2) {
            Node<T, M, L> parent_parent;
            File.seekg(node.address_of_parent);
            parent_parent.read_from_file(File);
            int index = -1;
            for (int i = 0; i <= parent_parent.size; i++) {
                if (parent_parent.address_of_children[i] == address) {
                    index = i;
                    break;
                }
            }
            long address_parent = node.address_of_parent;
            merge_node(address_parent, index);
        }
        return true;
    }
    int size_ = 0;
public:
    B_plus_tree() {}
    B_plus_tree(std::string str) : file_name(str), address_of_root(0) {
        File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!File) {
            File.clear();
            File.open(file_name, std::ios::out | std::ios::binary);
            File.close();
            File.clear();
            File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            Node<T, M, L> initial;
            initial.is_leaf = 1;
            leaf_Node<T, V, M, L> initial_leaf;
            File.seekp(0);
            initial.write_to_file(File);
            initial.address_of_children[0] = File.tellp();
            File.seekp(0);
            initial.write_to_file(File);
            initial_leaf.address_of_parent = 0;
            File.seekp(initial.address_of_children[0]);
            initial_leaf.write_to_file(File);
        } else {
            long address_before;
            int size_before;
            File.seekg(0, std::ios::end);
            long final_pos = File.tellg();
            File.seekg(final_pos - sizeof(long) - sizeof(int));
            File.read(reinterpret_cast<char*> (&address_before), sizeof(long));
            File.seekg(final_pos - sizeof(int));
            File.read(reinterpret_cast<char*> (&size_before), sizeof(int));
            address_of_root = address_before;
            size_ = size_before;
        }
    }
    void set_file_name(std::string str) {
        address_of_root = 0;
        file_name = str;
        File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!File) {
            File.clear();
            File.open(file_name, std::ios::out | std::ios::binary);
            File.close();
            File.clear();
            File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            Node<T, M, L> initial;
            initial.is_leaf = 1;
            leaf_Node<T, V, M, L> initial_leaf;
            File.seekp(0);
            initial.write_to_file(File);
            initial.address_of_children[0] = File.tellp();
            File.seekp(0);
            initial.write_to_file(File);
            initial_leaf.address_of_parent = 0;
            File.seekp(initial.address_of_children[0]);
            initial_leaf.write_to_file(File);
        } else {
            long address_before;
            int size_before;
            File.seekg(0, std::ios::end);
            long final_pos = File.tellg();
            File.seekg(final_pos - sizeof(long) - sizeof(int));
            File.read(reinterpret_cast<char*> (&address_before), sizeof(long));
            File.seekg(final_pos - sizeof(int));
            File.read(reinterpret_cast<char*> (&size_before), sizeof(int));
            address_of_root = address_before;
            size_ = size_before;
        }
    }
    bool empty() {
        File.seekg(address_of_root);
        Node<T, M, L> node;
        node.read_from_file(File);
        if (node.is_leaf == 1 && node.size == 0) {
            leaf_Node<T, V, M, L> leaf;
            File.seekg(node.address_of_children[0]);
            leaf.read_from_file(File);
            return (leaf.size == 0) && (size_ == 0);
        } else {
            return false;
        }
    }
    ~B_plus_tree() {
        put_root();
    }
    bool insert(const T& key, const V& value) {
        long address = find_corresponding_leaf(key);
        if (insert_to_bpt(address, key, value)) {
            size_++;
            return true;
        }
        return false;
    }
    bool remove(const T& value) {
        long address = find_corresponding_leaf(value);
        if (delete_the_node(address, value)) {
            size_--;
            return true;
        }
        return false;
    }
    sjtu::vector<std::pair<T, V>> find(const T& minimal, const T& maximal) {
        sjtu::vector<std::pair<T, V>> values;
        long address = find_corresponding_leaf(minimal);
        leaf_Node<T, V, M, L> node;
        File.seekg(address);
        node.read_from_file(File);
        int index = 0;
        while (index < node.size && node.key[index] < minimal) {index++;}
        while (true) {
            while (index < node.size && (node.key[index] < maximal || node.key[index] == maximal)) {
                values.push_back({node.key[index], node.value[index]});
                index++;
            }
            if (node.address_of_right_node == -1 || node.key[node.size - 1] > maximal) {
                break;
            }
            File.seekg(node.address_of_right_node);
            node.read_from_file(File);
            index = 0;
        }
        return values;
    }
    void put_root() {
        File.seekp(0, std::ios::end);
        File.write(reinterpret_cast<char*> (&address_of_root), sizeof(long));
        File.seekp(0, std::ios::end);
        File.write(reinterpret_cast<char*> (&size_), sizeof(int));
        return;
    }
    int size() {return size_;}
    void clear() {
        size_ = 0;
        address_of_root = 0;
        File.clear();
        File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        Node<T, M, L> initial;
        initial.is_leaf = 1;
        leaf_Node<T, V, M, L> initial_leaf;
        File.seekp(0);
        initial.write_to_file(File);
        initial.address_of_children[0] = File.tellp();
        File.seekp(0);
        initial.write_to_file(File);
        initial_leaf.address_of_parent = 0;
        initial_leaf.write_to_file(File);
    }
};

#endif    //B_PLUS_TREE