/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"
#include "vector.hpp"

namespace sjtu {

template<
    class Key,
    class T,
    class Compare = std::less<Key>
> 
class map {
public:
    /**
     * the internal type of data.
     * it should have a default constructor, a copy constructor.
     * You can use sjtu::map as value_type by typedef.
     */
    typedef pair<const Key, T> value_type;
    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */
    class const_iterator;
    class iterator;

    private:
    struct AVLNode {
        value_type data;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent_;
        int height;
        AVLNode(const value_type& data_, AVLNode* l = nullptr, AVLNode* r = nullptr, AVLNode* parent = nullptr, int bf = 1):
        left(l), right(r), parent_(parent), height(bf), data(data_) {}
    };
    AVLNode* root;
    size_t size_;
	Compare cmp;
    int get_height(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        } else {
            return node->height;
        }
    }
    void rotate_l(AVLNode* node) {
        AVLNode* parent_right = node->right;
        AVLNode* parent_right_left = parent_right->left;
        node->right = parent_right_left;
        if (parent_right_left) {
            parent_right_left->parent_ = node;
        }
        if (node == root) {
            parent_right->parent_ = nullptr;
            root = parent_right;
            parent_right->left = node;
            node->parent_ = parent_right;
        } else {
            AVLNode* parent_parent = node->parent_;
            if (node == parent_parent->left) {
                parent_parent->left = parent_right;
            } else {
                parent_parent->right = parent_right;
            }
            parent_right->parent_ = node->parent_;
            parent_right->left = node;
            node->parent_ = parent_right;
        }
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
        parent_right->height = std::max(get_height(parent_right->left), get_height(parent_right->right)) + 1;
        return;
    }
    void rotate_r(AVLNode* node) {
        AVLNode* parent_left = node->left;
        AVLNode* parent_left_right = parent_left->right;
        node->left = parent_left_right;
        if (parent_left_right) {
            parent_left_right->parent_ = node;
        }
        if (node == root) {
            parent_left->parent_ = nullptr;
            root = parent_left;
            parent_left->right = node;
            node->parent_ = parent_left;
        } else {
            AVLNode* parent_parent = node->parent_;
            if (node == parent_parent->left) {
                parent_parent->left = parent_left;
            } else {
                parent_parent->right = parent_left;
            }
            parent_left->parent_ = node->parent_;
            parent_left->right = node;
            node->parent_ = parent_left;
        }
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
        parent_left->height = std::max(get_height(parent_left->left), get_height(parent_left->right)) + 1;
        return;
    }
    void rotate_lr(AVLNode* node) {
        rotate_l(node->left);
        rotate_r(node);
    }
    void rotate_rl(AVLNode* node) {
        rotate_r(node->right);
        rotate_l(node);
    }
    AVLNode* copy_the_tree(AVLNode* other) {
        if (other == nullptr) {
            return nullptr;
        }
        AVLNode* tmp = new AVLNode(other->data);
        tmp->height = other->height;
        if (other->left) {
            tmp->left = copy_the_tree(other->left);
            tmp->left->parent_ = tmp;
        }
        if (other->right) {
            tmp->right = copy_the_tree(other->right);
            tmp->right->parent_ = tmp;
        }
        return tmp;
    }
    AVLNode* find_the_node(const Key& key) const {
        AVLNode* parent = nullptr;
		AVLNode* node = root;
		while (node) {
			if (cmp(node->data.first, key)) {
				parent = node;
                node = node->right;
			} else if (cmp(key, node->data.first)) {
                parent = node;
                node = node->left;
            } else {
                return node;
            }
		}
        return nullptr;
    }
    bool insert_(value_type value) {
        if (root == nullptr) {
            root = new AVLNode(value);
            size_++;
            return true;
        }
		AVLNode* parent = nullptr;
		AVLNode* node = root;
		while (node) {
			if (cmp(node->data.first, value.first)) {
				parent = node;
                node = node->right;
			} else if (cmp(value.first, node->data.first)) {
                parent = node;
                node = node->left;
            } else {
                return false;
            }
		}
		AVLNode* cur_node = new AVLNode(value);
        if (cmp(parent->data.first, value.first)) {
            parent->right = cur_node;
            cur_node->parent_ = parent;
        } else {
            parent->left = cur_node;
            cur_node->parent_ = parent;
        }
        /**
         * Update the balance factor with rotations.
         */
        while (parent) {
            parent->height = std::max(get_height(parent->left), get_height(parent->right)) + 1;
            if (get_height(parent->left) - get_height(parent->right) == 0) {
                break;
            } else if (get_height(parent->left) - get_height(parent->right) == -1 || get_height(parent->left) - get_height(parent->right) == 1) {
                cur_node = parent;
                parent = parent->parent_;
                continue;
            } else {
                if (get_height(parent->right) - get_height(parent->left) == -2 && get_height(cur_node->right) - get_height(cur_node->left) <= 0) {
                    rotate_r(parent);
                }
                else if (get_height(parent->right) - get_height(parent->left) == -2 && get_height(cur_node->right) - get_height(cur_node->left) == 1) {
                    rotate_lr(parent);
                }
                else if (get_height(parent->right) - get_height(parent->left) == 2 && get_height(cur_node->right) - get_height(cur_node->left) == -1) {
                    rotate_rl(parent);
                }
                else if (get_height(parent->right) - get_height(parent->left) == 2 && get_height(cur_node->right) - get_height(cur_node->left) >= 0) {
                    rotate_l(parent);
                }
            }
        }
        size_++;
        return true;
    }
    void update_height(AVLNode* parent) {
        if (parent == nullptr) {
            return;
        }
        AVLNode* cur;
        while (parent) {
            parent->height = std::max(get_height(parent->left), get_height(parent->right)) + 1;
            /** 
             * We can break the loop iff parent's balance factor = 1/-1.
             */
            if (get_height(parent->right) - get_height(parent->left) == 1 || get_height(parent->right) - get_height(parent->left) == -1) {
                break;
            } else if (get_height(parent->right) - get_height(parent->left) == 0) {
                cur = parent;
                parent = parent->parent_;
                continue;
            } else if (get_height(parent->right) - get_height(parent->left) == 2) {
                /** 
                 * In this case, we need to correct the balance factor after doing rotate_l.
                 * However, after doing this, the tree's height won't influenced. Thus, we can break!
                 */
                if (get_height(parent->right->right) - get_height(parent->right->left) == 0) {
                    AVLNode* parent_right = parent->right;
                    rotate_l(parent);
                    break;
                }
                /**
                 * We don't need to correct the balance factor in this case, as the 
                 * deleted node won't influence the height of the tree. However, the
                 * height of the whole tree reduces. Thus, we need to do the operation
                 * continuously. 
                 */
                else if (get_height(parent->right->right) - get_height(parent->right->left) == 1) {
                    rotate_l(parent);
                }
                /**
                 * Same as the second case.
                 */
                else if (get_height(parent->right->right) - get_height(parent->right->left) == -1) {
                    rotate_rl(parent);
                }
                cur = parent->parent_;
                if (cur) {
                    parent = cur->parent_;
                } else {
                    parent = nullptr;
                }
            } else if (get_height(parent->right) - get_height(parent->left) == -2) {
                if (get_height(parent->left->right) - get_height(parent->left->left) == 0) {
                    AVLNode* parent_left = parent->left;
                    rotate_r(parent);
                    break;
                } else if (get_height(parent->left->right) - get_height(parent->left->left) == -1) {
                    rotate_r(parent);
                } else if (get_height(parent->left->right) - get_height(parent->left->left) == 1) {
                    rotate_lr(parent);
                }
                cur = parent->parent_;
                if (cur) {
                    parent = cur->parent_;
                } else {
                    parent = nullptr;
                }
            }
        }
    }
    bool erase_(const Key& key) {
        if (root == nullptr) {
            return false;
        }
        AVLNode* parent = nullptr;
        AVLNode* cur = root;
        /*
         * In this loop, we will fix the constructure of the AVLtree, without correcting the balance factors.
        */
        cur = find_the_node(key);
        if (cur == nullptr) {
            return false;
        }
        parent = cur->parent_;
        if (cur->left == nullptr) {
            if (parent == nullptr || cur == root) {
                root = cur->right;
                if (root != nullptr) {
                    root->parent_ = nullptr;
                }
                delete cur;
                cur = nullptr;
                size_--;
                return true;
            } else {
                if (cur == parent->left) {
                    parent->left = cur->right;
                } else {
                    parent->right = cur->right;
                }
                if (cur->right != nullptr) {
                    cur->right->parent_ = parent;
                }
                update_height(parent);
            }
            delete cur;
        } else if (cur->right == nullptr) {
            if (parent == nullptr || cur == root) {
                root = cur->left;
                root->parent_ = nullptr;
                delete cur;
                size_--;
                return true;
            } else {
                if (cur == parent->left) {
                    parent->left = cur->left;
                } else {
                    parent->right = cur->left;
                }
                if (cur->left != nullptr) {
                    cur->left->parent_ = parent;
                }
                update_height(parent);
            }
            delete cur;
        } else {
            parent = cur;
            AVLNode* minimal_right = cur->right;
            while (minimal_right->left) {
                parent = minimal_right;
                minimal_right = minimal_right->left;
            }
            if (parent == cur) {
                minimal_right->left = cur->left;
                if (cur->left != nullptr) {
                    cur->left->parent_ = minimal_right;
                }
                if (cur == root) {
                    root = minimal_right;
                    root->parent_ = nullptr;
                    delete cur;
                    update_height(minimal_right);
                } else {
                    if (cur->parent_->left == cur) {
                        cur->parent_->left = minimal_right;
                        minimal_right->parent_ = cur->parent_;
                    } else {
                        cur->parent_->right = minimal_right;
                        minimal_right->parent_ = cur->parent_;
                    }
                    delete cur;
                    update_height(minimal_right);
                }
            } else {
                if (root == cur) {
                    root = minimal_right;
                    root->parent_ = nullptr;
                } else {
                    if (cur == cur->parent_->left) {
                        cur->parent_->left = minimal_right;
                    } else {
                        cur->parent_->right = minimal_right;
                    }
                    minimal_right->parent_ = cur->parent_;
                }
                if (parent->left == minimal_right) {
                    parent->left = minimal_right->right;
                } else {
                    parent->right = minimal_right->right;
                }
                if (minimal_right->right != nullptr) {
                    minimal_right->right->parent_ = parent;
                }
                minimal_right->height = cur->height;
                minimal_right->left = cur->left;
                minimal_right->right = cur->right;
                cur->left->parent_ = minimal_right;
                cur->right->parent_ = minimal_right;
                delete cur;
                update_height(parent);
            }
        }
        size_--;
        return true;
    }
    void clear_the_tree(AVLNode* node){
        if (node == nullptr) {
            return;
        }
        AVLNode* tmp_left = node->left;
        AVLNode* tmp_right = node->right;
        delete node;
        clear_the_tree(tmp_left);
        clear_the_tree(tmp_right);
        size_ = 0;
        node = nullptr;
        return;
    }
public:
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        AVLNode* value;
        map* this_;
    public:
        iterator() {
            value = nullptr;
        }
        iterator(const iterator &other) {
            value = other.value;
            this_ = other.this_;
        }
        iterator(AVLNode* value_, map* this__) : value(value_), this_(this__) {}
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            if (value == nullptr) {
                throw invalid_iterator();
            }
            iterator tmp(*this);
            if (value->right) {
                value = value->right;
                while (value->left) {
                    value = value->left;
                }
                return tmp;
            } else {
                AVLNode* parent = value->parent_;
                while (parent && value == parent->right) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return tmp;
                }
                value = parent;
                return tmp;
            }
        }
        /**
         * TODO ++iter
         */
        iterator & operator++() {
            if (value == nullptr) {
                throw invalid_iterator();
            }
            if (value->right != nullptr) {
                value = value->right;
                while (value->left) {
                    value = value->left;
                }
                return *this;
            } else {
                AVLNode* parent = value->parent_;
                while (parent && value == parent->right) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return *this;
                }
                value = parent;
                return *this;
            }
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            if (!this_->root) {
                throw invalid_iterator();
            }
            AVLNode* tmp = this_->root;
            while (tmp->left) {
                tmp = tmp->left;
            }
            if (value == tmp) {
                throw invalid_iterator();
            }
            iterator cur_value(*this);
            if (value == nullptr) {
                AVLNode* node = this_->root;
                while (node->right) {
                    node = node->right;
                }
                value = node;
                return cur_value;
            }
            if (value->left) {
                value = value->left;
                while (value->right) {
                    value = value->right;
                }
                return cur_value;
            } else {
                AVLNode* parent = value->parent_;
                while (parent && value == parent->left) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return cur_value;
                }
                value = parent;
                return cur_value;
            }
        }
        /**
         * TODO --iter
         */
        iterator & operator--() {
            if (!this_->root) {
                throw invalid_iterator();
            }
            AVLNode* tmp = this_->root;
            while (tmp->left) {
                tmp = tmp->left;
            }
            if (value == tmp) {
                throw invalid_iterator();
            }
            if (value == nullptr) {
                AVLNode* node = this_->root;
                while (node->right) {
                    node = node->right;
                }
                value = node;
                return *this;
            }
            if (value->left) {
                value = value->left;
                while (value->right) {
                    value = value->right;
                }
                return *this;
            } else {
                AVLNode* parent = value->parent_;
                while (parent && value == parent->left) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return *this;
                }
                value = parent;
                return *this;
            }
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        value_type & operator*() const {
            if (!value) {
                throw invalid_iterator();
            }
            return (value->data);
        }
        AVLNode* get_value() const {return value;}
        map* get_map() const {return this_;}
        bool operator==(const iterator &rhs) const {
            return (value == rhs.get_value() && this_ == rhs.get_map());
        }
        bool operator==(const const_iterator &rhs) const {
            return (value == rhs.get_value() && this_ == rhs.get_map());
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
        /**
         * for the support of it->first. 
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        value_type* operator->() const noexcept {
            return &(value->data);
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
        private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        const AVLNode* value;
        const map* this_;
    public:
        const_iterator() {
            value = nullptr;
        }
        const_iterator(const iterator &other) {
            value = other.get_value();
            this_ = other.get_map();
        }
        const_iterator(const const_iterator &other) {
            value = other.value;
            this_ = other.this_;
        }
        const_iterator(const AVLNode* value_, const map* this__) : value(value_), this_(this__) {}
        const_iterator operator++(int) {
            if (value == nullptr) {
                throw invalid_iterator();
            }
            const_iterator tmp(*this);
            if (value->right) {
                value = value->right;
                while (value->left) {
                    value = value->left;
                }
                return tmp;
            } else {
                const AVLNode* parent = value->parent_;
                while (parent && value == parent->right) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return tmp;
                }
                value = parent;
                return tmp;
            }
        }
        const_iterator & operator++() {
            if (value == nullptr) {
                throw invalid_iterator();
            }
            if (value->right) {
                value = value->right;
                while (value->left) {
                    value = value->left;
                }
                return *this;
            } else {
                const AVLNode* parent = value->parent_;
                while (parent && value == parent->right) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return *this;
                }
                value = parent;
                return *this;
            }
        }
        const_iterator operator--(int) {
            if (!this_->root) {
                throw invalid_iterator();
            }
            AVLNode* tmp = this_->root;
            while (tmp->left) {
                tmp = tmp->left;
            }
            if (value == tmp) {
                throw invalid_iterator();
            }
            const_iterator cur_value(*this);
            if (value == nullptr) {
                const AVLNode* node = this_->root;
                while (node->right) {
                    node = node->right;
                }
                value = node;
                return cur_value;
            }
            if (value->left) {
                value = value->left;
                while (value->right) {
                    value = value->right;
                }
                return cur_value;
            } else {
                const AVLNode* parent = value->parent_;
                while (parent && value == parent->left) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return cur_value;
                }
                value = parent;
                return cur_value;
            }
        }
        const_iterator & operator--() {
            if (!this_->root) {
                throw invalid_iterator();
            }
            AVLNode* tmp = this_->root;
            while (tmp->left) {
                tmp = tmp->left;
            }
            if (value == tmp) {
                throw invalid_iterator();
            }
            if (value == nullptr) {
                const AVLNode* node = this_->root;
                while (node->right) {
                    node = node->right;
                }
                value = node;
                return *this;
            }
            if (value->left) {
                value = value->left;
                while (value->right) {
                    value = value->right;
                }
                return *this;
            } else {
                const AVLNode* parent = value->parent_;
                while (parent && value == parent->left) {
                    value = parent;
                    parent = value->parent_;
                }
                if (!parent) {
                    value = nullptr;
                    return *this;
                }
                value = parent;
                return *this;
            }
        }
        const value_type & operator*() const {
            if (!value) {
                throw invalid_iterator();
            }
            return (value->data);
        }
        const AVLNode* get_value() const {return value;}
        const map* get_map() const {return this_;}
        bool operator==(const iterator &rhs) const {
            return (value == rhs.get_value() && this_ == rhs.get_map());
        }
        bool operator==(const const_iterator &rhs) const {
            return (value == rhs.get_value() && this_ == rhs.get_map());
        }
        bool operator!=(const iterator &rhs) const {
            return (value != rhs.get_value() || this_ != rhs.get_map());
        }
        bool operator!=(const const_iterator &rhs) const {
            return (value != rhs.get_value() || this_ != rhs.get_map());
        }
        const value_type* operator->() const noexcept {
            return &(value->data);
        }
    };
    /**
     * TODO two constructors
     */
    bool check(AVLNode* node) {
        if (node == nullptr)
        {
            return true;
        }
        if (get_height(node->right) - get_height(node->left) != 0 && get_height(node->right) - get_height(node->left) != 1 && get_height(node->right) - get_height(node->left) != -1) {
            return false;
        }
        bool f1 = check(node->left);
        bool f2 = check(node->right);
        return (f1 && f2);
    }
    AVLNode* get() {return root;}
    map() {
        root = nullptr;
        size_ = 0;
        cmp = Compare();
    }
    map(const map &other) {
        root = copy_the_tree(other.root);
        size_ = other.size_;
        cmp = other.cmp;
    }
    /**
     * TODO assignment operator
     */
    map & operator=(const map &other) {
        if (&other == this) return *this;
        clear_the_tree(root);
        this->root = copy_the_tree(other.root);
        this->size_ = other.size_;
        this->cmp = other.cmp;
        return *this;
    }
    /**
     * TODO Destructors
     */
    ~map() {
        clear_the_tree(root);
        root = nullptr;
        size_ = 0;
    }
    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key equivalent to key.
     * If no such element exists, an exception of type `index_out_of_bound'
     */
    T & at(const Key &key) {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            throw index_out_of_bound();
        }
        return tmp->data.second;
    }
    const T & at(const Key &key) const {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            throw index_out_of_bound();
        }
        return tmp->data.second;
    }
    /**
     * TODO
     * access specified element 
     * Returns a reference to the value that is mapped to a key equivalent to key,
     *   performing an insertion if such key does not already exist.
     */
    T & operator[](const Key &key) {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            value_type tmp(key, T());        // !!
            insert_(tmp);
            return find_the_node(key)->data.second;
        }
        return tmp->data.second;
    }
    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T & operator[](const Key &key) const {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            throw index_out_of_bound();
        }
        return tmp->data.second;
    }
    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        AVLNode* minimal = root;
        while (minimal && minimal->left) {
            minimal = minimal->left;
        }
        return iterator(minimal, this);
    }
    const_iterator cbegin() const {
        const AVLNode* minimal = root;
        while (minimal && minimal->left) {
            minimal = minimal->left;
        }
        return const_iterator(minimal, this);
    }
    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const {return (size_ == 0);}
    /**
     * returns the number of elements.
     */
    size_t size() const {return size_;}
    /**
     * clears the contents
     */
    void clear() {
        clear_the_tree(root);
        root = nullptr;
    }
    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the insertion), 
     *   the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &value) {
        bool flag = insert_(value);
        AVLNode* tmp = find_the_node(value.first);
        iterator pointer(tmp, this);
        pair<iterator, bool> outcome{pointer, flag};
        return outcome;
    }
    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
     */
    void erase(iterator pos) {
        if (pos == this->end()) {
            throw index_out_of_bound();
        }
        if (this != pos.get_map()) {
            throw index_out_of_bound();
        }
        bool flag = erase_(pos.get_value()->data.first);
        if (!flag) {
            throw index_out_of_bound();
        }
        return;
    }
    /**
     * Returns the number of elements with key 
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0 
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
        AVLNode* tmp = find_the_node(key);
        if (tmp != nullptr) {
            return 1;
        } else {
            return 0;
        }
    }
    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is returned.
     */
    iterator find(const Key &key) {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            return this->end();
        } else {
            iterator it(tmp, this);
            return it;
        }
    }
    const_iterator find(const Key &key) const {
        AVLNode* tmp = find_the_node(key);
        if (tmp == nullptr) {
            return this->cend();
        } else {
            const_iterator it(tmp, this);
            return it;
        }
    }
    sjtu::vector<Key> traverse() {
        iterator it = begin();
        sjtu::vector<Key> result;
        while (it.get_value() != nullptr) {
            result.push_back(it->get_value->data.first);
            it++;
        }
        return result;
    }
};

}

#endif