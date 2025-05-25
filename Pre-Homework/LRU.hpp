// LRU.hpp
#ifndef LRC_HPP
#define LRC_HPP

#include "vector/unordered_map.hpp"
#include "vector/list.hpp"
#include <utility>
#include <fstream>
#include <cstddef>

template<typename Value>
class LRU {
private:
    using Node = std::pair<long, Value>;
    using ListIterator = typename sjtu::list<Node>::iterator;
    std::size_t capacity;
    sjtu::list<Node> lru_list;
    sjtu::unordered_map<long, ListIterator> cache;
    std::fstream File;
public:
    explicit LRU(std::size_t cap = 2000) : capacity(cap) {}
    // void set_file(const std::string& str) {File.open(str, std::ios::in | std::ios::out | std::ios::binary);}
    ~LRU() {
        // put_info();
        clear();
    }
    // void put_info() {
    //     while (!lru_list.empty()) {
    //         auto last = lru_list.back();
    //         File.seekp(last.first);
    //         File.write(reinterpret_cast<char*>(&last.second), sizeof(Value));
    //         cache.erase(last.first);
    //         lru_list.pop_back();
    //     }
    // }
    bool contains(const long& key) const {
        return cache.find(key) != nullptr;
    }
    bool get(const long& key, Value& value) {
        ListIterator* it = cache.find(key);
        if (it == nullptr) return false;
        lru_list.splice(lru_list.begin(), *it);
        (*it)->second = value;
        return true;
    }
    bool get_(const long& key, Value& value) {
        ListIterator* it = cache.find(key);
        if (it == nullptr) return false;
        lru_list.splice(lru_list.begin(), *it);
        value = (*it)->second;
        return true;
    }
    void put(const long& key, const Value& value) {
        ListIterator* it = cache.find(key);
        if (it != nullptr) {
            auto list_it = *it;
            list_it->second = value; 
            lru_list.splice(lru_list.begin(), *it);
        } else {
            if (lru_list.size() >= capacity) {
                auto last = lru_list.back();
                // File.seekp(last.first);
                // File.write(reinterpret_cast<char*>(&last.second), sizeof(Value));
                cache.erase(last.first);
                lru_list.pop_back();
            }
            Node temp = {key, value};
            lru_list.push_front(temp);
            cache.insert(key, lru_list.begin());
        }
    }
    bool erase(const long& key) {
        ListIterator* it = cache.find(key);
        if (it == nullptr) return false;
        lru_list.erase(*it);
        cache.erase(key);
        return true;
    }
    void clear() {
        cache.clear();
        lru_list.clear();
    }
    std::size_t size() const {
        return cache.size();
    }
    std::size_t max_size() const {
        return capacity;
    }
};

#endif // LRC_HPP