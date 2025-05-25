#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
#include "LRU.hpp"

template<typename T, typename V, const int M, const int L>
class B_plus_tree {
private:
    class Node;
    class leaf_Node;
    std::string file_name;
    std::fstream File;
    long address_of_root = 10;
    LRU<long, Node> LRU_node;
    LRU<long, leaf_Node> LRU_leaf_node;
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
        Node& operator=(const Node& other) {
            this->is_leaf = other.is_leaf;
            this->size = other.size;
            this->address_of_parent = other.address_of_parent;
            for (int i = 0; i <= M; i++) {
                this->key[i] = other.key[i];
                this->address_of_children[i] = other.address_of_children[i];
            }
            return *this;
        }
    };
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
        leaf_Node& operator=(const leaf_Node& other) {
            this->size = other.size;
            this->address_of_right_node = other.address_of_right_node;
            this->address_of_parent = other.address_of_parent;
            for (int i = 0; i < L; i++) {
                this->key[i] = other.key[i];
                this->value[i] = other.value[i];
            }
            return *this;
        }
    };
    void write_to_file1(const long pos, Node& node) {
        // if (LRU_node.get(pos, node)) {
        //     return;
        // }
        File.seekp(pos);
        if (!File) {return;}
        File.write(reinterpret_cast<char*> (&node), sizeof(Node));
        LRU_node.put(pos, node);
    }
    void read_from_file1(const long pos, Node& node) {
        if (LRU_node.get_(pos, node)) {
            return;
        }
        File.seekg(pos);
        if (!File) {return;}
        File.read(reinterpret_cast<char*> (&node), sizeof(Node));
        LRU_node.put(pos, node);
    }
    void write_to_file2(const long pos, leaf_Node& node) {
        // if (LRU_leaf_node.get(pos, node)) {
        //     return;
        // }
        File.seekp(pos);
        if (!File) {return;}
        File.write(reinterpret_cast<char*> (&node), sizeof(leaf_Node));
        LRU_leaf_node.put(pos, node);
    }
    void read_from_file2(const long pos, leaf_Node& node) {
        if (LRU_leaf_node.get_(pos, node)) {
            return;
        }
        File.seekg(pos);
        if (!File) {return;}
        File.read(reinterpret_cast<char*> (&node), sizeof(leaf_Node));
        LRU_leaf_node.put(pos, node);
    }
    // To find the index in node.key, which is the first element less than target. If not exist, return -1.
    int binary_find(Node node, T target) {
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
    int binary_find_value(leaf_Node node, T target) {
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
            leaf_Node parent;
            read_from_file2(address, parent);
            Node parent_parent;
            read_from_file1(parent.address_of_parent, parent_parent);
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
            Node parent;
            read_from_file1(address, parent);
            if (parent.address_of_parent == -1) {   // Which means that the parent node is the root.
                Node new_root;
                new_root.address_of_children[0] = address;
                new_root.address_of_parent = -1;
                File.seekp(0, std::ios::end);
                long root_address = File.tellp();
                write_to_file1(root_address, new_root);
                address_of_root = root_address;
                File.seekp(0);
                File.write(reinterpret_cast<char*> (&address_of_root), sizeof(long));
                parent.address_of_parent = root_address;
                write_to_file1(address, parent);
                split_node(root_address, 0);
            } else {
                Node parent_parent;
                read_from_file1(parent.address_of_parent, parent_parent);
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
        Node parent;
        read_from_file1(address, parent);
        if (parent.is_leaf == 1) {
            leaf_Node child;
            read_from_file2(parent.address_of_children[k], child);
            leaf_Node new_node;
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
            write_to_file2(address_last, new_node);
            write_to_file2(parent.address_of_children[k], child);
            for (int i = parent.size; i > k; i--) {
                parent.key[i] = parent.key[i - 1];
                parent.address_of_children[i + 1] = parent.address_of_children[i];
            }
            parent.key[k] = child.key[child.size];
            parent.address_of_children[k + 1] = address_last;
        } else {
            Node child;
            read_from_file1(parent.address_of_children[k], child);
            Node new_node;
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
            write_to_file1(address_last, new_node);
            write_to_file1(parent.address_of_children[k], child);
            for (int i = 0; i <= new_node.size; i++) {
                if (new_node.is_leaf == 1) {
                    leaf_Node tmp;
                    read_from_file2(new_node.address_of_children[i], tmp);
                    tmp.address_of_parent = address_last;
                    write_to_file2(new_node.address_of_children[i], tmp);
                } else {
                    Node tmp;
                    read_from_file1(new_node.address_of_children[i], tmp);
                    tmp.address_of_parent = address_last;
                    write_to_file1(new_node.address_of_children[i], tmp);
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
        write_to_file1(address, parent);
        if (parent.size > M - 1) {
            recall_for_insert(address, false);
        }
        return;
    }
    // Insert the certain value to the certain node(found by the address of itself).
    bool insert_to_bpt(long address, const T& k, const V& v) {
        leaf_Node node;
        read_from_file2(address, node);
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
        write_to_file2(address, node);
        if (node.size >= L) {
            recall_for_insert(address, true);
        }
        return true;
    }
    // For certain T, find its corresponding leaf. Begin the finding from the root, stop at the leaf.
    long find_corresponding_leaf(const T& value) {
        Node node;
        long address = address_of_root;
        read_from_file1(address_of_root, node);
        bool c = true;
        while (true) {
            int index = binary_find(node, value);
            address = node.address_of_children[index + 1];
            if (node.is_leaf == 0) {
                read_from_file1(node.address_of_children[index + 1], node);
            } else {
                break;
            }
        }
        return address;
    }
    // Usually, we will merge the node with the right one. However, for the last node, merge it with the left one.
    void merge_node(long address, int k) {
        Node parent;
        read_from_file1(address, parent);
        if (k == parent.size) {k--;}
        if (k < 0) {return;}
        if (parent.is_leaf == 1) {
            leaf_Node child;
            read_from_file2(parent.address_of_children[k], child);
            leaf_Node right_child;
            read_from_file2(parent.address_of_children[k + 1], right_child);
            long ad = parent.address_of_children[k + 1];
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
                write_to_file1(address, parent);
                write_to_file2(parent.address_of_children[k], child);
                LRU_leaf_node.erase(ad);
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
                write_to_file1(address, parent);
                write_to_file2(parent.address_of_children[k], child);
                write_to_file2(parent.address_of_children[k + 1], right_child);
                return;
            }
        } else {
            Node child;
            read_from_file1(parent.address_of_children[k], child);
            Node right_child;
            read_from_file1(parent.address_of_children[k + 1], right_child);
            long ad = parent.address_of_children[k + 1];
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
                        leaf_Node tmp;
                        read_from_file2(right_child.address_of_children[i], tmp);
                        tmp.address_of_parent = parent.address_of_children[k];
                        write_to_file2(right_child.address_of_children[i], tmp);
                    } else {
                        Node tmp;
                        read_from_file1(right_child.address_of_children[i], tmp);
                        tmp.address_of_parent = parent.address_of_children[k];
                        write_to_file1(right_child.address_of_children[i], tmp);
                    }
                }
                for (int i = k; i < parent.size - 1; i++) {
                    parent.key[i] = parent.key[i + 1];
                    parent.address_of_children[i + 1] = parent.address_of_children[i + 2];
                }
                parent.size--;
                write_to_file1(address, parent);
                write_to_file1(parent.address_of_children[k], child);
                LRU_node.erase(ad);
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
                            leaf_Node tmp;
                            read_from_file2(child.address_of_children[i], tmp);
                            tmp.address_of_parent = parent.address_of_children[k];
                            write_to_file2(child.address_of_children[i], tmp);
                        } else {
                            Node tmp;
                            read_from_file1(child.address_of_children[i], tmp);
                            tmp.address_of_parent = parent.address_of_children[k];
                            write_to_file1(child.address_of_children[i], tmp);
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
                            leaf_Node tmp;
                            read_from_file2(right_child.address_of_children[i], tmp);
                            tmp.address_of_parent = parent.address_of_children[k + 1];
                            write_to_file2(right_child.address_of_children[i], tmp);
                        } else {
                            Node tmp;
                            read_from_file1(right_child.address_of_children[i], tmp);
                            tmp.address_of_parent = parent.address_of_children[k + 1];
                            write_to_file1(right_child.address_of_children[i], tmp);
                        }
                    }
                    parent.key[k] = child.key[left_size];
                }
                child.size = left_size;
                right_child.size = right_size;
                write_to_file1(address, parent);
                write_to_file1(parent.address_of_children[k], child);
                write_to_file1(parent.address_of_children[k + 1], right_child);
                return;
            }
        }
        if (parent.size < (M - 1) / 2 && address != address_of_root) {
            Node parent_parent;
            read_from_file1(parent.address_of_parent, parent_parent);
            int index;
            for (int i = 0; i <= parent_parent.size; i++) {
                if (parent_parent.address_of_children[i] == address) {
                    index = i;
                    break;
                }
            }
            long address_parent = parent.address_of_parent;
            merge_node(address_parent, index);
        } else if (parent.size == 0 && address == address_of_root) {
            Node root;
            read_from_file1(address_of_root, root);
            if (root.is_leaf == 1) {
                return;
            }
            address_of_root = parent.address_of_children[0];
            File.seekp(0);
            File.write(reinterpret_cast<char*> (&address_of_root), sizeof(long));
            if (address_of_root == -1) {
                return;
            }
            Node tmp;
            read_from_file1(address_of_root, tmp);
            tmp.address_of_parent = -1;
            write_to_file1(address_of_root, tmp);
            return;
        }
    }
    bool delete_the_node(long address, const T& value) {
        leaf_Node node;
        read_from_file2(address, node);
        int index = binary_find_value(node, value);
        if (index == -1 || !(node.key[index] == value)) {
            return false;
        }
        for (int i = index; i < node.size - 1; i++) {
            node.key[i] = node.key[i + 1];
            node.value[i] = node.value[i + 1];
        }
        node.size--;
        write_to_file2(address, node);
        if (node.size < (L - 1) / 2) {
            Node parent_parent;
            read_from_file1(node.address_of_parent, parent_parent);
            int index;
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
public:
    B_plus_tree(std::string str) : file_name(str), address_of_root(10) {
        File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!File) {
            File.clear();
            File.open(file_name, std::ios::out | std::ios::binary);
            File.close();
            File.clear();
            File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            // LRU_node.set_file(str);
            // LRU_leaf_node.set_file(str);
            LRU_node.clear();
            LRU_leaf_node.clear();
            File.seekp(0);
            File.write(reinterpret_cast<char*> (&address_of_root), sizeof(long));
            Node initial;
            initial.is_leaf = 1;
            leaf_Node initial_leaf;
            write_to_file1(10, initial);
            initial.address_of_children[0] = File.tellp();
            write_to_file1(10, initial);
            initial_leaf.address_of_parent = 10;
            write_to_file2(initial.address_of_children[0], initial_leaf);
        } else {
            // LRU_node.set_file(str);
            // LRU_leaf_node.set_file(str);
            LRU_node.clear();
            LRU_leaf_node.clear();
            long address_before;
            File.seekg(0);
            File.read(reinterpret_cast<char*> (&address_before), sizeof(long));
            address_of_root = address_before;
        }
    }
    bool insert(const T& key, const V& value) {
        long address = find_corresponding_leaf(key);
        return insert_to_bpt(address, key, value);
    }
    bool remove(const T& value) {
        long address = find_corresponding_leaf(value);
        return delete_the_node(address, value);
    }
    std::vector<std::pair<T, V>> find(const T& minimal, const T& maximal) {
        std::vector<std::pair<T, V>> values;
        long address = find_corresponding_leaf(minimal);
        leaf_Node node;
        read_from_file2(address, node);
        int index = 0;
        while (index < node.size && node.key[index] < minimal) {index++;}
        int i = 0;
        while (true) {
            while (index < node.size && (node.key[index] < maximal || node.key[index] == maximal)) {
                values.push_back({node.key[index], '0'});
                index++;
            }
            if (node.address_of_right_node == -1 || node.key[node.size - 1] > maximal) {
                break;
            }
            read_from_file2(node.address_of_right_node, node);
            index = 0;
        }
        return values;
    }
    void put_root() {
        // LRU_node.put_info();
        // LRU_leaf_node.put_info();
        LRU_node.clear();
        LRU_leaf_node.clear();
        File.seekp(0);
        File.write(reinterpret_cast<char*> (&address_of_root), sizeof(long));
        return;
    }
};
class key_value {
public:
    char key[65] = {};
    int value;
public:
    key_value() : value(INT_MIN) {}
    key_value(char* key_, int value_) : value(value_) {
        for (int i = 0; i < 65; i++) {
            key[i] = key_[i];
        }
    }
    key_value(std::string key_, int value_) : value(value_) {
        for (int i = 0; i < key_.length(); i++) {
            key[i] = key_[i];
        }
    }
    key_value operator=(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            key[i] = other.key[i];
        }
        value = other.value;
        return *this;
    }
    bool operator<(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return (key[i] < other.key[i]);
            }
        }
        return (value < other.value);
    }
    bool operator>(const key_value& other) {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return (key[i] > other.key[i]);
            }
        }
        return (value > other.value);
    }
    bool operator==(const key_value& other) const {
        for (int i = 0; i < 65; i++) {
            if (key[i] != other.key[i]) {
                return false;
            }
        }
        return (value == other.value);
    }
};
int main() {
    int n;
    std::cin >> n;
    B_plus_tree<key_value, char, 3, 3> bpt("File_for_bpt");
    for (int i = 0; i < n; i++) {
        std::string operation;
        std::cin >> operation;
        if (operation == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            key_value tmp(key, value);
            bpt.insert(tmp, '0');
        }
        if (operation == "delete") {
            std::string key;
            int value;
            std::cin >> key >> value;
            key_value tmp(key, value);
            bpt.remove(tmp);
        }
        if (operation == "find") {
            std::string key;
            std::cin >> key;
            key_value minimal(key, INT_MIN);
            key_value maximal(key, INT_MAX);
            std::vector<std::pair<key_value, char>> tmp = bpt.find(minimal, maximal);
            if (tmp.size() == 0) {
                std::cout << "null" << std::endl;
            } else {
                for (int i = 0; i < tmp.size(); i++) {
                    std::cout << tmp[i].first.value << ' ';
                }
                std::cout << std::endl;
            }
        }
    }
    bpt.put_root();
}