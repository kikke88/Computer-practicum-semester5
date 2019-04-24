#include <iostream>
#include <functional>
#include <cmath>
#include <fstream>
#include <vector>
#include <deque>



class Heat_transfer_equation {
	std::function<double(const double, const double)> equation;
	double a {1.0};
	std::function<double(const double)> left, right, begin;
	bool first_type_left, first_type_right;
	std::function<double(const double, const double)> f_function;
	int num_of_x_step {100}, num_of_time_step {40000};//100 40000
	int counter;
	int devider {10};
	double time_end, x_step {1.0 / num_of_x_step}, time_step {time_end / num_of_time_step};
	std::deque<double> values_in_grid_nodes;
	double V_min, V_max;
	void min_max();
	void boundary_value(int);
	void recording_data_to_file(std::ofstream&);
public:
	Heat_transfer_equation(std::function<double(const double, const double)>,
							std::function<double(const double)>,
							std::function<double(const double)>,
							std::function<double(const double)>,
							bool, bool,
							std::function<double(const double, const double)>,
							double, int, int);
	void explicit_scheme();
	void implicit_scheme();
	void approximation();
	void graph();
};

Heat_transfer_equation::Heat_transfer_equation(
							std::function<double(const double, const double)> _equation,
							std::function<double(const double)> _left,
							std::function<double(const double)> _right,
							std::function<double(const double)>_begin,
							bool _first_type_left, bool _first_type_right,
							std::function<double(const double, const double)> _f_function,
                            double _time_end, int _num_of_x_step, int _num_of_time_step):
							equation {_equation}, left {_left}, right {_right}, begin {_begin},
							first_type_left {_first_type_left}, first_type_right {_first_type_right},
							f_function {_f_function}, values_in_grid_nodes (num_of_x_step + 1),
							time_end(_time_end), num_of_x_step{_num_of_x_step}, num_of_time_step{_num_of_time_step}
{
	std::cout << time_end << "  " <<  num_of_x_step << "  "<< num_of_time_step << std::endl;
}

void Heat_transfer_equation::min_max()
{
	for (int i = 0; i < num_of_x_step + 1; ++i) {
		if (V_min > values_in_grid_nodes[i]) {
			V_min = values_in_grid_nodes[i];
		} else if (V_max < values_in_grid_nodes[i]) {
			V_max = values_in_grid_nodes[i];
		}
	}
}

void Heat_transfer_equation::boundary_value(int time)
{
	if (first_type_left) {
		values_in_grid_nodes[0] = left(time_step * time);
	} else {
		values_in_grid_nodes[0] = (4 * values_in_grid_nodes[1] - values_in_grid_nodes[2] -
			2 * x_step * left(time_step * time)) / 3;
	}
	if (first_type_right) {
		values_in_grid_nodes[num_of_x_step] = right(time_step * time);
	} else {
		values_in_grid_nodes[num_of_x_step] = (2 * x_step * right(time_step * time) + 
			4 * values_in_grid_nodes[num_of_x_step - 1] - values_in_grid_nodes[num_of_x_step - 2]) / 3;
	}
}

void Heat_transfer_equation::recording_data_to_file(std::ofstream& ofile)
{

	if (counter % devider) {
		++counter;
		return;
	}
	++counter;
	double cur_x {0.0};
	for (int i {0}; i < num_of_x_step + 1; ++i) {
		ofile << cur_x << "\t" << values_in_grid_nodes[i] << std::endl;
		cur_x += x_step;
	}
	ofile << std::endl << std::endl;
}

void Heat_transfer_equation::explicit_scheme()
{
	std::ofstream ofile("graph.data");
	for (int i {1}; i < num_of_x_step; ++i) {
		values_in_grid_nodes[i] = begin(x_step * i);
	}
	boundary_value(0);
	V_min = V_max = values_in_grid_nodes[0];
	min_max();
	recording_data_to_file(ofile);
	for (int i {0}; i < num_of_time_step; ++i) {
		for (int j {1}; j < num_of_x_step; ++j) {
			values_in_grid_nodes[j - 1] = values_in_grid_nodes[j] + 
			time_step * a * a * (values_in_grid_nodes[j + 1] - 2 * values_in_grid_nodes[j] + values_in_grid_nodes[j - 1]) / (x_step * x_step) +
			time_step * f_function(j * x_step, i * time_step);
		}
		values_in_grid_nodes.push_front(0);
		values_in_grid_nodes.pop_back();
		boundary_value(i + 1);
		min_max();
		recording_data_to_file(ofile);
	}
}

void Heat_transfer_equation::implicit_scheme()
{
	double A {a * a * time_step / x_step / x_step},
		   B {A},
		   C {1 + 2 * A};
	std::ofstream ofile("graph.data");
	for (int i {1}; i < num_of_x_step; ++i) {
		values_in_grid_nodes[i] = begin(x_step * i);
	}
	boundary_value(0);
	V_min = V_max = values_in_grid_nodes[0];
	min_max();
	recording_data_to_file(ofile);
	std::vector<double> alpha(num_of_x_step + 1), beta(num_of_x_step + 1);
	if (first_type_left) {
		alpha[1] = 0;
	} else {
		alpha[1] = 1;
	}	
	for (int i {1}; i < num_of_time_step + 1; ++i) {
		if (first_type_left) {
			beta[1] = left(time_step * i);
		} else {
			beta[1] = -x_step * left(time_step * i);
		}
		for (int j {1}; j < num_of_x_step; ++j) {
			alpha[j + 1] = B / (C - A * alpha[j]);
			beta[j + 1] = ((time_step * f_function(x_step * j, time_step * i) + values_in_grid_nodes[j]) + A * beta[j]) / (C - A * alpha[j]); 
		}
		if (first_type_right) {
			values_in_grid_nodes[num_of_x_step] = right(time_step * i);	
		} else {
			values_in_grid_nodes[num_of_x_step] = (x_step * right(time_step * i) + beta[num_of_x_step]) / (1 - alpha[num_of_x_step]);
		}
		for (int j {num_of_x_step - 1}; j >= 0; --j) {
			values_in_grid_nodes[j] = alpha[j + 1] * values_in_grid_nodes[j + 1] + beta[j + 1];
		}
		min_max();
		recording_data_to_file(ofile);
	}
}

