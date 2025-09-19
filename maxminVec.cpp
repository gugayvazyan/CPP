#include <iostream>
#include <vector>

void maxmin(const std::vector<int>& arr, int &max, int &min) {
    max = arr[0];
    min = arr[0];

    for (int x : arr) {
        if (x > max) max = x;
        if (x < min) min = x;
    }
}
       




int main() {
    int N = 0;
    int sum = 0;
    std::cout << "Enter your nums amount: ";
    std::cin >> N;
    
    std::vector <int> arr(N);

    std::cout << "Enter " << N << " numbers: ";
    
    for (int i = 0; i < N; ++i) {
        std::cin >> arr[i];
        sum += arr[i];
    }

    int max = arr[0];
    int min = arr[0];
    
    maxmin(arr, max, min);
    
    std::cout << "Max: " << max << std::endl;
    std::cout << "Min: " << min << std::endl;
    
    return 0;
}
