#include <iostream>
int main() {
	int a = 3, b = 4;
	{
		int a = 1, b = 2;
		std::cout << a << b << std::endl;
	}
	return 0;
}