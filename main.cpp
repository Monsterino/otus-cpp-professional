#include "matrix.h"
#include <string>
int main() {

	Matrix<int, 0> matrix;
	for (int i = 0; i < 10; i++) {
		matrix[i][i] = i;
		matrix[9 - i][i] = 9 - i;
	}

	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			int value = matrix[i][j];
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Matrix size: " << matrix.size() << std::endl;

	for (auto c : matrix) {
		int x;
		int y;
		int v;
		std::tie(x, y, v) = c;
		std::cout << x << y << v << std::endl;
	}

	return 0;
}
