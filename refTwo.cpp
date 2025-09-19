#include <iostream>


int main() {

	int x = 0;
	
	std::cout << "Enter number: ";
	
	std::cin >> x;

	std::cout << "Your num: " << x << std::endl;


	int &a = x; int &b = x;
	 
	a = 77;

	std::cout << "Your b: " << b << std::endl;

	return 0;
}
