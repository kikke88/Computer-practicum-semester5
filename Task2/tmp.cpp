#include <iostream>
int main() {
	int a = 3, b = 4;
	{
		int a  = (-1)* 2,b = 3 * -1 * 3, c = -3 * -3;
		std::cout << a << b << c << std::endl;
	}
	return 0;
}