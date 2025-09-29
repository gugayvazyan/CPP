
#include <stdlib.h>
#include <iostream>
#include "vector.hpp"



void myvector_init(MyVector* v, size_t capacity){
    v->data = new int[capacity];
    v->size = 0;
    v->capacity = capacity;
}


void myvector_init(MyVector* v, size_t capacity, int val){
    v->data = new int[capacity];
    v->size = capacity;
    v->capacity = capacity;
    for(int i = 0; i < capacity; ++i) v->data[i] = val;
}

void myvector_destroy(MyVector* v) {

    delete[] v->data;
    v->data = nullptr;
    v->size = v->capacity = 0;
}

void realloc_helper(MyVector* v) { 
    v->capacity = (v->capacity == 0) ? 1 : v->capacity * 2;
    
    int* tmp = new int[v->capacity];
    for (int i = 0; i < v->size; ++i) tmp[i] = v->data[i];
    delete[]v->data;
    v->data = tmp;

}


void myvector_clear(MyVector* v){
    v->size = 0; 
    
}


size_t myvector_size(const MyVector* v){ return v->size;}
size_t myvector_capacity(const MyVector* v){ return v->capacity;}


void myvector_push_back(MyVector* v, int value){ 
    if(v->size == v->capacity) {
        
        realloc_helper(v);
    } 
    
    v->data[v->size] = value;
    ++(v->size);
}

void myvector_pop_back(MyVector* v) {
    if (v->size == 0) {
        std::cout << "Vector is empty:" << std::endl;
        return;
    }
    
    --(v->size);
}

void myvector_insert(MyVector* v, size_t index, int value){
    if(index > v->size || index < 0) {
        std::cout << "You can't use index bigger than your size" << std::endl;
        return;
    }

    if(v->size == v->capacity) {
        realloc_helper(v);
    }

    for (int i = v->size; i > index; --i) v->data[i] = v->data[i - 1];
    v->data[index] = value;
    ++(v->size);
}


void myvector_erase(MyVector* v, size_t index){
    if(index > v->size || index < 0 || v->size == 0) {
        std::cout << "You can't use index bigger than your size" << std::endl;
        return;
    }


    for(size_t i = index; i < v->size - 1; ++i) {
        v->data[i] = v->data[i + 1];
    }
    
    --(v->size);
    
}

void myvector_print(const MyVector* v){
    for(int i = 0; i < v->size; ++i) std::cout << v->data[i] << " ";
    std::cout << std::endl;
}

