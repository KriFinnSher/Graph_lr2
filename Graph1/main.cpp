// Задача 1. Даны две разреженные матрицы CS. Перемножить их и результат занести в разреженную матрицу CRS.

#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include<chrono>
#include<algorithm>
#include<sstream>
#include<string>


struct Elem {
	int row, col, val;
	Elem(int row, int col, int val) : row(row), col(col), val(val) {}
};

std::vector<Elem> mult(std::vector<Elem> mat_1, std::vector<Elem> mat_2) {
	std::vector<Elem> result;
	for (int i = 0; i < mat_1.size(); i++) {
		for (int j = 0; j < mat_2.size(); j++) {
			if (mat_1[i].col == mat_2[j].row)
				result.push_back(Elem(mat_1[i].row, mat_2[j].col, mat_1[i].val * mat_2[j].val));
		}
	}
	return result;
}

std::vector<Elem> mult_helper(std::vector<Elem> temp) {
	std::vector<Elem> result;
	size_t end = temp.size();
	for (int i = 0; i < end; i++) {
		for (int j = 0; j < end; j++) {
			if (temp[i].col == temp[j].col && temp[i].row == temp[j].row && i != j) {
				temp[i].val += temp[j].val;
				temp.erase(temp.begin() + j);
				end--;
				j--;
			}
		}
		result.push_back(temp[i]);
	}
	return result;
}

std::vector<std::vector<int>> convert_to_crs_mat(std::vector<Elem> mat, int row, int col) {
	std::vector<std::vector<int>> crs_mat(3);
	crs_mat[2].push_back(0);
	int cnt = 0;
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			bool found = false;
			for (const auto& elem : mat) {
				if (elem.row == i && elem.col == j) {
					crs_mat[0].push_back(elem.val);
					crs_mat[1].push_back(elem.col);
					found = true;
					cnt++;
					break;
				}
			}
		}
		crs_mat[2].push_back(cnt);
	}
	return crs_mat;
}

std::vector<Elem> generate_sparse_mat(int row, int col, int count_of_elems) {
	std::vector<Elem> mat;
	std::vector<int> rows;
	std::vector<int> cols;
	std::vector<std::pair<int, int>> all_indicies;
	while (mat.size() < count_of_elems) {
		int rand_row = rand() % row;
		int rand_col = rand() % col;
		if (std::count(all_indicies.begin(), all_indicies.end(), std::make_pair(rand_row, rand_col)) == 0 &&
			std::count(rows.begin(), rows.end(), rand_row) <= row && std::count(cols.begin(), cols.end(), rand_col) <= col) {
			all_indicies.emplace_back(rand_row, rand_col);
			rows.emplace_back(rand_row);
			cols.emplace_back(rand_col);
			mat.push_back(Elem(rand_row, rand_col, 1 + rand() % 9));
		}
	}
	return mat;
}

void print_sparse_mat(std::vector<Elem> mat, int row, int col) {
	std::cout << "1 - Вывести матрицу целиком, 2 - вывести в формате head & tail\n";
	int choice = 0;
	std::cin >> choice;
	if (choice == 1) {
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				bool found = false;
				for (const auto& elem : mat) {
					if (elem.row == i && elem.col == j) {
						std::cout << elem.val << " ";
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "0 ";
				}
			}
			std::cout << std::endl;
		}
	}
	else if (choice == 2) {
		std::cout << "Введите значения head и tail через пробел: ";
		int head = 0, tail = 0;
		std::cin >> head >> tail;
		for (int i = 0; i < head; ++i) {
			for (int j = 0; j < head; ++j) {
				bool found = false;
				for (const auto& elem : mat) {
					if (elem.row == i && elem.col == j) {
						std::cout << elem.val << " ";
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "0 ";
				}
			}
			std::cout << "* ";
			for (int j = col - tail; j < col; ++j) {
				bool found = false;
				for (const auto& elem : mat) {
					if (elem.row == i && elem.col == j) {
						std::cout << elem.val << " ";
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "0 ";
				}
			}
			std::cout << std::endl;
		}
		for (int i = 0; i < head + tail + 1; i++)
			std::cout << "* ";
		std::cout << "\n";
		for (int i = row - tail; i < row; ++i) {
			for (int j = 0; j < head; ++j) {
				bool found = false;
				for (const auto& elem : mat) {
					if (elem.row == i && elem.col == j) {
						std::cout << elem.val << " ";
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "0 ";
				}
			}
			std::cout << "* ";
			for (int j = col - tail; j < col; ++j) {
				bool found = false;
				for (const auto& elem : mat) {
					if (elem.row == i && elem.col == j) {
						std::cout << elem.val << " ";
						found = true;
						break;
					}
				}
				if (!found) {
					std::cout << "0 ";
				}
			}
			std::cout << std::endl;
		}
	}
	else
		std::cout << "Ошибка ввода, перезапустите программу..." << std::endl;
}

void write_custom_to_file(std::string& filename) {
	std::string mat_line;
	std::vector<std::string> mat_lines;
	std::ofstream custom(filename);
	std::cin.get();
	while (std::getline(std::cin, mat_line)) {
		mat_lines.push_back(mat_line);
		if (mat_line == "-1") break;
	}
	mat_lines.pop_back();
	for (const std::string& line : mat_lines)
		custom << line << std::endl;
}

void write_crs_to_file(std::string& filename, std::vector<std::vector<int>> crs_mat) {
	std::ofstream custom(filename);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < crs_mat[i].size(); j++)
			custom << crs_mat[i][j] << " ";
		custom << "\n";
	}
}

