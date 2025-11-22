#include "Vtable.hpp"
#include <iostream>

int main() {
    Virtualizable* v = new Base;

    if (Derived* d = dynamicCast<Derived>(v)) {
        std::cout  << v->type() << std::endl;
    }

    if (Base* b = dynamicCast<Base>(v)) {
       std::cout  << v->type() << std::endl;
    }



    return 0;
}
