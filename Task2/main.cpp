#include <iostream>
#include <fstream>
#include <vector>

void array_input(int& size, std::vector<std::vector<double>>& arr, std::ifstream& file) {
	file >> size;
	double tmp;
	for (int i {0}; i < size; ++i) {
		arr.push_back(std::vector<double>());
		for (int j {0}; j < size; ++j) {
			file >> tmp;
			arr[i].push_back(tmp);
		}
	}
}

void array_input(int& size, std::vector<std::vector<double>>& arr,
				std::function<double(const int, const int, const int)> func)
{
	std::cin >> size;
	double tmp;
	for (int i {0}; i < size; ++i) {
		arr.push_back(std::vector<double>());
		for (int j {0}; j < size; ++j) {
			tmp = func(i, j, size);
			arr[i].push_back(tmp);
		}
	}
}

void main_func(int size, std::vector<std::vector<double>>& arr, std::vector<std::vector<double>> evec, double eps)
{

}
int main(int argc, char* argv[])
{
	int strategy;
	std::cout << "Enter the number of the strategy 1 / 2 / 3." << std::endl;
	std::cin >> strategy;
	int array_size;
	std::vector<std::vector<double>> array;
	if (argc == 2) {
		std::ifstream file {argv[1]};
		array_input(array_size, array, file);
		file.close();	
	} else if (argc == 1) {
		array_input(array_size, array,
								[](const int i, const int j, const int n) -> double { return 1.0 / (2 * n - i - j - 1); });//1.0
	} else {
		std::cout << "Restart the programm with correct arguments!" << std::endl;
		return 1;
	}
	std::vector<std::vector<double>> eigen_vectors(array_size, std::vector<double>(array_size, 0));
	for (int i {0}; i < array_size; ++i) {
		eigen_vectors[i][i] = 1;
	}
	double epsilon;///////////////
	//main_func(array_size, array, eigen_vectors, epsilon);

	return 0;
}