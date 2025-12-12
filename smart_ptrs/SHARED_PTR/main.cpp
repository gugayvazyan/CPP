#include "shared_ptr.hpp"
#include <iostream>




int main() {
    SharedPtr<int> a(new int(10));
    std::cout << a.use_count() << std::endl;
    
    SharedPtr<int> b = a;
    std::cout << b.use_count() << std::endl;
    SharedPtr<int> c = a;
    std::cout << c.use_count() << std::endl;
    SharedPtr<int> d = a;
    std::cout << d.use_count() << std::endl;
    SharedPtr<int> f = a;
    std::cout << f.use_count() << std::endl;
    
    
    std::cout << b.use_count() << std::endl;

    std::cout << a.use_count() << std::endl;
    

    
    return 0;
}