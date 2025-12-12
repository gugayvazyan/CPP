
template <typename T>
class uniquee_ptr { 
    T* data;
public:
    uniquee_ptr(T* _data = nullptr) noexcept : data(_data) {}

    uniquee_ptr(uniquee_ptr&& other) noexcept : data(other.data) { other.data = nullptr; }
    uniquee_ptr& operator=(uniquee_ptr&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    uniquee_ptr(const uniquee_ptr&) = delete;
    uniquee_ptr& operator=(const uniquee_ptr&) = delete;
    
    ~uniquee_ptr() { delete data; }

public:
    void reset(T* ptr = nullptr) noexcept {
        if (data != ptr) {
        delete data;
        data = ptr;
        }
    }

    T* release() noexcept {
        T* tmp = data;
        data = nullptr;
        return tmp;
    }

    void swap(uniquee_ptr& other) noexcept {
        std::swap(data, other.data);
    }


    T* get() const noexcept {
        return data;
    }

    explicit operator bool() const noexcept {
        return data != nullptr;
    }

    T* operator->() const noexcept {
        return data;
    }

    T& operator*() const noexcept {
        return *data;
    }


};
