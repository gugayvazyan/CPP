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

        VecInt();
        VecInt(std::size_t n, int val = 0);
        VecInt(std::initializer_list<int> init);
        VecInt(const VecInt& other);
        ~VecInt();


        
        size_t size() const;
        size_t capacity() const;


    
        void push_back(int val);
        void pop_back();
        void insert(size_t ind, int val);
        void myvector_clear();


        
        void print() const;
};


#endif //VECINT_HPP