std::vector<Elem> read_custom_from_file(std::ifstream& mat_file, int row, int col) {
	std::vector<Elem> custom_mat;
	std::string temp_line;
	while (std::getline(mat_file, temp_line)) {
		std::istringstream mat_line(temp_line);
		int row = 0, col = 0, val = 0;
		mat_line >> row >> col >> val;
		custom_mat.push_back(Elem(row, col, val));
	}
	return custom_mat;
}

void print_crs_mat(std::vector<std::vector<int>> mat) {
	std::cout << "\nИтоговая матрица в формате CRS:\n";
	std::cout << "Массив значений: { ";
	for (int i = 0; i < mat[0].size(); i++)
		std::cout << mat[0][i] << " ";
	std::cout << "}\n";
	std::cout << "Массив индексов столбцов: { ";
	for (int i = 0; i < mat[1].size(); i++)
		std::cout << mat[1][i] << " ";
	std::cout << "}\n";
	std::cout << "Массив индексации строк: { ";
	for (int i = 0; i < mat[2].size(); i++)
		std::cout << mat[2][i] << " ";
	std::cout << "}\n";
}

int main() {
	srand(time(0));
	setlocale(LC_ALL, "ru");

	std::vector<Elem> mat_1;
	std::vector<Elem> mat_2;
	int rows_1 = 0, columns_1 = 0, count_of_elems = 0, res_row = 0, res_col = 0, rows_2 = 0, columns_2 = 0, fullness = 0;

	std::cout << "\t\t\t\t----------- Информация по программе -----------\n\n"
		<< "Для работы с матрицами выберете способ их создания:\n\n"
		<< "1 - произвольная генерация по заданным параметрам\n"
		<< "2 - построение по заданной конкретной структуре\n\n";

	int choice = 0;
	std::cin >> choice;
	std::string file_name;

	if (choice == 1) {
		std::cout << "Введите размерность первой матрицы через пробел: ";
		std::cin >> rows_1 >> columns_1;
		res_row = rows_1;
		count_of_elems = rows_1 * columns_1;
		std::cout << "Введите заполненность первой матрицы (процент ненулевых элементов): ";
		std::cin >> fullness;
		count_of_elems = count_of_elems * fullness / 100;
		mat_1 = generate_sparse_mat(rows_1, columns_1, count_of_elems);

		std::cout << "Введите размерность второй матрицы через пробел: ";
		std::cin >> rows_2 >> columns_2;
		res_col = columns_2;
		count_of_elems = rows_2 * columns_2;
		std::cout << "Введите заполненность второй матрицы (процент ненулевых элементов): ";
		std::cin >> fullness;
		count_of_elems = count_of_elems * fullness / 100;
		mat_2 = generate_sparse_mat(rows_2, columns_2, count_of_elems);

		std::vector<Elem> temp;
		std::vector<Elem> result;
		auto start1 = std::chrono::high_resolution_clock::now();
		temp = mult(mat_1, mat_2);
		result = mult_helper(temp);
		auto end1 = std::chrono::high_resolution_clock::now();
		auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
		std::cout << "\nПервая матрица: " << std::endl;
		print_sparse_mat(mat_1, rows_1, columns_1);
		std::cout << "\nВторая матрица: " << std::endl;
		print_sparse_mat(mat_2, rows_2, columns_2);
		std::cout << "\nИтоговая матрица: " << std::endl;
		print_sparse_mat(result, res_row, res_col);
		std::cout << "\nВремя работы алгоритма: " << duration1 << " мкс.\n";
		std::vector<std::vector<int>> crs_result = convert_to_crs_mat(result, res_row, res_col);
		print_crs_mat(crs_result);
		file_name = "crs_res.txt";
		write_crs_to_file(file_name, crs_result);

	}
	if (choice == 2) {
		std::cout << "Введите размерность первой матрицы через пробел: ";
		std::cin >> rows_1 >> columns_1;
		res_row = rows_1;
		count_of_elems = rows_1 * columns_1;

		file_name = "custom_1.txt";
		std::cout << "Введите значения для первой матрицы через пробел на каждой строке: \n";
		write_custom_to_file(file_name);
		std::ifstream custom1(file_name);
		mat_1 = read_custom_from_file(custom1, rows_1, columns_1);

		std::cout << "Введите размерность второй матрицы через пробел: ";
		std::cin >> rows_2 >> columns_2;
		res_col = columns_2;
		count_of_elems = rows_2 * columns_2;

		file_name = "custom_2.txt";
		std::cout << "Введите значения для второй матрицы через пробел на каждой строке: \n";
		write_custom_to_file(file_name);
		std::ifstream custom2(file_name);
		mat_2 = read_custom_from_file(custom2, rows_2, columns_2);

		std::vector<Elem> temp;
		std::vector<Elem> result;
		auto start2 = std::chrono::high_resolution_clock::now();
		temp = mult(mat_1, mat_2);
		result = mult_helper(temp);
		auto end2 = std::chrono::high_resolution_clock::now();
		auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
		std::cout << "Первая матрица: " << std::endl;
		print_sparse_mat(mat_1, rows_1, columns_1);
		std::cout << "\nВторая матрица: " << std::endl;
		print_sparse_mat(mat_2, rows_2, columns_2);
		std::cout << "\nИтоговая матрица: " << std::endl;
		print_sparse_mat(result, res_row, res_col);
		std::cout << "\nВремя работы алгоритма: " << duration2 << " мкс.\n";
		std::vector<std::vector<int>> crs_result = convert_to_crs_mat(result, res_row, res_col);
		print_crs_mat(crs_result);
		file_name = "crs_res.txt";
		write_crs_to_file(file_name, crs_result);
	}
	return 0;
}
