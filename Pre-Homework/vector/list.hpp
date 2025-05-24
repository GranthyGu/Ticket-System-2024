#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <exception>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
template <typename T> class list {
protected:
  class node {
  public:
    /**
     * add data members and constructors & destructor
     */
    T *data;
    node* prev;
    node* next;
    node() : data(nullptr), prev(nullptr), next(nullptr) {}
    node(const T& data, node* p = nullptr, node* n = nullptr) : data(new T(data)), prev(p), next(n) {}
    ~node() {
      delete data;
    }
  };

protected:
  /**
   * add data members for linked list as protected members
   */
  node* head;
  node* tail;
  size_t length;
  /**
   * insert node cur before node pos
   * return the inserted node cur
   */
  node *insert(node *pos, node *cur) {
    cur->next = pos;
    cur->prev = pos->prev;
    pos->prev->next = cur;
    pos->prev = cur;
    ++length;
    return cur;
  }
  /**
   * remove node pos from list (no need to delete the node)
   * return the removed node pos
   */
  node *erase(node *pos) {
    node *nextNode = pos->next;
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
    --length;
    return nextNode;
  }

public:
  class const_iterator;
  class iterator {
  private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    node* cur;
    friend class list;
  public:
    iterator(node* p = nullptr) : cur(p) {}
    iterator operator++(int) {
      iterator tmp = *this;
      cur = cur->next;
      return tmp;
    }
    iterator &operator++() {
      cur = cur->next;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp = *this;
      cur = cur->prev;
      return tmp;
    }
    iterator &operator--() {
      cur = cur->prev;
      return *this;
    }

    /**
     * TODO *it
     * throw std::exception if iterator is invalid
     */
    T &operator*() const {
      if (!cur->data) {
        throw std::exception();
      }
      return *(cur->data);
    }
    /**
     * TODO it->field
     * throw std::exception if iterator is invalid
     */
    T *operator->() const noexcept {
      if (!cur->data) {
        throw std::exception();
      }
      return (cur->data);
    }
    T* get_data() {return cur->data;}
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      return cur == rhs.cur;
    }
    bool operator==(const const_iterator &rhs) const {
      return cur == rhs.cur;
    }
    
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return cur != rhs.cur;
    }
    bool operator!=(const const_iterator &rhs) const {
      return cur != rhs.cur;
    }
  };

  /**
   * TODO
   * has same function as iterator, just for a const object.
   * should be able to construct from an iterator.
   */
  class const_iterator {
    private:
      const node *cur;
      friend class list;
    public:
      const_iterator(const node *p = nullptr) : cur(p) {}
      const_iterator(const iterator &it) : cur(it.cur) {}
      const_iterator operator++(int) {
        const_iterator tmp = *this;
        cur = cur->next;
        return tmp;
      }
      const_iterator &operator++() {
        cur = cur->next;
        return *this;
      }
      const_iterator operator--(int) {
        const_iterator tmp = *this;
        cur = cur->prev;
        return tmp;
      }
      const_iterator &operator--() {
        cur = cur->prev;
        return *this;
      }
      const T &operator*() const {
        if (!cur->data) {
          throw std::exception();
        }
        return *(cur->data);
      }
      const T *operator->() const noexcept {
        if (!cur->data) {
          throw std::exception();
        }
        return cur->data;
      }
      bool operator==(const const_iterator &rhs) const { 
        return cur == rhs.cur; 
      }
      bool operator!=(const const_iterator &rhs) const {
        return cur != rhs.cur; 
      }
  };

  /**
   * TODO Constructs
   * Atleast two: default constructor, copy constructor
   */
  list() : length(0) {
    head = new node();
    tail = new node();
    head->next = tail;
    tail->prev = head;
  }
  list(const list &other) : list() {
    for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
      push_back(*(cur->data));
    }
  }
  /**
   * TODO Destructor
   */
  ~list() {
    clear();
    delete head;
    delete tail;
  }
  /**
   * TODO Assignment operator
   */
  list &operator=(const list &other) {
    if (this != &other) {
      clear();
      for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
        push_back(*(cur->data));
      }
    }
    return *this;
  }
  /**
   * access the first / last element
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
    if (empty()) {
      throw std::exception();
    }
    return *(head->next->data);
  }
  const T &back() const {
    if (empty()) {
      throw std::exception();
    }
    return *(tail->prev->data);
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() {
    return iterator(head->next);
  }
  const_iterator cbegin() const {
    return const_iterator(head->next);
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() {
    return iterator(tail);
  }
  const_iterator cend() const {
    return const_iterator(tail);
  }
  /**
   * checks whether the container is empty.
   */
  virtual bool empty() const {
    return length == 0;
  }
  /**
   * returns the number of elements
   */
  virtual size_t size() const {
    return length;
  }

  /**
   * clears the contents
   */
  virtual void clear() {
    while (!empty()) {
      pop_back();
    }
  }
  /**
   * insert value before pos (pos may be the end() iterator)
   * return an iterator pointing to the inserted value
   * throw if the iterator is invalid
   */
  virtual iterator insert(iterator pos, const T &value) {
    if (pos.cur == head || pos.cur == nullptr) {
      throw std::exception();
    }
    return iterator(insert(pos.cur, new node(value)));
  }
  /**
   * remove the element at pos (the end() iterator is invalid)
   * returns an iterator pointing to the following element, if pos pointing to
   * the last element, end() will be returned. throw if the container is empty,
   * the iterator is invalid
   */
  virtual iterator erase(iterator pos) {
    if (empty() || pos.cur == tail) {
      throw std::exception();
    }
    return iterator(erase(pos.cur));
  }
  /**
   * adds an element to the end
   */
  void push_back(const T &value) {
    insert(iterator(tail), value);
  }
  /**
   * removes the last element
   * throw when the container is empty.
   */
  void pop_back() {
    if (empty()) {
      throw std::exception();
    }
    erase(--end());
  }
  /**
   * inserts an element to the beginning.
   */
  void push_front(const T &value) {
    insert(iterator(head->next), value);
  }
  /**
   * removes the first element.
   * throw when the container is empty.
   */
  void pop_front() {
    if (empty()) {
      throw std::exception();
    }
    erase(begin());
  }
  void splice(iterator pos, iterator it) {
    if (pos.cur == nullptr || it.cur == nullptr || it.cur == head || pos.cur == head) {
      return;
    }
    if (pos == it) return;
    node* n = it.cur;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->next = pos.cur;
    n->prev = pos.cur->prev;
    pos.cur->prev->next = n;
    pos.cur->prev = n;
  }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
