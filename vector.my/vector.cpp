
#include <stdlib.h>
#include <iostream>

void myvector_init(MyVector* v, size_t capacity){
    v->data = new int[capacity];
    v->capacity = capacity;
    v->size = 0;
}


void myvector_init(MyVector* v, size_t size, int val){
    v->data = new int[size * 2];
    v->capacity = size * 2;
    v->size = size;
    for(int i = 0; i < size; ++i) v->data[i] = val;
}

void myvector_destroy(MyVector* v) {
    delete[]v->data;
    v->data = nullptr;
    v->size = 0;
    v->capacity = 0;
}

void myvector_clear(MyVector* v){
    for(int i = 0; i < v->size; ++i) v->data[i] = 0; 
}


size_t myvector_size(const MyVector* v){ return v->size;}
size_t myvector_capacity(const MyVector* v){ return v->capacity;}


void myvector_push_back(MyVector* v, int value){ 
    if(v->size == v->capacity || v->size + 1 == v->capacity) {
        v->capacity *= 2;
        int* tmp = new int[v->size];
        for (int i = 0; i < v->size; ++i) tmp[i] = v->data[i];
        delete[]v->data;
        v->data = new int[v->capacity];
        for (int i = 0; i < v->size; ++i) v->data[i] = tmp[i];
        delete[]tmp;
        ++v->size;
        v->data[v->size - 1] = value;
    } else {
        v->data[v->size] = value;
        ++v->size;
    }
}

    void myvector_pop_back(MyVector* v){ --v->size; }

    void myvector_insert(MyVector* v, size_t index, int value){
    if(index > v->size) throw "INSERT: you can't use index bigger than your size";
    if(v->size == v->capacity || v->size + 1 == v->capacity) {
        v->capacity *= 2;
        int* tmp = new int[v->size];
        for (int i = 0; i < v->size; ++i) tmp[i] = v->data[i];
        delete[]v->data;
        v->data = new int[v->capacity];
        for (int i = 0; i < v->size; ++i) v->data[i] = tmp[i];
        delete[]tmp;
    }
    for(int i = v->size; i > index; --i) v->data[i] = v->data[i - 1];
    v->data[index] = value;
    ++v->size;
}


    void myvector_erase(MyVector* v, size_t index){
        if(index > v->size) throw "INSERT: you can't use index bigger than your size";
        
        for(size_t i = index; i < v->size - 1; ++i) {
        v->data[i] = v->data[i + 1];
    }
        --v->size;
    
    }

    void myvector_print(const MyVector* v){
        for(int i = 0; i < v->size; ++i) std::cout << v->data[i] << " ";
        std::cout << std::endl;
    }

