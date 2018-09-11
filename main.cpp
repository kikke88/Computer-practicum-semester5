#include <iostream>
#include <fstream>
#include <functional>

void b_input(const int size, double** arr, double** b_arr, int method) { // const double **
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
			///ругаться об ошибке
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

void array_input(int& size, double*** arr, double** b_arr, std::function<double(int, int)> func) {
	std::cin >> size;

	*arr = new double* [size];
	for (int i = 0; i < size; ++i) {
		(*arr)[i] = new double[size]();/////////////////////////////
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

void array_print(int size, double ** arr) {
	std::cout << "____________________________" << std::endl;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			std::cout << arr[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void main_func(const int size, const double** arr, /*потом убрать следующие два аргумента */double*** L, double*** U, const double* b_arr, double** x_arr,) {
 	
    
    //algorithm realisation
	for (int i {0}; i < size; ++i) {///потом этот фор можно будет совсем убрать
		(*L)[i][0] = arr[i][0];
	}
	
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
				//tmp1
				for (int j {0}; j < k; ++j) {
					sum = sum + (*L)[i][j] * (*U)[j][k];
				}
				(*L)[i][k] = arr[i][k] - sum;
			}
		}
	}
 	array_print(array_size, L);
	array_print(array_size, U);
    ///
    ////
    ///
}


int main(int argc, char* argv[]) {
	int array_size {0};///мб убрать инициализацию
	double** array {nullptr};
	double* b_array {nullptr};
	
	if (argc > 1) {
		std::ifstream file {argv[1]};
		array_input(array_size, &array, &b_array, file);
		file.close();	
	} else {
		array_input(array_size, &array, &b_array, [](int i, int j) -> double { return i + j + 1; });
	}

	array_print(array_size, array);

	std::cout << "____________________________" << std::endl;
	for (int j {0}; j < array_size; ++j) {
			std::cout << b_array[j] << "\t";
	}
	std::cout << std::endl;

	double** L {nullptr};
	double** U {nullptr};
    double* x{nullptr};

	L = new double* [array_size];
	for (int i {0}; i < array_size; ++i) {
		L[i] = new double[array_size]();
	}

	U = new double* [array_size];//////////////
	for (int i {0}; i < array_size; ++i) {
		U[i] = new double[array_size]();
	}
    x = new double[array_size]();

	//	array_print(array_size, L);
	//	array_print(array_size, U);
    main_func(array_size, array, &L, &U, b_array, &x); 
	array_print(array_size, L);
	array_print(array_size, U);




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
	/////
    delete[] x;
	delete[] b_array;
	//////

	return 0;
}
