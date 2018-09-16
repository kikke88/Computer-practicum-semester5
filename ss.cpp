#include <iostream>
#include <fstream>
#include <functional>
#include <cmath>
#include <utility>
#include <cfloat>

void x_print(const int size, const std::vector<double>& x, std::ostream& stream, const std::vector<int>& indexes) {
	stream << "<<x>>" << std::endl;
	for (int i {0}; i < size; ++i) {
		stream << "x[" << i << "]= " << x[indexes[i]] << std::endl;;
	}
	stream << std::endl;
}

void L_U_print(const int size, const std::vector<std::vector<double>>& arr, std::ostream& stream, const std::vector<int>& indexes) {
	stream << "<<L>>" << std::endl;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			if (i < j) {
				stream << 0.0 << "\t";
			} else {
				stream << arr[indexes[i]][j] << "\t";
			}
		}
		stream << std::endl;
	}
	stream << "<<U>>" << std::endl;
	for(int i {0}; i < size; ++i) {
		for(int j {0}; j < size; ++j) {
			if (i > j) {
				stream << 0.0 << "\t";
			} else if (i == j) {
				stream << 1.0 << "\t";
			} else {
				stream << arr[indexes[i]][j] << "\t";
			}
		}
		stream << std::endl;
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

void max_column(const int size, const int line, const int column,
								std::vector<int>& indexes, const std::vector<std::vector<double>>& arr) {
	double max {std::abs(arr[indexes[line]][column])};
	int line_of_max = line;
	for (int i = line + 1; i < size; ++i) {
		double cur_num {std::abs(arr[indexes[i]][column])};
		if (cur_num > max) {
			max = cur_num;
			line_of_max = i;
		}
	}
	if (max < DBL_EPSILON) {
		int ERROR = 1;
		throw ERROR;
	}
	if (line_of_max != line) {
		std::swap(indexes[line], indexes[line_of_max]);	
	}
}

std::vector<int> main_func(const int size, std::vector<std::vector<double>>& arr, const std::vector<double>& b_arr,
								std::vector<double>& x_arr) { 	
	std::vector<int> indexes;
	for(int i{0}; i < size; ++i) {
		indexes.push_back(i);
	}
	max_column(size, 0, 0, indexes, arr);
	for (int counter {0}; counter < size - 1;) {
		if (counter == 0) {//Uik
			for (int j {1}; j < size; ++j) {
				arr[indexes[0]][j] = arr[indexes[0]][j] / arr[indexes[0]][0];
			}		
		} else {
			int i = counter;
			for (int k {i + 1}; k < size; ++k) {
				double sum {0};
				for (int j {0}; j < i; ++j) {
					sum = sum + arr[indexes[i]][j] * arr[indexes[j]][k];
				}
				arr[indexes[i]][k] = (arr[indexes[i]][k] - sum) / arr[indexes[i]][i];
			}
		}
		++counter;
		if(counter != size - 1) {
		max_column(size, counter, counter, indexes, arr);	
		}
		int k = counter;//Lik
		for (int i {k}; i < size; ++i) {
			double sum {0};
			for (int j {0}; j < k; ++j) {
				sum = sum + arr[indexes[i]][j] * arr[indexes[j]][k];
			}
			arr[indexes[i]][k] = arr[indexes[i]][k] - sum;
		}
	}
	std::vector<double> y (size, 0);
	for (int i {0}; i < size; ++i)	{
		double sum {0};
		for (int j {0}; j < i; ++j) {
			sum = sum + y[indexes[j]] * arr[indexes[i]][j];
		}
		y[indexes[i]] = (b_arr[indexes[i]] - sum) / arr[indexes[i]][i];
	}
	for (int i {size - 1}; i >= 0; --i)	{
		double sum {0};
		for (int j {i + 1}; j < size; ++j) {
			sum = sum + x_arr[indexes[j]] * arr[indexes[i]][j];
		}
		x_arr[indexes[i]] = y[indexes[i]] - sum;
	}
	return indexes;
}

double residual_calculation(const int size, const std::vector<std::vector<double>>& arr,
								std::vector<double>& x_arr, std::vector<double>& b_arr, const std::vector<int>& indexes) {
	
	for (int i {0}; i < size; ++i) {
		double sum {x_arr[indexes[i]]};
		for (int j{i + 1}; j < size; ++j) {
			sum = sum + x_arr[indexes[j]] * arr[indexes[i]][j];
		}
		x_arr[indexes[i]] = sum;
	}
	for (int i {0}; i < size; ++i) {
		double sum {0};
		for (int j{0}; j < i + 1; ++j) {
			sum = sum + x_arr[indexes[j]] * arr[indexes[i]][j];
		}
		b_arr[indexes[i]] = std::pow(sum - b_arr[indexes[i]], 2);
	}
	double res {0};
	for (int i {0}; i < size; ++i) {
		res = res + b_arr[indexes[i]];
	}
	res = std::sqrt(res);
	return res;
}

void result_output(const int size, const std::vector<std::vector<double>>& arr, std::vector<double>& x_arr,
								std::vector<double>& b_arr, const std::vector<int>& indexes, bool bigsize) {	
	if (bigsize) {
		std::ofstream result_file {"result.txt"};
		L_U_print(size, arr, result_file, indexes);
		x_print(size, x_arr, result_file, indexes);
		result_file << residual_calculation(size, arr, x_arr, b_arr, indexes) << std::endl;
		result_file.close();
	} else {
		L_U_print(size, arr, std::cout, indexes);
		x_print(size, x_arr, std::cout, indexes);
		std::cout << std::scientific << residual_calculation(size, arr, x_arr, b_arr, indexes) << std::endl;
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
		array_input(array_size, array, b_array, [](const int i, const int j, const int n) -> double { return (i + j + 1) / n; });//1.0
	}
    std::vector<double> x (array_size, 0);
    std::vector<int> indexes;
//    std::vector<int> indexes(main_func(array_size, array, b_array, x));
    try {
//    indexes = std::move(main_func(array_size, array, b_array, x));     	
    	indexes = main_func(array_size, array, b_array, x);
    } catch (const int) {
    	std::cout << "degenerate matrix" << std::endl;
    	return 1;
    }
	if (array_size <= 10) {
		result_output(array_size, array, x, b_array, indexes, 0);
	} else {
		result_output(array_size, array, x, b_array, indexes, 1);
	}
	return 0;
}