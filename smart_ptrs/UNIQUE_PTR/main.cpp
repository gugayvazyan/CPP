#include <iostream>
#include "unique_ptr.hpp"


int main() {
    uniquee_ptr<int> a;
    a.reset(new int(5));
    
}