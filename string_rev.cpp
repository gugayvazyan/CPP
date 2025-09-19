#include <iostream>
#include <string>
#include <cstring>

void reverse(std::string &arr) {
	int len = arr.length();
	for(int i = 0; i < len / 2; ++i) {
		char tmp = arr[i];
		arr[i] = arr[len - i - 1];
		arr[len - i - 1] = tmp;
	}
}


int main() {
	
	std::string arr;
	
	std::cout << "Write something: ";
	
	std::cin >> arr;
	
	std::cout << "Your text: " << arr << std::endl;
	
	reverse(arr);
	
	std::cout << "Reversed: " << arr << std::endl;

	return 0;
}
