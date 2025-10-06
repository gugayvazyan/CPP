#include "VecInt.hpp"

VecInt::VecInt() : data(nullptr), _size(0), _capacity(0) {}


VecInt::VecInt(std::size_t n, int val) : data(new int[n]), _size(n), _capacity(n) {
    for (std::size_t i = 0; i < n; ++i) data[i] = val;
}


VecInt::VecInt(std::initializer_list<int> init): data(new int[init.size()]), _size(0), _capacity(init.size()) {
    for (int val : init) data[_size++] = val;
}


VecInt::VecInt(const VecInt& other) : data(new int[other._capacity]), _size(other._size), _capacity(other._capacity) {
    for (std::size_t i = 0; i < _size; ++i) data[i] = other.data[i];
}


VecInt::~VecInt() { delete[] data;}



size_t VecInt::size() const { return _size; }
size_t VecInt::capacity() const { return _capacity; }



void VecInt::realloc_helper() {
    _capacity = (_capacity == 0) ? 1 : _capacity * 2;
    int* tmp = new int[_capacity];
    for (size_t i = 0; i < _size; ++i) tmp[i] = data[i];
    delete[] data;
    data = tmp;
}



void VecInt::push_back(int val) {
    if (_size == _capacity) realloc_helper();
    data[_size++] = val;
}


void VecInt::pop_back() {
    if (_size == 0) {
        std::cout << "Vector is empty:" << std::endl;
        return;
    }
    --_size;
}


void VecInt::insert(std::size_t ind, int val) {
    if (ind > _size) {
        std::cout << "You can't use index bigger than your size" << std::endl;
        return;
    }
    if (_size == _capacity) realloc_helper();
    for (std::size_t i = _size; i > ind; --i) data[i] = data[i - 1];
    data[ind] = val;
    ++_size;
}


void VecInt::myvector_clear() { _size = 0; }

void VecInt::print() const {
for (size_t i = 0; i < _size; ++i) std::cout << data[i] << " ";
std::cout << std::endl;
}