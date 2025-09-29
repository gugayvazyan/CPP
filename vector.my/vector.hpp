#ifndef MY_VECTOR_H
#define MY_VECTOR_H

struct MyVector {
    int* data;
    size_t size;
    size_t capacity;
};

void myvector_init(MyVector*, size_t);
void myvector_init(MyVector*, size_t, int);

void realloc_helper(MyVector*);
 
void myvector_destroy(MyVector*);
void myvector_clear(MyVector*);

size_t myvector_size(const MyVector*);
size_t myvector_capacity(const MyVector*);

void myvector_push_back(MyVector*, int);
void myvector_pop_back(MyVector*);

void myvector_insert(MyVector*, size_t, int); 
void myvector_erase(MyVector*, size_t);

void myvector_print(const MyVector*);








#endif // MY_VECTOR_H