void Heat_transfer_equation::approximation()
{
	double max1 {0.0}, sum1 {0.0}, cur_x {0.0},
			max2 {0.0}, sum2 {0.0}, cur_value1, cur_value2;
	for (int i {0}; i < num_of_x_step + 1; ++i) {
		cur_value1 = std::abs(equation(cur_x, time_end) - values_in_grid_nodes[i]);
		cur_value2 = std::abs(equation(cur_x, time_end));
		if (cur_value1 > max1) {
			max1 = cur_value1;
		}
		if (cur_value2 > max2) {
			max2 = cur_value2;
		}
		sum1 += cur_value1 * cur_value1;
		sum2 += cur_value2 * cur_value2;		
		cur_x += x_step;
	}
	sum1 = std::sqrt(x_step * sum1);
	sum2 = std::sqrt(x_step * sum2);
	std::cout << "Absolute accuracy" << std::endl << max1 << std::endl << sum1 << std::endl;
	std::cout << "Fractional accuracy" << std::endl << max1 / max2 << std::endl << sum1 / sum2 << std::endl;
	//std::cout << V_max << std::endl << V_min << std::endl;	
}

void Heat_transfer_equation::graph()
{
	std::ofstream omain("main.gn"), oplotter("plotter.gn");
	omain << "set xrange [0:1]\nset yrange [" << static_cast<int>(V_min) - 1 <<": " << static_cast<int>(V_max)  + 1 << "]\niter = 0\nload\"plotter.gn\"";
	oplotter << "iter = iter + 1\nplot \"graph.data\" i iter u 1:2 w l lt 6 notitle\npause 0.1\nif (iter < " << num_of_time_step / devider - 5 << ") reread\n";
}

//[](const int i, const int j, const int n) -> double { return 1.0 / (2 * n - i - j - 1); }
/*
double u(const double x, const double t)
{
	//return (t * t + 1) * sin(x * M_PI);
	 //return  exp(5.0 - M_PI * M_PI * t) * sin(x * M_PI);
    return M_PI * x + t + 2 * co;
}

double left(const double t)
{
	//return sin(t);
	//return 0;//true
	//return (t * t + 1) * M_PI;
	return t;//true
    //return M_PI + 10 * cos(10 * t);//false
    //return exp(5.0 - M_PI * M_PI * t) * M_PI;//false
}
double right(const double t)
{
	//return 0;
	//return 0;//true
	//return -(t * t + 1) * M_PI;
	//return -exp(5.0 - M_PI * M_PI * t) * M_PI;//false
    //(1,t)
    return M_PI + t + 2 * cos(10 * t) * sin(5);//true
    //return M_PI + 10 * cos(10 * t) * cos(5);//false
}
double begin(const double x)
{
	//return sin(x * M_PI);
	//return exp(5.0) * sin(x * M_PI);
    return M_PI* x + 2 * sin(5 * x);
    //return 0;
}
double f(const double x, const double t)
{
	//return 0;
	return 1 + sin(5 * x) * (50 * cos(10 * t) - 20 * sin(10 * t));
    //return (2 * t + (t * t + 1) * M_PI * M_PI) * sin(x * M_PI);
}
*/
double u(double x, double t) {
//return exp(5.0 - M_PI * M_PI * t) * sin(x * M_PI);
return (t*t + 1)*sin(M_PI_2 * x);
}

double begin(double x) {
//return exp(5.0) * sin(x * M_PI);
return sin(M_PI_2 * x);
}

double left(double t) {
//return 0;
return M_PI_2*(t*t + 1); // 1

}

double right(double t) {
//return 0;
return 0;
}

double psi1(double t) {
//return t;
return M_PI * tan(t) * t + 10 * cos(10 * t);
}

double psi2(double t) {
//return -exp(5.0 - M_PI * M_PI * t) * M_PI;
return M_PI * tan(t) * t + 10 * cos(10 * t) * cos(5);
}

double f(double x, double t) {
//return 0;
return 2*t*sin(M_PI_2 * x) + (M_PI_2*M_PI_2)*(t*t + 1)*sin(M_PI_2 * x);
}
int main(int argc, char* argv[])
{
	int num_x {100}, num_time {1000};//2*x*x<t
	for (int i {1}; i < argc - 1; i += 2) {
		std::string param = argv[i];
		if (param == "-x") {
			num_x = std::strtol(argv[i + 1], NULL, 10);
		} else if (param == "-t") {
			num_time = std::strtol(argv[i + 1], NULL, 10);
		} else {
			std::cout << "wrong parameters" << std::endl;
			exit(1);
		}
	}
	Heat_transfer_equation equation (u, left, right, begin, false, false, f, 1.0, num_x, num_time);
    std::cout << "Explicit(0) or implicit(1) scheme?" << std::endl;
    int tmp;
    std::cin >> tmp;
    if (!tmp) {
        equation.explicit_scheme();
    } else {
        equation.implicit_scheme();

    }
    //equation.implicit_scheme();
	equation.approximation();
	//equation.graph();
	return 0;
}
