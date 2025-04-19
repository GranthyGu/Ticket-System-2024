#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
const int M = 63;
const int L = 63;

template<typename T>
class Node {
public:
    int is_leaf;
    int size;
    T key[M + 1] = {};
    long address_of_children[M + 1] = {-1};
    long address_of_parent;
    long address_of_right_node;
    Node() : is_leaf(0), size(0), address_of_right_node(-1), address_of_parent(-1) {}
    Node(int leaf, int size, T* key_, long* add, long add_, long add_parent) : is_leaf(leaf), size(size), address_of_right_node(add_), address_of_parent(add_parent) {
        for (int i = 0; i < M; i++) {
            key[i] = key_[i];
            address_of_children[i] = add[i];
        }
    }
    Node<T>& operator=(const Node<T>& other) {
        this->is_leaf = other.is_leaf;
        this->size = other.size;
        this->address_of_right_node = other.address_of_right_node;
        this->address_of_parent = other.address_of_parent;
        for (int i = 0; i < M; i++) {
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
        File.write(reinterpret_cast<char*> (&address_of_right_node), sizeof(long));
        File.write(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
    void read_from_file(std::fstream& File) {
        if (!File) {return;}
        File.read(reinterpret_cast<char*> (&is_leaf), sizeof(int));
        File.read(reinterpret_cast<char*> (&size), sizeof(int));
        File.read(reinterpret_cast<char*> (key), (M + 1) * sizeof(T));
        File.read(reinterpret_cast<char*> (address_of_children), (M + 1) * sizeof(long));
        File.read(reinterpret_cast<char*> (&address_of_right_node), sizeof(long));
        File.read(reinterpret_cast<char*> (&address_of_parent), sizeof(long));
    }
};
template<typename T>
class B_plus_tree {
private:
    std::string file_name;
    std::fstream File;
    long address_of_root;
    // To find the index in node.key, which is the first element less than target. If not exist, return -1.
    int binary_find(Node<T> node, T target) {
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
    void recall_for_insert(long address) {
        Node<T> parent;
        File.seekg(address);
        parent.read_from_file(File);
        if (parent.address_of_parent == -1) {   // Which means that the parent node is the root.
            Node<T> new_root;
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
            Node<T> parent_parent;
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
    // Split the ith child of the parent node.
    void split_node(long address, int k) {
        File.seekg(address);
        Node<T> parent;
        parent.read_from_file(File);
        Node<T> child;
        File.seekg(parent.address_of_children[k]);
        child.read_from_file(File);
        Node<T> new_node;
        new_node.is_leaf = child.is_leaf;
        new_node.address_of_parent = child.address_of_parent;
        File.seekp(0, std::ios::end);
        long address_last = File.tellp();
        if (child.is_leaf == 1) {
            new_node.address_of_right_node = child.address_of_right_node;
            child.address_of_right_node = address_last;
            new_node.size = child.size / 2;
            child.size = child.size - new_node.size;
            for (int i = 0; i < new_node.size; i++) {
                new_node.key[i] = child.key[i + child.size];
            }
            new_node.write_to_file(File);
            File.seekp(parent.address_of_children[k]);
            child.write_to_file(File);
        } else {
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
                File.seekg(new_node.address_of_children[i]);
                Node<T> tmp;
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
        parent.size++;
        File.seekp(address);
        parent.write_to_file(File);
        if (parent.size > M - 1) {
            recall_for_insert(address);
        }
        return;
    }
    // Insert the certain value to the certain node(found by the address of itself).
    bool insert_to_bpt(long address, T value) {
        File.seekg(address);
        Node<T> node;
        node.read_from_file(File);
        int index = binary_find(node, value);
        if (node.is_leaf == 1) {
            if (index != -1 && node.key[index] == value) {
                return false;
            }
            for (int i = node.size; i > index + 1; i--) {
                node.key[i] = node.key[i - 1];
            }
            node.key[index + 1] = value;
            node.size++;
            File.seekp(address);
            node.write_to_file(File);
            if (node.size >= L) {
                recall_for_insert(address);
            }
            return true;
        } else {
            return false;
        }
    }
    // For certain T, find its corresponding leaf. Begin the finding from the root, stop at the leaf.
    long find_corresponding_leaf(T value) {
        Node<T> node;
        File.seekg(address_of_root);
        long address = address_of_root;
        node.read_from_file(File);
        while (node.is_leaf == 0) {
            int index = binary_find(node, value);
            File.seekg(node.address_of_children[index + 1]);
            address = node.address_of_children[index + 1];
            node.read_from_file(File);
        }
        return address;
    }
    // Usually, we will merge the node with the right one. However, for the last node, merge it with the left one.
    void merge_node(long address, int k) {
        File.seekg(address);
        Node<T> parent;
        parent.read_from_file(File);
        if (k == parent.size) {k--;}
        if (k < 0) {return;}
        Node<T> child;
        File.seekg(parent.address_of_children[k]);
        child.read_from_file(File);
        Node<T> right_child;
        File.seekg(parent.address_of_children[k + 1]);
        right_child.read_from_file(File);
        int total_size = right_child.size + child.size;
        if (child.is_leaf == 1 && total_size <= L - 1) {
            for (int i = 0; i < right_child.size; i++) {
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
        } else if (child.is_leaf == 0 && total_size < M - 1) {
            child.key[child.size] = parent.key[k];
            for (int i = 0; i < right_child.size; i++) {
                child.key[child.size + i + 1] = right_child.key[i];
                child.address_of_children[child.size + i + 1] = right_child.address_of_children[i];
            }
            child.address_of_children[child.size + right_child.size + 1] = right_child.address_of_children[right_child.size];
            child.size += right_child.size + 1;
            child.address_of_right_node = right_child.address_of_right_node;\
            for (int i = 0; i <= right_child.size; i++) {
                File.seekg(right_child.address_of_children[i]);
                Node<T> tmp;
                tmp.read_from_file(File);
                tmp.address_of_parent = parent.address_of_children[k];
                File.seekp(right_child.address_of_children[i]);
                tmp.write_to_file(File);
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
                if (child.is_leaf == 1) {
                    for (int i = child.size; i < left_size; i++) {
                        child.key[i] = right_child.key[i - child.size];
                    }
                    parent.key[k] = right_child.key[delta];
                    for (int i = 0; i < right_size; i++) {
                        right_child.key[i] = right_child.key[i + delta];
                    }
                } else {
                    child.key[child.size] = parent.key[k];
                    for (int i = child.size + 1; i < left_size; i++) {
                        child.key[i] = right_child.key[i - child.size - 1];
                        child.address_of_children[i] = right_child.address_of_children[i - child.size - 1];
                    }
                    child.address_of_children[left_size] = right_child.address_of_children[left_size - child.size - 1];
                    for (int i = child.size + 1; i <= left_size; i++) {
                        File.seekg(child.address_of_children[i]);
                        Node<T> tmp;
                        tmp.read_from_file(File);
                        tmp.address_of_parent = parent.address_of_children[k];
                        File.seekp(child.address_of_children[i]);
                        tmp.write_to_file(File);
                    }
                    parent.key[k] = right_child.key[delta - 1];
                    for (int i = 0; i < right_size; i++) {
                        right_child.key[i] = right_child.key[i + delta];
                        right_child.address_of_children[i] = right_child.address_of_children[i + delta];
                    }
                    right_child.address_of_children[right_size] = right_child.address_of_children[right_size + delta];
                }
                child.size = left_size;
                right_child.size = right_size;
            } else {
                delta = -delta;
                if (child.is_leaf == 1) {
                    for (int i = right_child.size - 1; i >= 0; i--) {
                        right_child.key[i + delta] = right_child.key[i];
                    }
                    for (int i = 0; i < delta; i++) {
                        right_child.key[i] = child.key[left_size + i];
                    }
                    parent.key[k] = right_child.key[0];
                } else {
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
                        File.seekg(right_child.address_of_children[i]);
                        Node<T> tmp;
                        tmp.read_from_file(File);
                        tmp.address_of_parent = parent.address_of_children[k + 1];
                        File.seekp(right_child.address_of_children[i]);
                        tmp.write_to_file(File);
                    }
                    parent.key[k] = child.key[left_size];
                }
                child.size = left_size;
                right_child.size = right_size;
            }
            File.seekp(address);
            parent.write_to_file(File);
            File.seekp(parent.address_of_children[k]);
            child.write_to_file(File);
            File.seekg(parent.address_of_children[k + 1]);
            right_child.write_to_file(File);
            return;
        }
        if (parent.size < (M - 1) / 2 && address != address_of_root) {
            Node<T> parent_parent;
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
            merge_node(address_parent, index);
        } else if (parent.size == 0 && address == address_of_root) {
            address_of_root = parent.address_of_children[0];
            if (address_of_root == -1) {
                return;
            }
            File.seekg(address_of_root);
            Node<T> tmp;
            tmp.read_from_file(File);
            tmp.address_of_parent = -1;
            File.seekp(address_of_root);
            tmp.write_to_file(File);
            return;
        }
    }
    bool delete_the_node(long address, T value) {
        File.seekg(address);
        Node<T> node;
        node.read_from_file(File);
        int index = binary_find(node, value);
        if (node.is_leaf == 1) {
            if (index == -1 || node.key[index] != value) {
                return false;
            }
            for (int i = index; i < node.size - 1; i++) {
                node.key[i] = node.key[i + 1];
            }
            node.size--;
            File.seekp(address);
            node.write_to_file(File);
            if (node.size < (L - 1) / 2 && address != address_of_root) {
                Node<T> parent_parent;
                File.seekg(node.address_of_parent);
                parent_parent.read_from_file(File);
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
        } else {
            return false;
        }
    }
public:
    B_plus_tree(std::string str) : file_name(str), address_of_root(0) {
        File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!File) {
            File.clear();
            File.open(file_name, std::ios::out | std::ios::binary);
            File.close();
            File.clear();
            File.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            Node<T> initial;
            initial.is_leaf = 1;
            File.seekp(0);
            initial.write_to_file(File);
        } else {
            long address_before;
            File.seekg(0, std::ios::end);
            long final_pos = File.tellg();
            File.seekg(final_pos - sizeof(long));
            File.read(reinterpret_cast<char*> (&address_before), sizeof(long));
            address_of_root = address_before;
        }
    }
    bool insert(T value) {
        long address = find_corresponding_leaf(value);
        return insert_to_bpt(address, value);
    }
    bool remove(T value) {
        long address = find_corresponding_leaf(value);
        return delete_the_node(address, value);
    }
    std::vector<T> find(T minimal, T maximal) {
        std::vector<T> values;
        long address = find_corresponding_leaf(minimal);
        long address_ = find_corresponding_leaf(maximal);
        if (address == address_) {
            Node<T> node;
            File.seekg(address);
            node.read_from_file(File);
            int index1 = binary_find(node, minimal);
            int index2 = binary_find(node, maximal);
            for (int i = index1 + 1; i <= index2; i++) {
                values.push_back(node.key[i]);
            }
            return values;
        } else {
            Node<T> node, node_;
            File.seekg(address);
            node.read_from_file(File);
            File.seekg(address_);
            node_.read_from_file(File);
            int index1 = binary_find(node, minimal);
            int index2 = binary_find(node_, maximal);
            for (int i = index1 + 1; i < node.size; i++) {
                values.push_back(node.key[i]);
            }
            address = node.address_of_right_node;
            while (address != address_) {
                File.seekg(address);
                node.read_from_file(File);
                for (int i = 0; i < node.size; i++) {
                    values.push_back(node.key[i]);
                }
                address = node.address_of_right_node;
            }
            for (int i = 0; i <= index2; i++) {
                values.push_back(node_.key[i]);
            }
            return values;
        }
    }
    void put_root() {
        File.seekp(0, std::ios::end);
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
    B_plus_tree<key_value> bpt("File_for_bpt");
    for (int i = 0; i < n; i++) {
        std::string operation;
        std::cin >> operation;
        if (operation == "insert") {
            std::string key;
            int value;
            std::cin >> key >> value;
            key_value tmp(key, value);
            bpt.insert(tmp);
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
            std::vector<key_value> tmp = bpt.find(minimal, maximal);
            if (tmp.size() == 0) {
                std::cout << "null" << std::endl;
            } else {
                for (int i = 0; i < tmp.size(); i++) {
                    std::cout << tmp[i].value << ' ';
                }
                std::cout << std::endl;
            }
        }
    }
    bpt.put_root();
}