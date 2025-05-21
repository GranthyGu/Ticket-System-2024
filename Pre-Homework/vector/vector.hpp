#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
	T* data;
	size_t capacity;
	size_t current_size;
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T* ptr;
		vector<T>* vec;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator() : ptr(nullptr), vec(nullptr) {}
		iterator(T* p = nullptr, vector<T>* v = nullptr) : ptr(p), vec(v) {}
		iterator operator+(const int &n) const {
			//TODO
			iterator tmp(ptr + n);
			return tmp;
		}
		iterator operator-(const int &n) const {
			//TODO
			iterator tmp(ptr - n);
			return tmp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		T* get_pos() const {return ptr;}
		vector<T>* get_vec() const {return vec;}
		int operator-(const iterator &rhs) const {
			//TODO
			T* tmp = rhs.get_pos();
			if (vec != rhs.get_vec()) {
				throw invalid_iterator();
			} else {
				return ptr - tmp;
			}
		}
		iterator& operator+=(const int &n) {
			//TODO
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n) {
			//TODO
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			ptr++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			ptr++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			ptr--;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			ptr--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return (ptr == rhs.get_pos());
		}
		bool operator==(const const_iterator &rhs) const {
			return (ptr == rhs.get_pos());
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return (ptr != rhs.get_pos());
		}
		bool operator!=(const const_iterator &rhs) const {
			return (ptr != rhs.get_pos());
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	private:
		/*TODO*/
		const T* ptr;
		const vector<T>* vec;
	public:
		const_iterator() : ptr(nullptr), vec(nullptr) {}
		const_iterator(const T* p = nullptr, const vector<T>* v = nullptr) : ptr(p), vec(v) {}
		const_iterator operator+(const int &n) const {
			const_iterator tmp(ptr + n);
			return tmp;
		}
		const_iterator operator-(const int &n) const {
			const_iterator tmp(ptr - n);
			return tmp;
		}
		const T* get_pos() const {return ptr;}
		const vector<T>* get_vec() const {return vec;}
		int operator-(const const_iterator &rhs) const {
			T* tmp = rhs.get_pos();
			if (vec != rhs.get_vec()) {
				throw invalid_iterator();
			} else {
				return ptr - tmp;
			}
		}
		const_iterator& operator+=(const int &n) {
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			ptr++;
			return tmp;
		}
		const_iterator& operator++() {
			ptr++;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			ptr--;
			return tmp;
		}
		const_iterator& operator--() {
			ptr--;
			return *this;
		}
		const T& operator*() const{
			return *ptr;
		}
		bool operator==(const iterator &rhs) const {
			return (ptr == rhs.get_pos());
		}
		bool operator==(const const_iterator &rhs) const {
			return (ptr == rhs.get_pos());
		}
		bool operator!=(const iterator &rhs) const {
			return (ptr != rhs.get_pos());
		}
		bool operator!=(const const_iterator &rhs) const {
			return (ptr != rhs.get_pos());
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : current_size(0), capacity(1) {
		data = (T*) malloc(capacity * sizeof(T));
	}
	vector(const vector &other) : current_size(other.current_size), capacity(other.capacity) {
		data = (T*) malloc(capacity * sizeof(T));
		for (size_t i = 0; i < current_size; i++) {
			new (data + i) T (other.data[i]);
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		clear();
		free(data);
		data = nullptr;
		capacity = 0;
		current_size = 0;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) {
			return *this;
		}
		if (data) {
			for (int i = 0; i < current_size; i++) {
				data[i].~T();
			}
			free(data);
		}
		data = (T*) malloc(other.capacity * sizeof(T));
		capacity = other.capacity;
		current_size = other.current_size;
		for (size_t i = 0; i < other.current_size; i++) {
			new (data + i) T (other.data[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= current_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return data[pos];
		}
	}
	const T & at(const size_t &pos) const {
		if (pos >= current_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return data[pos];
		}
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		return at(pos);
	}
	const T & operator[](const size_t &pos) const {
		return at(pos);
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (current_size == 0) {
			throw container_is_empty();
		} else {
			return data[0];
		}
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (current_size == 0) {
			throw container_is_empty();
		} else {
			return data[current_size - 1];
		} 
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data);
	}
	const_iterator cbegin() const {
		return const_iterator(data);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + current_size);
	}
	const_iterator cend() const {
		return const_iterator(data + current_size);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return (current_size == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return current_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < current_size; i++) {
			data[i].~T();
		}
		current_size = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int index = pos - begin();
		if (capacity == 0) {
			capacity = 1;
			data = (T*) malloc(capacity * sizeof(T));
			new (data) T (value);
			current_size++;
			return pos;
		}
		if (current_size == capacity)
		{
			capacity *= 2;
			T* tmp = (T*) malloc(capacity * sizeof(T));
			for (int i = 0; i < index; i++) {
				new (tmp + i) T (std::move(data[i]));
			}
			new (tmp + index) T (value);
			for (int i = index; i < current_size; i++) {
				new (tmp + i + 1) T (std::move(data[i]));
			}
			for (int i = 0; i < current_size; i++) {
				data[i].~T();
			}
			free(data);
			data = tmp;
			current_size++;
		} else {
			for (int i = current_size - 1; i >= index; --i){
                new (data + i + 1) T (std::move(data[i]));
				data[i].~T();
            }
            new (data + index) T (value);
			current_size++;
		}
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > current_size || ind < 0) {
			throw index_out_of_bound();
		}
		return insert(begin() + ind, value);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		if (pos == end()) {
			throw index_out_of_bound();
		}
		size_t index = pos - begin();
		data[index].~T();
		for (size_t i = index; i < current_size - 1; i++) {
			new (data + i) T (std::move(data[i + 1]));
			data[i + 1].~T();
		}
		--current_size;
		return iterator(data + index);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= current_size || ind < 0) {
			throw index_out_of_bound();
		}
		return erase(begin() + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(current_size, value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (current_size == 0) {
			throw container_is_empty();
		}
		erase(current_size - 1);
	}
};


}

#endif