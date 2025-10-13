#ifndef VECINT_HPP
#define VECINT_HPP
#include <initializer_list>
#include <iostream>


class VecInt {
    private:
        int* data;
        size_t _size;
        size_t _capacity;

    private:
        void realloc_helper();

    public:

    VecInt() : data(nullptr), _size(0), _capacity(0) {}

    VecInt(std::size_t n, int val);

    VecInt(std::initializer_list<int> init);

    VecInt(const VecInt& other);
    VecInt(VecInt&& other);

    VecInt& operator= (const VecInt& other);
    VecInt& operator= ( VecInt&& other);

    ~VecInt();

    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    

    void push_back(int val);

    void pop_back();

    void insert(size_t ind, int val);

    void myvector_clear();

    void print() const;
};

#endif //VECINT_HPP