#include <iostream>
#include <fstream>
#include <functional>

void b_input(const int size, const double* const* arr, double** b_arr, const int method) { // const double **
	double sum {0};
	switch (method) {
		case 0: {
			for (int i {0}; i < size; ++i) {
				for (int j {0}; j < size; ++j) {
					sum = sum + arr[i][j];
				}
			(*b_arr)[i] = sum;
			sum = 0;
			}
		break;
		}
		case 1: {
			for (int i {0}; i < size; ++i) {
				for (int j {1}; j < size; j = j + 2) {
					sum = sum + arr[i][j];
				}
			(*b_arr)[i] = sum;
			sum = 0;
			}
		break;
		}
		case 2: {
			for (int i {0}; i < size; ++i) {
				for (int j {0}; j < size; j = j + 2) {
					sum = sum + arr[i][j];
				}
			(*b_arr)[i] = sum;
			sum = 0;
			}
		break;
		}
		default: {
			///error
		}
	}	
}

void array_input(int& size, double*** arr, double** b_arr, std::ifstream& file) {
	file >> size;
	*arr = new double* [size];
	for (int i {0}; i < size; ++i) {
		(*arr)[i] = new double[size];
	}
	*b_arr = new double[size];
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			file >> (*arr)[i][j];
		}
		file >> (*b_arr)[i];
	}
}

void array_input(int& size, double*** arr, double** b_arr, std::function<double(const int, const int)> func) {
	std::cin >> size;
	*arr = new double* [size];
	for (int i = 0; i < size; ++i) {
		(*arr)[i] = new double[size]();
	}
	*b_arr = new double[size]();
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			(*arr)[i][j] = func(i, j);
		}
	}
	int method;
	std::cin >> method; 
	b_input(size, *arr, b_arr, method);	
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

void one_dimensianal_array_print(const int size, const double* arr, std::ostream& stream) {
	stream << "____________________________" << std::endl;
	for (int i {0}; i < size; ++i) {
		stream << arr[i] << "\t";
	}
	stream << std::endl;
}
//const
void main_func(const int size, const double* const* arr, double*** L,
				double*** U, const double* b_arr, double** x_arr) { 	
    //algorithm realisation
	for (int i {0}; i < size; ++i) {///
		(*L)[i][0] = arr[i][0];
	}
	//sum
	for (int counter {0}; counter < size - 1;) {
		{//Uik
			if (counter == 0) {
				for (int j {1}; j < size; ++j) {/////
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
		}
		++counter;
		{//Lik
			int k = counter;
			for (int i {k}; i < size; ++i) {
				double sum {0};
				for (int j {0}; j < k; ++j) {
					sum = sum + (*L)[i][j] * (*U)[j][k];
				}
				(*L)[i][k] = arr[i][k] - sum;
			}
		}
	}
	for (int i {0}; i < size; ++i) {//
		(*U)[i][i] = 1;
	}
	double* y = new double [size]();
	//sum
	for (int i {0}; i < size; ++i)	{
		double sum {0};
		for (int j {0}; j < i; ++j) {
			sum = sum + y[j] * (*L)[i][j];
		}
		y[i] = (b_arr[i] - sum) / (*L)[i][i];
	}
	//sum
	for (int i {size - 1}; i >= 0; --i)	{
		double sum {0};
		for (int j {i + 1}; j < size; ++j) {
			sum = sum + (*x_arr)[j] * (*U)[i][j];
		}
		(*x_arr)[i] = y[i] - sum;
	}
	delete[] y;
}

void result_output(const int size, const double* const* L, const double* const* U,
					const double* x_arr, const double* b_arr, bool bigsize) {

	
	if (bigsize) {
		std::ofstream result_file {"result.txt"};
		two_dimensianal_array_print(size, L, result_file);
		two_dimensianal_array_print(size, U, result_file);
		one_dimensianal_array_print(size, x_arr, result_file);
	} else {
		two_dimensianal_array_print(size, L, std::cout);
		two_dimensianal_array_print(size, U, std::cout);
		one_dimensianal_array_print(size, x_arr, std::cout);
	}
	//невязка
}


int main(int argc, char* argv[]) {
	int array_size {0};
	double** array {nullptr};
	double* b_array {nullptr};
	if (argc > 1) {
		std::ifstream file {argv[1]};
		array_input(array_size, &array, &b_array, file);
		file.close();	
	} else {
		array_input(array_size, &array, &b_array, [](const int i, const int j) -> double { return i + j + 1; });
	}
	two_dimensianal_array_print(array_size, array, std::cout);
	one_dimensianal_array_print(array_size, b_array, std::cout);
	double** L {nullptr};
	double** U {nullptr};
    double* x {nullptr};
	L = new double* [array_size];
	for (int i {0}; i < array_size; ++i) {
		L[i] = new double[array_size]();
	}
	U = new double* [array_size];//////////////
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
	//////////Очистка памяти
	for (int i {0}; i < array_size; ++i) {
		delete[] array[i];
	}
	delete[] array;
	for (int i {0}; i < array_size; ++i) {
		delete[] L[i];
	}
	delete[] L;
	for (int i {0}; i < array_size; ++i) {
		delete[] U[i];
	}
	delete[] U;
    delete[] x;
	delete[] b_array;
	return 0;
}