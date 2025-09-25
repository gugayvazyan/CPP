#include <iostream>
#include "vector.hpp"
#include "vector.cpp"


int main() {
    MyVector arr;
    myvector_init(&arr, 10, 5);
    std::cout << "Size: "<< myvector_size(&arr) << std::endl;
    std::cout << "Capacity: " << myvector_capacity(&arr) << std::endl;
    myvector_push_back (&arr, 14);
    myvector_print(&arr);
    std::cout << "Size: "<< myvector_size(&arr) << std::endl;
    std::cout << "Capacity: " << myvector_capacity(&arr) << std::endl;
    myvector_pop_back(&arr);
    myvector_print(&arr);
    std::cout << "Size: "<< myvector_size(&arr) << std::endl;
    std::cout << "Capacity: " << myvector_capacity(&arr) << std::endl;
    myvector_clear(&arr);
    std::cout << "Size: "<< myvector_size(&arr) << std::endl;
    std::cout << "Capacity: " << myvector_capacity(&arr) << std::endl;
    myvector_print(&arr);
    myvector_destroy(&arr);
    std::cout << "Size: "<< myvector_size(&arr) << std::endl;
    std::cout << "Capacity: " << myvector_capacity(&arr) << std::endl;

    return 0;


}