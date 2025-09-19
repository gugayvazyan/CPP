#include <iostream> 

void swap(int &a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}


int main() {
	int a;
	int b;
	
	std::cout << "Input 2 values: ";
	std::cin >> a >> b;
	std::cout << a << " " << b << std::endl;
	swap(a,b);
	std::cout << "Your result: " << a << " " <<	b << std::endl;
	
	return 0;
}
