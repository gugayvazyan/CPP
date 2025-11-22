#include <vector>
#include <string>
#include <iostream>

class Virtualizable {
     std::vector<std::string> names;
public:
    using FnPtr = void (Virtualizable::*)();

    void registerVirtual(const std::string& name, FnPtr fn) {
        this->names.push_back(name);
        this->funcs.push_back(fn);
    }
    
    void callVirtual(const std::string& name) {
        for (int i = 0; i < names.size(); ++i) {
            if (names[i] == name) (this->*funcs[i])();
        }
    }
    
    Virtualizable (const std::string& name, const std::string& funcName1, FnPtr fooPtr, const std::string& funcName2, FnPtr barPtr) : typeid_{name} {
        registerVirtual(funcName1, fooPtr);
        registerVirtual(funcName2, barPtr);
    }
    
private:
    
    std::string typeid_;
    std::vector<FnPtr> funcs;

public:
    std::string type() { return this->typeid_; }
};


class Base : public Virtualizable {

    std::string typeid_ = "Base";

public:
    static std::string type() { return std::string("Base"); }
    
    void bar() { std::cout << "Base::Bar" << std::endl; }
    void foo() { std::cout << "Base::Foo" << std::endl; }
    
    Base (const std::string& name = type(), const std::string& funcName1 = "foo", FnPtr fooPtr = (FnPtr)&Base::foo,
          const std::string& funcName2 = "bar", FnPtr barPtr = (FnPtr)&Base::bar) : Virtualizable(name, funcName1, fooPtr, funcName2, barPtr) {}
};


class Derived : public Base {
    std::string typeid_ = "Derived";

public:
    static std::string type() { return std::string("Derived"); }
        
    void bar() { std::cout << "Derived::Bar" << std::endl; }
    void foo() { std::cout << "Derived::Foo" << std::endl; }

    Derived (const std::string& name = type(), const std::string& funcName1 = "foo", FnPtr fooPtr = (FnPtr)&Derived::foo,
             const std::string& funcName2 = "bar", FnPtr barPtr = (FnPtr)&Derived::bar) : Base(name, funcName1, fooPtr, funcName2, barPtr) {}
};


  
template <typename T>
T* dynamicCast(Virtualizable* ptr) {
    if (ptr->type() == T::type())
        return static_cast<T*>(ptr);

        return nullptr;
}
