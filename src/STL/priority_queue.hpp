#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <iostream>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
    struct Node {
        Node* left;
        Node* right;
        T* data_;
        int npl;
        Node(const T& data, Node* lft = nullptr, Node* rht = nullptr) : left(lft), right(rht), npl(0) {
            data_ = new T (data);
        }
        ~Node() {
            delete data_;
            left = nullptr;
            right = nullptr;
        }
    };
    Node* head;
    size_t size_;
    Compare cmp;
    Node* copy_the_tree(Node* other) {
        Node* tmp = new Node(*(other->data_));
        tmp->npl = other->npl;
        if (other->left != nullptr) {
            tmp->left = copy_the_tree(other->left);
        }
        if (other->right != nullptr) {
            tmp->right = copy_the_tree(other->right);
        }
        return tmp;
    }
    void clear_the_tree(Node* node) {
        if (node == nullptr) {
            return;
        }
        Node* tmp_left = node->left;
        Node* tmp_right = node->right;
        delete node;
        clear_the_tree(tmp_left);
        clear_the_tree(tmp_right);
        return;
    }
    Node* merge_the_tree(Node* tree1, Node* tree2) {
        if (tree1 == nullptr) {
            return tree2;
        }
        if (tree2 == nullptr) {
            return tree1;
        }
        if (cmp(*(tree1->data_), *(tree2->data_))) {
            Node* tmp = tree1;
            tree1 = tree2;
            tree2 = tmp;
        }
        tree1->right = merge_the_tree(tree1->right, tree2);
        if (tree1->right == nullptr || tree1->left == nullptr) {
            tree1->npl = 0;
        } else {
            tree1->npl = std::min(tree1->right->npl, tree1->left->npl) + 1;
        }
        if (tree1->left == nullptr || tree1->left->npl < tree1->right->npl) {
            Node* tmp = tree1->left;
            tree1->left = tree1->right;
            tree1->right = tmp;
        }
        return tree1;  
        }
public:
    /**
     *  * TODO constructors
     */
    priority_queue() : head(nullptr), size_(0), cmp(Compare()) {}
    priority_queue(const priority_queue &other) : size_(other.size_), cmp(other.cmp) {
        head = copy_the_tree(other.head);
    }
    /**
     * TODO deconstructor
     */
    ~priority_queue() {
        clear_the_tree(head);
        head = nullptr;
        size_ = 0;
    }
    /**
     * TODO Assignment operator
     */
    priority_queue &operator=(const priority_queue &other) {
        if (this == &other) {
            return *this;
        }
        if (size_ != 0) {
            clear_the_tree(head);
        }
        size_ = other.size_;
        cmp = other.cmp;
        head = copy_the_tree(other.head);
        return *this;
    }
    /**
     * get the top of the queue.
     * @return a reference of the top element.
     * throw container_is_empty if empty() returns true;
     */
    const T & top() const {
        if (size_ == 0) {
            throw container_is_empty();		
        }
        return *(head->data_);
    }
    /**
     * TODO
     * push new element to the priority queue.
	 */
    void push(const T &e) {
        Node* tmp = new Node(e);
        try {
            head = merge_the_tree(head, tmp);
            size_++;
        }
        catch(...) {
            delete tmp;
            throw;
        }
    }
    /**
     * TODO
     * delete the top element.
     * throw container_is_empty if empty() returns true;
     */
    void pop() {
        if (size_ == 0) {
            throw container_is_empty();
        }
        Node* tmp = head;
        head = merge_the_tree(head->left, head->right);
        size_--;
        delete tmp;
    }
    /**
     * return the number of the elements.
     */
    size_t size() const {
        return size_;
    }
    /**
     * check if the container has at least an element.
     * @return true if it is empty, false if it has at least an element.
     */
    bool empty() const {
        return (size_ == 0);
    }
    /**
     * merge two priority_queues with at most O(logn) complexity.
     * clear the other priority_queue.
     */
    void merge(priority_queue &other) {
        head = merge_the_tree(head, other.head);
        size_ += other.size_;
        other.head = nullptr;
        other.size_ = 0;
    }
};

}

#endif 