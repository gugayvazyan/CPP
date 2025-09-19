#include <iostream>


void sum(int &a, int &b, int &res) { res = a + b; }



int main() {
	
	int a = 0;
	int b = 0;
	int res = 0;

	std::cout << "Input two numbers: ";
	std::cin >> a >> b;
	
	sum(a,b,res);
	std::cout << "Result: " << res << std::endl;
	
	return 0;
}
	
