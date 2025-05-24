#ifndef UNORDERED_MAP_HPP
#define UNORDERED_MAP_HPP

#include <iostream>
#include <functional>

namespace sjtu {
template <typename Key, typename Value>
class unordered_map {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };
    Node** buckets;
    size_t bucket_count;
    size_t size_;
    size_t hash_fn(const Key& key) {
        return (size_t)key % bucket_count;
    }
public:
    unordered_map(size_t bucket_count = 20000) 
        : bucket_count(bucket_count), size_(0) {
        buckets = new Node*[bucket_count]();
    }
    ~unordered_map() {
        clear();
        delete[] buckets;
    }
    void insert(const Key& key, const Value& value) {
        size_t index = hash_fn(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        Node* newNode = new Node(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        size_++;
    }
    bool erase(const Key& key) {
        size_t index = hash_fn(key);
        Node* prev = nullptr;
        Node* current = buckets[index];
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    buckets[index] = current->next;
                }
                delete current;
                size_--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    Value* find(const Key& key) {
        size_t index = hash_fn(key);
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }
    size_t size() const {
        return size_;
    }
    void clear() {
        for (size_t i = 0; i < bucket_count; ++i) {
            Node* current = buckets[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        size_ = 0;
    }
    unordered_map(const unordered_map&) = delete;
    unordered_map& operator=(const unordered_map&) = delete;
};
}

#endif     //UNORDERED_MAP_HPP