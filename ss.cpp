#include <iostream>
#include <fstream>
#include <functional>
#include <cmath>
#include <utility>

void one_dimensianal_array_print(const int size, const std::vector<double>& arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for(double num : arr) {
		std::cout << num << "\t";
	}
	std::cout << std::endl;
}

void two_dimensianal_array_print(const int size, const std::vector<std::vector<double>>& arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for(const std::vector<double>& sub_array : arr) {
		for(const double num: sub_array) {
			std::cout << num << "\t";
		}
	std::cout << std::endl;
	}
}

void b_input(const int size, const std::vector<std::vector<double>>& arr, std::vector<double>& b_arr, const int method) {
	double sum {0};
	switch (method) {
		case 0: {
			for (int i {0}; i < size; ++i) {
				for (int j {0}; j < size; ++j) {
					sum = sum + arr[i][j];
				}
			b_arr.push_back(sum);
			sum = 0;
			}
		break;
		}
		case 1: {
			for (int i {0}; i < size; ++i) {
				for (int j {1}; j < size; j = j + 2) {
					sum = sum + arr[i][j];
				}
			b_arr.push_back(sum);
			sum = 0;
			}
		break;
		}
		case 2: {
			for (int i {0}; i < size; ++i) {
				for (int j {0}; j < size; j = j + 2) {
					sum = sum + arr[i][j];
				}
			b_arr.push_back(sum);
			sum = 0;
			}
		break;
		}
		default: {
			///error
		}
	}	
}

void array_input(int& size, std::vector<std::vector<double>>& arr, std::vector<double>& b_arr, std::ifstream& file) {
	file >> size;
	double tmp;
	for (int i {0}; i < size; ++i) {
		arr.push_back(std::vector<double>());
		for (int j {0}; j < size; ++j) {
			file >> tmp;
			arr[i].push_back(tmp);
		}
		file >> tmp;
		b_arr.push_back(tmp);
	}
}

void array_input(int& size, std::vector<std::vector<double>>& arr, std::vector<double>& b_arr,
				std::function<double(const int, const int, const int)> func) {
	std::cin >> size;
	double tmp;
	for (int i {0}; i < size; ++i) {
		arr.push_back(std::vector<double>());
		for (int j {0}; j < size; ++j) {
			tmp = func(i, j, size);
			arr[i].push_back(tmp);
		}
	}
	int method;
	std::cin >> method; 
	b_input(size, arr, b_arr, method);	
}

void max_in_column(const int size, const int from_index, const int in_column,
								std::vector<int>& indexes, const std::vector<std::vector<double>>& arr) {
	double max {std::abs(arr[indexes[from_index]][in_column])};
	int index_of_max = from_index;
	for (int i = from_index + 1; i < size; ++i) {
		double cur_num {std::abs(arr[i][in_column])};
		if (cur_num > max) {
			max = cur_num;
			index_of_max = i;
		}
	}
	if (index_of_max != from_index) {
		std::swap(indexes[from_index], indexes[index_of_max]);	
	}
}

std::vector<int> main_func(const int size, std::vector<std::vector<double>>& arr, const std::vector<double>& b_arr, std::vector<double>& x_arr) { 	
	std::vector<int> indexes{};
	for(int i{0}; i < size; ++i) {
		indexes.push_back(i);
	}
	
	for (int counter {0}; counter < size - 1;) {
		if (counter == 0) {//Uik
			for (int j {1}; j < size; ++j) {
				arr[0][j] = arr[0][j] / arr[0][0];
			}				
		} else {
			int i = counter;
			for(int k {i + 1}; k < size; ++k) {
				double sum {0};
				for (int j {0}; j < i; ++j) {
					sum = sum + arr[i][j] * arr[j][k];
				}
				//eps
				arr[i][k] = (arr[i][k] - sum) / arr[i][i];
			}
		}
		++counter;
		int k = counter;//Lik
		for (int i {k}; i < size; ++i) {
			double sum {0};
			for (int j {0}; j < k; ++j) {
				sum = sum + arr[i][j] * arr[j][k];
			}
			arr[i][k] = arr[i][k] - sum;
		}
	}
	std::vector<double> y (size, 0);
	for (int i {0}; i < size; ++i)	{
		double sum {0};
		for (int j {0}; j < i; ++j) {
			sum = sum + y[j] * arr[i][j];
		}
		y[i] = (b_arr[i] - sum) / arr[i][i];
	}
	for (int i {size - 1}; i >= 0; --i)	{
		double sum {0};
		for (int j {i + 1}; j < size; ++j) {
			sum = sum + x_arr[j] * arr[i][j];
		}
		x_arr[i] = y[i] - sum;
	}
	return indexes;
}

double residual_calculation(const int size, const std::vector<std::vector<double>>& arr,
								std::vector<double>& x_arr, std::vector<double>& b_arr) {
	/*
	for (int i {0}; i < size; ++i) {
		double sum {x_arr[i]};
		for (int j{i + 1}; j < size; ++j) {
			sum = sum + x_arr[j] * U[i][j];
		}
		x_arr[i] = sum;
	}
	for (int i {0}; i < size; ++i) {
		double sum {0};
		for (int j{0}; j < i + 1; ++j) {
			sum = sum + x_arr[j] * L[i][j];
		}
		b_arr[i] = pow(sum - b_arr[i], 2);
	}
	double res {0};
	for (int i {0}; i < size; ++i)
	{
		res = res + b_arr[i];
	}
	res = sqrt(res);
	*/
	return 0;
}

void result_output(const int size, const std::vector<std::vector<double>>& arr, std::vector<double>& x_arr, std::vector<double>& b_arr, bool bigsize) {	
	if (bigsize) {
		std::ofstream result_file {"result.txt"};
		//two_dimensianal_array_print(size, L, result_file);
		//two_dimensianal_array_print(size, U, result_file);
		one_dimensianal_array_print(size, x_arr, result_file);
		//result_file << residual_calculation(size, L, U, x_arr, b_arr) << std::endl;
		result_file.close();
	} else {
		//two_dimensianal_array_print(size, L, std::cout);
		//two_dimensianal_array_print(size, U, std::cout);
		one_dimensianal_array_print(size, x_arr, std::cout);
		//std::cout << residual_calculation(size, L, U, x_arr, b_arr) << std::endl;
	}
}

int main(int argc, char* argv[]) {
	int array_size;
	std::vector<std::vector<double>> array;
	std::vector<double> b_array;
	if (argc > 1) {
		std::ifstream file {argv[1]};
		array_input(array_size, array, b_array, file);
		file.close();	
	} else {
		array_input(array_size, array, b_array, [](const int i, const int j, const int n) -> double { return (i + j + 1) / n; });
	}
    std::vector<double> x (array_size, 0);
    main_func(array_size, array, b_array, x); 
    two_dimensianal_array_print(array_size, array, std::cout);
    one_dimensianal_array_print(array_size, x, std::cout);
/*
	if (array_size <= 10) {
		result_output(array_size, array, x, b_array, 0);
	} else {
		result_output(array_size, array, x, b_array, 1);
	}
*/
	return 0;
}