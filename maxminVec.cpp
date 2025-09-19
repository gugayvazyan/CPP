#include <iostream>
#include <vector>

int main() {

	int N = 0;

	std::cout << "Input arr size: ";
	std::cin >> N;

	std::vector<int> arr(N);
	std::cout << "Input " << N << " number: ";

	for	(int i = 0; i < N; ++i) {
		std::cin >> arr[i];
	}

	int max = arr[0];
	int min = arr[0];

	for(int i = 1; i < N; ++i) {
		if (max < arr[i]) max = arr[i];
		if (min > arr[i]) min = arr[i];
	}

	std::cout << "Max: " << max << std::endl;
	std::cout << "Min: " << min << std::endl;
	
	return 0;
}
		
