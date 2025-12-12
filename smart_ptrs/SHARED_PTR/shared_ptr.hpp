#include <iostream>

class ControlBlock {
public:
    std::size_t count = 1;
};

template <typename T>
class SharedPtr {
private:
    T* ptr = nullptr;
    ControlBlock* control = nullptr;

    void release() {
        if (control) {
            control->count--;
            if (control->count == 0) {
                delete ptr;
                delete control;
            }
        }
    }

public:
    SharedPtr(T* p = nullptr) : ptr(p) { 
        if (p) control = new ControlBlock();
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), control(other.control) {
        if (control) control->count++;
    }
    
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), control(other.control) {
        other.ptr = nullptr;
        other.control = nullptr;
    }
    
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            control = other.control;
            if (control)
            control->count++;
        }
        return *this;
    }
    
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            control = other.control;
            other.ptr = nullptr;
            other.control = nullptr;
        }
        return *this;
    }
    
    ~SharedPtr() {
        release();
    }
    void reset(T* p = nullptr) {
        release();
        if (p) {
            ptr = p;
            control = new ControlBlock();
        } else {
            ptr = nullptr;
            control = nullptr;
        }
    }

    T* get() const { return ptr; }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    size_t use_count() const { return control ? control->count : 0; }
};




    
