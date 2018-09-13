#include <iostream>
#include <fstream>
#include <functional>
#include <cmath>

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

void two_dimensianal_array_print(const int size, const std::vector<std::vector<double>>& arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for(const std::vector<double>& sub_array : arr) {
		for(const double num: sub_array) {
			std::cout << num << "\t";
		}
	std::cout << std::endl;
	}
}

void two_dimensianal_array_print(const int size, const double* const* arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			stream << arr[i][j] << "\t";
		}
		stream << std::endl;
	}
}

void one_dimensianal_array_print(const int size, const std::vector<double>& arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for(double num : arr) {
		std::cout << num << "\t";
	}
	std::cout << std::endl;
}

void one_dimensianal_array_print(const int size, const double* arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for (int i {0}; i < size; ++i) {
		stream << arr[i] << std::endl;
	}
}

void main_func(const int size, std::vector<std::vector<double>>& arr, double*** L,
				double*** U, const std::vector<double>& b_arr, double** x_arr) { 	
	for (int i {0}; i < size; ++i) {
		(*L)[i][0] = arr[i][0];
	}
	for (int counter {0}; counter < size - 1;) {
		if (counter == 0) {//Uik
			for (int j {1}; j < size; ++j) {
				(*U)[0][j] = arr[0][j] / (*L)[0][0];
			}				
		} else {
			int i = counter;
			for(int k {i + 1}; k < size; ++k) {
				double sum {0};
				for (int j {0}; j < i; ++j) {
					sum = sum + (*L)[i][j] * (*U)[j][k];
				}
				(*U)[i][k] = (arr[i][k] - sum) / (*L)[i][i];
			}
		}
		++counter;
		int k = counter;//Lik
		for (int i {k}; i < size; ++i) {
			double sum {0};
			for (int j {0}; j < k; ++j) {
				sum = sum + (*L)[i][j] * (*U)[j][k];
			}
			(*L)[i][k] = arr[i][k] - sum;
		}
	}
	for (int i {0}; i < size; ++i) {
		(*U)[i][i] = 1;
	}
	double* y = new double [size]();
	for (int i {0}; i < size; ++i)	{
		double sum {0};
		for (int j {0}; j < i; ++j) {
			sum = sum + y[j] * (*L)[i][j];
		}
		y[i] = (b_arr[i] - sum) / (*L)[i][i];
	}
	for (int i {size - 1}; i >= 0; --i)	{
		double sum {0};
		for (int j {i + 1}; j < size; ++j) {
			sum = sum + (*x_arr)[j] * (*U)[i][j];
		}
		(*x_arr)[i] = y[i] - sum;
	}
	delete[] y;
}

double residual_calculation(const int size, const double* const* L, const double* const* U, double* x_arr, std::vector<double>& b_arr) {
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
	return res;
}

void result_output(const int size, const double* const* L, const double* const* U,
					double* x_arr, std::vector<double>& b_arr, bool bigsize) {	
	if (bigsize) {
		std::ofstream result_file {"result.txt"};
		two_dimensianal_array_print(size, L, result_file);
		two_dimensianal_array_print(size, U, result_file);
		one_dimensianal_array_print(size, x_arr, result_file);
		result_file << residual_calculation(size, L, U, x_arr, b_arr) << std::endl;
		result_file.close();
	} else {
		two_dimensianal_array_print(size, L, std::cout);
		two_dimensianal_array_print(size, U, std::cout);
		one_dimensianal_array_print(size, x_arr, std::cout);
		std::cout << residual_calculation(size, L, U, x_arr, b_arr) << std::endl;
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
	double** L {nullptr};
	double** U {nullptr};
    double* x {nullptr};
	L = new double* [array_size];
	for (int i {0}; i < array_size; ++i) {
		L[i] = new double[array_size]();
	}
	U = new double* [array_size];
	for (int i {0}; i < array_size; ++i) {
		U[i] = new double[array_size]();
	}
    x = new double[array_size]();
    main_func(array_size, array, &L, &U, b_array, &x); 
	if (array_size <= 10) {
		result_output(array_size, L, U, x, b_array, 0);
	} else {
		result_output(array_size, L, U, x, b_array, 1);
	}
	for (int i {0}; i < array_size; ++i) {
		delete[] L[i];
	}
	delete[] L;
	for (int i {0}; i < array_size; ++i) {
		delete[] U[i];
	}
	delete[] U;
    delete[] x;
	return 0;
}