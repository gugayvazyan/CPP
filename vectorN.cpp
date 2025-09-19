#include <iostream>
#include <vector>

int main() {
    
    int N = 0;
    int sum = 0;
    std::cout << "Enter your nums amount: ";
    std::cin >> N;
    
    std::vector <int> arr(N);

    std::cout << "Enter " << N << " numbers";
    
    for (int i = 0; i < N; ++i) {
        std::cin >> arr[i];
        sum += arr[i];
    }
    
    std::cout << "Your sum: " << sum << std::endl;
    std::cout << "Your avg: " << sum / N << std::endl;
    
    return 0;
}
        
