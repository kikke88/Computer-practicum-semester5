#include <iostream>
#include <fstream>
#include <functional>

void b_input(int size, double** arr, double** b_arr, int method) {
	double sum {0};
	switch (method) {
		
		case 0: {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; ++j) {
					sum = sum + arr[i][j];
				}
			(*b_arr)[i] = sum;
			sum = 0;
			}
		break;
		}

		case 1: {
			for (int i = 0; i < size; ++i) {
				for (int j = 1; j < size; j = j + 2) {
					sum = sum + arr[i][j];
				}
			(*b_arr)[i] = sum;
			sum = 0;
			}
		break;
		}

		case 2: {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < size; j = j + 2) {
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
	*b_arr = new double[size];

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			(*arr)[i][j] = func(i, j);
		}
	}
	int method;
	std::cin >> method; 
	for (int i = 0; i < size; ++i) {

		b_input(size, *arr, b_arr, method);
		//(*b_arr)[i] = 0;///////////////////////////////////////
	}
}
void arr_pr(int size, double ** arr) {
	std::cout << "____________________________" << std::endl;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			std::cout << arr[i][j] << "\t";
		}
		std::cout << std::endl;
	}
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
		array_input(array_size, &array, &b_array, [](int i, int j) -> double { return i * j; });
	}
	for (int i {0}; i < array_size; ++i) {
		for (int j {0}; j < array_size; ++j) {
			std::cout << array[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "____________________________" << std::endl;
	for (int j {0}; j < array_size; ++j) {
			std::cout << b_array[j] << "\t";
	}
	std::cout << std::endl;
	double** L = nullptr;
	double** U = nullptr;

	L = new double* [array_size];/////////////////////////нет инициализации нулями
	for (int i {0}; i < array_size; ++i) {
		L[i] = new double[array_size]();
	}

	U = new double* [array_size];//////////////
	for (int i {0}; i < array_size; ++i) {
		U[i] = new double[array_size]();
	}
	arr_pr(array_size, L);
	arr_pr(array_size, U);
	bool zzz;
	std::cin >>zzz;
	//algorithm realisation
	for (int i {0}; i < array_size; ++i) {///потом этот фор можно будет совсем убрать
		L[i][0] = array[i][0];
	}
	for (int j {0}; j < array_size; ++j) {
		U[0][j] = array[0][j] / L[0][0];
	}
	arr_pr(array_size, L);
	arr_pr(array_size, U);
	//tmp
	for (int counter {1}; counter < array_size - 1; ++counter) {
		{//Lik
			int k = counter;
			for (int i {k}; i < array_size; ++i) {
				double sum {0};
				//tmp1
				for (int j {0}; j < k; ++j) {
					sum = sum + L[i][j] * U[j][k];
				}
				L[i][k] = array[i][k] - sum;
			}
		}
		{//Uik
			int i = counter;
			for(int k {i + 1}; k < array_size; ++k) {
				double sum {0};
				//tmp2
				for (int j {0}; j < i; ++j) {
					sum = sum + L[i][j] * U[j][k];
				}
				U[i][k] = (array[i][k] - sum) / L[i][i];
			}
		}
	}
	int k = array_size - 1;
	int i {k};
	double sum {0};
	//tmp1
	for (int j {0}; j < k; ++j) {
		sum = sum + L[i][j] * U[j][k];
	}
	L[i][k] = array[i][k] - sum;
	arr_pr(array_size, L);
	arr_pr(array_size, U);




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
	delete[] b_array;
	//////

	return 0;
}