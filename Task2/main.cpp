#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>


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
	std::cout << "Enter the array size." << std::endl;
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

void save_lines_colums(int size, int line, int column,
								std::vector<std::vector<double>>& arr,
								std::vector<double>& i_l, std::vector<double>& i_c,
								std::vector<double>& j_l, std::vector<double>& j_c)
{
	for (int k {0}; k < size; ++k) {
		i_l[k] = arr[line][k];
		j_l[k] = arr[column][k];
		i_c[k] = arr[k][line];
		j_c[k] = arr[k][column];
	}
}

void offdiagonal_recomputation(int size, double& old_offdiagonal, int line, int column,
								std::vector<std::vector<double>>& arr,
								std::vector<double>& i_l, std::vector<double>& i_c,
								std::vector<double>& j_l, std::vector<double>& j_c)
{
	for (int k {0}; k < size; ++k) {
		if (k != line) {
			old_offdiagonal += std::pow(arr[line][k], 2) - std::pow(i_l[k], 2) + std::pow(arr[k][line], 2) - std::pow(i_c[k], 2);
		}
	}
	for (int k {0}; k < size; ++k) {
		if (k != line && k != column) {
			old_offdiagonal += std::pow(arr[column][k], 2) - std::pow(j_l[k], 2) + std::pow(arr[k][column], 2) - std::pow(j_c[k], 2);
		}
	}
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

void multiplication(int size, int line, int column, double& offdiagonal,
								std::vector<std::vector<double>>& arr, std::vector<std::vector<double>>& evec,
								std::vector<double>& i_l, std::vector<double>& i_c,
								std::vector<double>& j_l, std::vector<double>& j_c)
{
	save_lines_colums(size, line, column, arr, i_l, i_c, j_l, j_c);
	double cos_fi, sin_fi, pi {std::acos(-1)};
	if (arr[line][line] == arr[column][column]) {
		cos_fi = sin_fi = sin(pi / 4);
	} else {
		double x = (-1) * 2 * arr[line][column];
		double y = arr[line][line] - arr[column][column];
		cos_fi = std::sqrt((1 + std::abs(y) / std::sqrt(std::pow(x, 2) + std::pow(y, 2))) / 2);
		sin_fi = std::abs(x) * sgn(x * y) / (2 * cos_fi * std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
	}

	double tmp1, tmp2;
	for (int k {0}; k < size; ++k) {
		tmp1 = evec[k][line];
		tmp2 = evec[k][column];
		evec[k][line] = tmp1 * cos_fi + tmp2 * sin_fi;
		evec[k][column] = tmp1 * (-sin_fi) + tmp2 * cos_fi;
	}
	for (int k {0}; k < size; ++k) {
		tmp1 = arr[k][line];
		tmp2 = arr[k][column];
		arr[k][line] = tmp1 * cos_fi + tmp2 * (-sin_fi);
		arr[k][column] = tmp1 * sin_fi + tmp2 * cos_fi;
	}
	for (int k {0}; k < size; ++k) {
		tmp1 = arr[line][k];
		tmp2 = arr[column][k];
		arr[line][k] = tmp1 * cos_fi + tmp2 * (-sin_fi);
		arr[column][k] = tmp1 * sin_fi + tmp2 * cos_fi;
	}
	offdiagonal_recomputation(size, offdiagonal, line, column, arr, i_l, i_c, j_l, j_c);
}

void main_func(int size, std::vector<std::vector<double>>& arr, std::vector<std::vector<double>>& evec, double eps)
{	
	int iteration {0};
	double offdiagonal;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			if (i != j) {
				offdiagonal += std::pow(arr[i][j], 2);
			}
		}
	}
	int strategy;
	std::cout << "Enter the number of the strategy 1 / 2 / 3." << std::endl;
	std::cin >> strategy;
	std::vector<double> i_line(size), i_column(size), j_line(size), j_column(size);//сохранять перед умножением
	switch (strategy)
	{
		case 1:
			{
				while (offdiagonal >= eps) {
					std::cout << eps << " |||||| " << offdiagonal << std::endl;
					++iteration;

					//1
					int line {0}, column {1};
					double tmp_max = std::abs(arr[line][column]);
					for (int i {0}; i < size; ++i) {
						for (int j {i + 1}; j < size; ++j) {///симметрия
							if (tmp_max < std::abs(arr[i][j])) {
								tmp_max = std::abs(arr[i][j]);
								line = i;
								column = j;
							}
						}
					}
					//1
					multiplication(size, line, column, offdiagonal, arr, evec, i_line, i_column, j_line, j_column);
				}			
				break;
			}	
		case 2:
			{
				if (offdiagonal < eps) {
					break;
				}
				std::vector<double> sum_lines(size, 0);
				for (int i {0}; i < size; ++i) {
					for (int j {0}; j < size; ++j) {
						if (i != j) {
							sum_lines[i] += std::pow(arr[i][j], 2);
						}
					}
				}
				int line, column;//function
				while (offdiagonal >= eps) {	
					++iteration;

					//2
					if (iteration > 1) {
						sum_lines[line] = 0;
						sum_lines[column] = 0;
						for (int j {0}; j < size; ++j) {
							if (j == line) {
								sum_lines[column] += std::pow(arr[column][j], 2);
							} else if (j == column) {
								sum_lines[line] += std::pow(arr[line][j], 2);
							} else {
								sum_lines[line] += std::pow(arr[line][j], 2);
								sum_lines[column] += std::pow(arr[column][j], 2);
							}
						}
						for (int k {0}; k < size; ++k) {
							if (k != line && k != column) {
								sum_lines[k] += std::pow(arr[k][line], 2) - std::pow(i_column[k], 2);//сохранить перед умножением
								sum_lines[k] += std::pow(arr[k][column], 2) - std::pow(j_column[k], 2); 
							}
						}	
					}

					line = 0;
					double max_line {sum_lines[line]};
					for (int i {1}; i < size; ++i) {
						if (max_line < sum_lines[i]) {
							max_line = sum_lines[i];
							line = i;
						}
					}
					double max_value_in_max_line;
					if (line != 0) {
						column = 0;
						max_value_in_max_line = std::abs(arr[line][0]);
					} else {
						column = 1;
						max_value_in_max_line = std::abs(arr[line][1]);
					}
					for (int j {0}; j < size; ++j) {
						if (j != line) {
							if (max_value_in_max_line < std::abs(arr[line][j])) {
								max_value_in_max_line = std::abs(arr[line][j]);
								column = j;
							}
						}
					}
					//2
					multiplication(size, line, column, offdiagonal, arr, evec, i_line, i_column, j_line, j_column);
				}
				break;
			}
		case 3:
			{
				for (int line {0}; line < size; ++line) {
					for (int column {line + 1}; column < size; ++column) {
						while (offdiagonal >= eps) {
						++iteration;

						multiplication(size, line, column, offdiagonal, arr, evec, i_line, i_column, j_line, j_column);
						}
						goto END;		
					}
				}
END:
				break;
			}
		default:
			{
			//error
			break;
			}
	}
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			std::cout << arr[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "++++++++++++++++++++++" << std::endl;
	for (int i {0}; i < size; ++i) {
		for (int j {0}; j < size; ++j) {
			std::cout << evec[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "iteration " << iteration << std::endl;

}

int main(int argc, char* argv[]) //проставить, где надо const
{
	std::cout.precision(4);
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
	for (int i {0}; i <array_size; ++i) {
		for (int j {0}; j < array_size; ++j) {
			std::cout << array[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::vector<std::vector<double>> eigen_vectors(array_size, std::vector<double>(array_size, 0));
	for (int i {0}; i < array_size; ++i) {
		eigen_vectors[i][i] = 1;
	}
	double epsilon = 1.0/10000;
	main_func(array_size, array, eigen_vectors, epsilon);

	return 0;
}