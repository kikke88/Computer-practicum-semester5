#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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

double offdiagonal_recomputation(int size, double old_offdiagonal, int line, int column,
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
	return old_offdiagonal;
}

void multiplication(int size,)

void main_func(int size, std::vector<std::vector<double>>& arr, std::vector<std::vector<double>> evec,
								double eps, int strategy)
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

	std::vector<double> i_line(size), i_column(size), j_line(size), j_column(size);//сохранять перед умножением
	switch (strategy)
	{
		case 1:
			while (offdiagonal >= eps) {
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

				//mul
				save_lines_colums(size, line, column, arr, i_line, i_column, j_line, j_column);
				///
				offdiagonal = offdiagonal_recomputation(size, offdiagonal, line, column, arr, i_line, i_column, j_line, j_column);
				//mul

			}			
			break;	
		case 2:
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
			{	
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

					//mul
					save_lines_colums(size, line, column, arr, i_line, i_column, j_line, j_column);
					//
					offdiagonal = offdiagonal_recomputation(size, offdiagonal, line, column, arr, i_line, i_column, j_line, j_column);
					//mul
				}
			}
			break;
		case 3:
			for (int line {0}; line < size; ++line) {
				for (int column {line + 1}; column < size; ++column) {
					while (offdiagonal < eps) {
					++iteration;

					//mul
					save_lines_colums(size, line, column, arr, i_line, i_column, j_line, j_column);
					//
					offdiagonal = offdiagonal_recomputation(size, offdiagonal, line, column, arr, i_line, i_column, j_line, j_column);
					//mul
					}
					goto END;		
				}
			}
END:
			break;
		default:
			//error
			break;
	}
}

int main(int argc, char* argv[]) //проставить, где надо const
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