#include <iostream>
#include <cmath>
#include <vector>

int main() {
	int a = 3, b = 4;
	std::vector<std::vector<double>> eigen_vectors(6, std::vector<double>(6, 0));
	for (int i {0}; i < 6; ++i) {
		eigen_vectors[i][i] = 1;
	}
	for (int i {0}; i < 6; ++i) {
		for (int j {0}; j < 6; ++j) {
			std::cout << eigen_vectors[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	return 0;
}