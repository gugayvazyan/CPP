#include <iostream>


int main() {

	int x = 0;
	
	std::cout << "Enter number: ";
	
	std::cin >> x;

	std::cout << "Your num: " << x << std::endl;
	
	int y = x;
	int &r = x;
	std::cout << "Your x: " << x << " y: " << y << " r: " << r << std::endl;	

	y = 7;
	r = 4;
	

	std::cout << "Your x: " << x << " y: " << y << " r: " << r << std::endl;
	
	return 0;
}
	
