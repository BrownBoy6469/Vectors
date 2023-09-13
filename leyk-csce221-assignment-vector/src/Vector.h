#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        T* arrayCopy = array;

        if(capacity() == 0) {
            array = new T[1];
            _capacity += 1;
        }
        else {
            array = new T[_capacity * 2];
            _capacity *= 2;
        }

        for (int i = 0; i < size(); i++) {
            array[i] = std::move(arrayCopy[i]);
        }
        
        delete[] arrayCopy;
        arrayCopy = nullptr;
     }

public:
    Vector() noexcept { 
        array = nullptr;
        _capacity = 0;
        _size = 0;
     }
    Vector(size_t count, const T& value) { 
        array = new T[count];
        _capacity = count;
        _size = count;

        for (int i = 0; i < count; i++) {
            array[i] = value;
        }
     }
    explicit Vector(size_t count) { 
        array = new T[count]();
        _capacity = count;
        _size = count;
        
        /*for (int i = 0; i < count; i++) {
            array[i] = 0;
        }*/
     }
    Vector(const Vector& other) { 
        array = new T[other.size()];
        _capacity = other.capacity();
        _size = other.size();

        for (int i = 0; i < _size; i++) {
            this->array[i] = other.array[i];
        }
     }
    Vector(Vector&& other) noexcept { 
        this->array = other.array;
        this->_capacity = other.capacity();
        this->_size = other.size();

        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
     }

    ~Vector() { 
        delete[] array;
        array = nullptr;
        _capacity = 0;
        _size = 0;
     }

    Vector& operator=(const Vector& other) { 
        if(this->array == other.array){
            return *this;
        }

        delete[] array;
        array = new T[other.size()];
        _capacity = other.capacity();
        _size = other.size();

        for (int i = 0; i < _size; i++) {
            this->array[i] = other.array[i];
        }
        return *this;
     }
    Vector& operator=(Vector&& other) noexcept { 
        if(this->array == other.array){
           return *this;
        }
        
        delete[] array;
        this->array = other.array;
        this->_capacity = other.capacity();
        this->_size = other.size();

        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
        return *this;
     }

    iterator begin() noexcept { 
        return iterator(array);
     }
    iterator end() noexcept { 
        return iterator(array + _size);
     }

    [[nodiscard]] bool empty() const noexcept { 
        if(_size == 0) {
            return true;
        }
        else {
            return false;
        }
        
     }
    size_t size() const noexcept { 
        return _size;
     }
    size_t capacity() const noexcept { 
        return _capacity;
     }

    T& at(size_t pos) { 
        if((array + pos >= array + _size)) {
            throw std::out_of_range("Error: Out of Range");
        } 
        else if((array + pos) < array) {
            throw std::out_of_range("Error: Out of Range");
        }
        else {
            return *(array + pos);
        }
     }
    const T& at(size_t pos) const { 
        if((array + pos >= array + _size)) {
            throw std::out_of_range("Error: Out of Range");
        } 
        else if((array + pos) < array) {
            throw std::out_of_range("Error: Out of Range");
        }
        else {
            return *(array + pos);
        }
     }
    T& operator[](size_t pos) { 
        return *(array + pos);
     }
    const T& operator[](size_t pos) const { 
        return *(array + pos);
     }
    T& front() { 
        return array[0];
     }
    const T& front() const { 
        return array[0];
     }
    T& back() { 
        return array[size() - 1];
     }
    const T& back() const { 
        return array[size() - 1];
     }

    void push_back(const T& value) { 
        if(size() == capacity()) {
            grow();
        }
        array[size()] = value;
        _size += 1;
     }
    void push_back(T&& value) { 
        if(size() == capacity()) {
            grow();
        }
        array[size()] = std::move(value);
        _size += 1;
     }
    void pop_back() { 
        _size -= 1;
     }

    iterator insert(iterator pos, const T& value) { 
        int index;
        for(int i = 0; i < size(); i++) {
            if(pos == array + i) {
                index = i;
            }
        }

        if(size() == capacity()) {
            grow();
            pos = array + index;
        }

        for(auto it = end(); it != pos; it--) {
            *it = std::move(*(it - 1));
        }
        _size++;
        *pos = value;

        return pos;
     }
    iterator insert(iterator pos, T&& value) { 
        int index;
        for(int i = 0; i < size(); i++) {
            if(pos == array + i) {
                index = i;
            }
        }

        if(size() == capacity()) {
            grow();
            pos = array + index;
        }

        for(auto it = end(); it != pos; it--) {
            *it = std::move(*(it - 1));
        }
        
        *pos = std::move(value);
        _size++;

        return pos;
     }
    iterator insert(iterator pos, size_t count, const T& value) { 
        if(count == 0) {
            return pos;
        }
        
        int index;
        for(int i = 0; i < size(); i++) {
            if(pos == array + i) {
                index = i;
            }
        }

        while(size() + count > capacity()){
            grow();
        }
        pos = array + index;
        

        for(auto it = end() + count - 1; it != pos + count - 1; it--) {
            *it = std::move(*(it - count));
        }

        for(int i = 0; i < count; i++) {
            *(pos + i) = value;
        }
        _size += count;

        return pos;
     }
    iterator erase(iterator pos) { 
        if(begin() == end()) {
            return end();
        }

        for(auto it = pos; it != end() - 1; it++) {
            *it = std::move(*(it + 1));
        }
        _size -= 1;

        return pos;
     }
    iterator erase(iterator first, iterator last) { 
        if(begin() == end()) {
            return end();
        }

        int dist = last - first;
        if(dist <= 0) {
            return last;
        }

        for(auto it = last; it != end(); it++) {
            *(it - dist) = std::move(*it);
        }
        _size -= dist;

        return first;
     }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
        T* itr;
    public:
        iterator() { 
            itr = nullptr;
         }
        // Add any constructors that you may need
        iterator(T* ptr) {
            itr = ptr;
        }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { 
            return *itr;
         }

        // This is used to access data members of the underlying elements
        // It should return the address of the element the iterator points to
        [[nodiscard]] pointer operator->() const noexcept { 
            return itr;
         }

        // Prefix Increment: ++a
        // The value returned should be the iterator moved forward in the container
        iterator& operator++() noexcept { 
            itr = itr + 1;
            return *this;
         }

        // Postfix Increment: a++
        // The value returned should be the iterator before it was moved forward in the container
        iterator operator++(int) noexcept { 
            iterator itrCopy = itr;
            itr = itr + 1;
            return itrCopy;
         }

        // Prefix Decrement: --a
        // The value returned should be the iterator moved backwards in the container
        iterator& operator--() noexcept { 
            itr = itr - 1;
            return *this;
         }

        // Postfix Decrement: a--
        // The value returned should be the iterator before it was moved backwards in the container
        iterator operator--(int) noexcept { 
            iterator itrCopy = itr;
            itr = itr - 1;
            return itrCopy;
         }

        // Should move the iterator forward by an offset and return the new iterator
        iterator& operator+=(difference_type offset) noexcept { 
            itr = itr + offset;
            return *this;
         }
        
        // Should return a copy of the iterator moved forward by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator itrCopy = itr;
            itrCopy.itr = itrCopy.itr + offset;
            return itrCopy;
         }
        
        // Should move the iterator backwards by an offset and return the new iterator
        iterator& operator-=(difference_type offset) noexcept { 
            itr = itr - offset;
            return *this;
         }

        // Should return a copy of the iterator moved backwards by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            iterator itrCopy = itr;
            itrCopy.itr = itrCopy.itr - offset;
            return itrCopy;
         }

        // Should return the difference between two iterators. This should be the offset betweent the
        // elements in memory.
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            int diff = itr - rhs.itr;
            return diff;
         }

        // Should return the element at the location offset from the iterator
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { 
            iterator itrCopy = itr;
            itrCopy = itrCopy.itr + offset;
            return *itrCopy;
         }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            if(itr == rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { 
            if(itr != rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { 
            if(itr < rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { 
            if(itr > rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { 
            if(itr <= rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { 
            if(itr >= rhs.itr) {
                return true;
            }
            else {
                return false;
            }
         }
    };


    void clear() noexcept { 
        delete[] array;
        array = nullptr;
        _size = 0;
     }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { 
    _Iterator iteratorCopy = iterator;
    iteratorCopy = iteratorCopy + offset;
    return iteratorCopy; 
 }

#endif
