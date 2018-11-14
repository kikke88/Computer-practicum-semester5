#include <iostream>
#include <functional>
#include <deque>


class Heat_transfer_equation {
	
	std::function<double(const double, const double)> equation;
	double a {1.0};
	std::function<double(const double)> left/*t*/, right/*t*/, begin/*x*/;
	bool first_type_left, first_type_right;
	std::function<double(const double, const double)> f_function;
	int num_of_x_step {100};
	int num_of_time_step {10};
	double x_step {1.0 / num_of_x_step};
	double time_step {1.0 / num_of_time_step};
	std::deque<double> values_in_grid_nodes(num_of_x_step + 1);//////////	
	
	void boundary_value(int);
	void recording_data_to_file(std::ofstream& ofile)
public:
	Heat_transfer_equation(std::function<double(const double, const double)> _equation,
							std::function<double(const double)> _left, _right, _begin,
							bool _first_type_left, _first_type_right,
							std::function<double(const double, const double)> _f_function);
	void implicit_scheme();
	void explicit_scheme();
	void approximation();
	void graph();
};


//[](const int i, const int j, const int n) -> double { return 1.0 / (2 * n - i - j - 1); }
void Heat_transfer_equation::boundary_value(int time)
{
	if (first_type_left) {
		values_in_grid_nodes[0] = left(time);
	} else {
		values_in_grid_nodes[0] = (4 * values_in_grid_nodes[1] - values_in_grid_nodes[2] -
			2 * x_step * left(time_step * time)) / 3;
	}
	if (first_type_right) {
		values_in_grid_nodes[num_of_x_step] = right(time);
	} else {
		values_in_grid_nodes[num_of_x_step] = (2 * x_step * right(time_step * time) + 
			4 * values_in_grid_nodes[num_of_x_step - 1] - values_in_grid_nodes[num_of_x_step - 2]) / 3;
	}
}

void Heat_transfer_equation::recording_data_to_file(std::ofstream& ofile)
{
	double cur_x {0.0}
	for (int i {0}; i < num_of_x_step + 1; ++i) {
		ofile << cur_x << "\t" << values[i] << std::endl;
		cur_x += x_step;
	}
	ofile << std::endl << std::endl;
}

void Heat_transfer_equation::implicit_scheme()
{
	std::ofstream ofile("graph.data");
	for (int i {1}; i < num_of_x_step; ++i) {
		values_in_grid_nodes[i] = begin(x_step * i);
	}
	boundary_value(values_in_grid_nodes, 0);
	recording_data_to_file(ofile);
	for (int i {1}; i < num_of_time_step + 1; ++i) {
		for (int j {1}; i < num_of_x_step; ++j) {
			values_in_grid_nodes[j - 1] = values_in_grid_nodes[j] + 
			time_step * a * a * (values_in_grid_nodes[j + 1] - 2 * values_in_grid_nodes[j] + values_in_grid_nodes[j - 1]) / (x_step * x_step) +
			time_step * f_function(j, i - 1);
		}
		values_in_grid_nodes.push_front(0);
		values_in_grid_nodes.pop_back();
		boundary_value(values_in_grid_nodes, i);
		recording_data_to_file(ofile);
	}
}

void Heat_transfer_equation::explicit_scheme()
{
	double A {(a * a * time_step) / (x_step * x_step)},
		   B {A}, //{(a * a * time_step) / (x_step * x_step)},
		   C {1 + 2 * A};
	std::vector<double> alpha(num_of_x_step), beta(num_of_x_step);
	if (first_type_left) {
		alpha[1] = 0;
	} else {
		alpha[1] = 1;
	}
	for (int i = 0; i < num_of_time_step + 1; ++i) {
		if (first_type_left) {
			beta[1] = left(time_step * i);
		} else {
			beta[1] = -(x_step * left(time_step * i));
		}
		for (int j {1}; j <= num_of_x_step - 1; ++j) {
			alpha[j + 1] = B / (C - A * alpha[j]);
			beta[j + 1] = (-(f_function(x_step * j, time_step * i)) /***** * time_step  *****/ + A * B) / (C - A * alpha[j]); 
		}
		if (first_type_right) {
			values_in_grid_nodes[num_of_x_step] = right(time_step * i);	
		} else {
			values_in_grid_nodes[num_of_x_step] = (right(time_step * i) + beta[num_of_x_step]) / (1 - alpha[num_of_x_step]);
		}
		for (int j {num_of_x_step - 1}; j => 0; --j) {
			values_in_grid_nodes[j] = alpha[j + 1] * values_in_grid_nodes[j + 1] + beta[j + 1];
		}
		recording_data_to_file(ofile);
	}
}

void Heat_transfer_equation::approximation()
{
	double max1 {0.0}, sum1 {0.0}, cur_x {0.0},
			max2 {0.0}, sum2 {0.0}, cur_value1, cur_value2;
	for (int i {0}; i < num_of_x_step + 1; ++i) {
		cur_value1 = std::abs(equation(cur_x, 1) - values_in_grid_nodes[i]);
		cur_value2 = std::abs(values_in_grid_nodes[i]);
		if (cur_value1 > max1) {
			max1 = cur_value1;
		}
		if (cur_value2 > max2) {
			max2 = cur_value2;
		}
		sum1 += cur_value * cur_value;
		sum2 += equation(cur_x, 1) * equation(cur_x, 1);		
		cur_x += x_step;
	}
	sum1 = std::sqrt(x_step * sum1);
	sum2 = std::sqrt(x_step * sum2);
	std::cout << "Absolute accuracy" << std::endl << max1 << std::endl << sum1 << std::endl;
	std::cout << "Fractional accuracy" << std::endl << max1 / max2 << std::endl << sum1 / sum2 << std::endl;
}

void graph()
{
	std::ofstream omain("main.gn"), oplotter("plotter.gn");
	omain << "set xrange [0:1]\nset yrange [" << Vmin << " - 1; " << Vmax << " + 1]\niter 0\nload\"plotter.gn\""////////////////////////////////////////////////////////////////
	oplotter << "iter = iter + 1\nplot \"graph.data\" index iter u 1:2 w l\npause 0,25\nif (iter < " << num_of_time_step << ") reread\n";

}
int main(int argc, char* argv[]) {
	



	return 0;
}