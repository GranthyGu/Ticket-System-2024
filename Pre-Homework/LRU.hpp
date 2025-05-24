// LRU.hpp
#ifndef LRC_HPP
#define LRC_HPP

#include "vector/unordered_map.hpp"
#include "vector/list.hpp"
#include <utility>
#include <cstddef>

template<typename Key, typename Value>
class LRU {
private:
    using Node = std::pair<Key, Value>;
    using ListIterator = typename sjtu::list<Node>::iterator;
    std::size_t capacity;
    sjtu::list<Node> lru_list;
    sjtu::unordered_map<Key, ListIterator> cache;
public:
    explicit LRU(std::size_t cap = 500) : capacity(cap) {}
    bool contains(const Key& key) const {
        return cache.find(key) != nullptr;
    }
    bool get(const Key& key, Value& value) {
        ListIterator* it = cache.find(key);
        if (it == nullptr) return false;
        lru_list.splice(lru_list.begin(), *it);
        value = (*it)->second;
        return true;
    }
    void put(const Key& key, const Value& value) {
        ListIterator* it = cache.find(key);
        if (it != nullptr) {
            auto list_it = *it;
            list_it->second = value; 
            lru_list.splice(lru_list.begin(), *it);
        } else {
            if (lru_list.size() == capacity) {
                auto last = lru_list.back();
                cache.erase(last.first);
                lru_list.pop_back();
            }
            Node temp = {key, value};
            lru_list.push_front(temp);
            cache.insert(key, lru_list.begin());
        }
    }
    bool erase(const Key& key) {
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