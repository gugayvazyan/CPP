#include <iostream>


int main() {
    int arr[3] = {1,2,3};
    int &r = arr[0];

    std::cout << arr[0] << std::endl;

    r = 5;
    
    std::cout << arr[0] << std::endl;
    
 
    return 0;
    
}
