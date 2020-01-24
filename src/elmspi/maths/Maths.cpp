#include "Maths.h"

namespace espi
{
	float toRadians(float degrees)
	{
		return degrees * M_PI_F / 180.0f;
	}
	float toDegrees(float radians)
	{
		return radians * 180.0f / M_PI_F;
	}
	double toRadians(double degrees)
	{
		return degrees * M_PI / 180.0;
	}
	double toDegrees(double radians)
	{
		return radians * 180.0 / M_PI;
	}
	unsigned int factorial(unsigned int factors)
	{
		unsigned int result = 1;
		for (int i = 1; i <= factors; i++) { result *= i; }
		return result;
	}

	vec2f operator+(float v, const vec2f& vec)
	{
		return vec2f(v + vec.x, v + vec.y);
	}
	vec2f operator-(float v, const vec2f& vec)
	{
		return vec2f(v - vec.x, v - vec.y);
	}
	vec2f operator*(float v, const vec2f& vec)
	{
		return vec2f(vec.x * v, vec.y * v);
	}
	vec2f operator/(float v, const vec2f& vec)
	{
		return vec2f(v / vec.x, v / vec.y);
	}
	std::ostream& operator<<(std::ostream& stream, const vec2f& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ")";
		return stream;
	}

	vec3f operator+(float v, const vec3f& vec)
	{
		return vec3f(v + vec.x, v + vec.y, v + vec.z);
	}
	vec3f operator-(float v, const vec3f& vec)
	{
		return vec3f(v - vec.x, v - vec.y, v - vec.z);
	}
	vec3f operator*(float v, const vec3f& vec)
	{
		return vec3f(vec.x * v, vec.y * v, vec.z * v);
	}
	vec3f operator/(float v, const vec3f& vec)
	{
		return vec3f(v / vec.x, v / vec.y, v / vec.z);
	}
	std::ostream& operator<<(std::ostream& stream, const vec3f& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}

	vec4f operator+(float v, const vec4f& vec)
	{
		return vec4f(v + vec.x, v + vec.y, v + vec.z, v + vec.w);
	}
	vec4f operator-(float v, const vec4f& vec)
	{
		return vec4f(v - vec.x, v - vec.y, v - vec.z, v - vec.w);
	}
	vec4f operator*(float v, const vec4f& vec)
	{
		return vec4f(vec.x * v, vec.y * v, vec.z * v, vec.w * v);
	}
	vec4f operator/(float v, const vec4f& vec)
	{
		return vec4f(v / vec.x, v / vec.y, v / vec.z, v / vec.w);
	}
	std::ostream& operator<<(std::ostream& stream, const vec4f& vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const matrixf& matrix)
	{
		std::string s = "";
		for (int row = 0; row < matrix.rows; row++)
		{
			s += row == 0 || row == matrix.rows - 1 ? "[" : "|";
			for (int column = 0; column < matrix.columns; column++)
			{
				s += std::to_string(matrix.e(row + 1, column + 1));
				if (column != matrix.columns - 1) s += ", ";
			}
			s += row == 0 || row == matrix.rows - 1 ? "]\n" : "|\n";
		}
		stream << s;

		return stream;
	}
	matrixf operator+(float v, const matrixf& mat)
	{
		matrixf result = matrixf(mat.rows, mat.columns);

		for (uint8_t column = 0; column < mat.columns; column++)
		{
			for (uint8_t row = 0; row < mat.rows; row++)
			{
				result.e(row + 1, column + 1) = v + mat.e(row + 1, column + 1);
			}
		}
		return matrixf(result);
	}
	matrixf operator-(float v, const matrixf& mat)
	{
		matrixf result = matrixf(mat.rows, mat.columns);

		for (uint8_t column = 0; column < mat.columns; column++)
		{
			for (uint8_t row = 0; row < mat.rows; row++)
			{
				result.e(row + 1, column + 1) = v - mat.e(row + 1, column + 1);
			}
		}
		return matrixf(result);
	}
	matrixf operator*(float v, const matrixf& mat)
	{
		matrixf result = matrixf(mat.rows, mat.columns);
		if (mat.rows != mat.rows && mat.columns != mat.columns) throw std::invalid_argument("Addition of matricies with different sizes");

		for (uint8_t column = 0; column < mat.columns; column++)
		{
			for (uint8_t row = 0; row < mat.rows; row++)
			{
				result.e(row + 1, column + 1) = v * mat.e(row + 1, column + 1);
			}
		}
		return matrixf(result);
	}
	matrixf transpose(const matrixf& mat)
	{
		float* prevArray = (float*)_alloca(mat.rows * mat.columns * sizeof(float));

		for (uint8_t row = 0; row < mat.rows; row++)
		{
			for (uint8_t column = 0; column < mat.columns; column++)
			{
				prevArray[row + column * mat.rows] = mat.e(row + 1, column + 1);
			}
		}
		return matrixf(prevArray, mat.columns, mat.rows);
	}
	/*Determinant Helper Function*/
	static float determinant2x2(float r1c1, float r1c2, float r2c1, float r2c2)
	{
		return r1c1 * r2c2 - r2c1 * r1c2;
	}
	static float determinant3x3(float r1c1, float r1c2, float r1c3,
								float r2c1, float r2c2, float r2c3,
								float r3c1, float r3c2, float r3c3)
	{
		return r1c1 * determinant2x2(r2c2, r2c3, r3c2, r3c3) - r1c2 * determinant2x2(r2c1, r2c3, r3c1, r3c3) + r1c3 * determinant2x2(r2c1, r2c2, r3c1, r3c2);
	}
	float determinant(const matrixf& mat)
	{
		if (mat.columns != mat.rows || mat.columns < 1) throw std::invalid_argument("Can't find the determinant of a rectangle shaped matrix or a 1 by 1 matrix.");
		if (mat.columns > 9) throw std::invalid_argument("Matrix too big to find determinant");

		if (mat.columns == 2) return determinant2x2(mat.e(1, 1), mat.e(1, 2), mat.e(2, 1), mat.e(2, 2));

		if (mat.columns == 3) return determinant3x3(mat.e(1, 1), mat.e(1, 2), mat.e(1, 3),
													mat.e(2, 1), mat.e(2, 2), mat.e(2, 3),
													mat.e(3, 1), mat.e(3, 2), mat.e(3, 3));

		if (mat.columns == 4)
		{
			float sum = 0.0f;
			for (int column = 1; column <= 4; column++)
			{
				int8_t sign = (column + 1) % 2 == 0 ? 1 : -1;
				float coeficientFactor = sign * mat.e(1, column);

				int col1, col2, col3;
				switch (column)
				{
				case 1:
					col1 = 2; col2 = 3; col3 = 4; break;
				case 2:
					col1 = 1; col2 = 3; col3 = 4; break;
				case 3:
					col1 = 1; col2 = 2; col3 = 4; break;
				case 4:
					col1 = 1; col2 = 2; col3 = 3; break;
				}

				float determinatFactor = determinant3x3(mat.e(2, col1), mat.e(2, col2), mat.e(2, col3),
														mat.e(3, col1), mat.e(3, col2), mat.e(3, col3),
														mat.e(4, col1), mat.e(4, col2), mat.e(4, col3));
				sum += coeficientFactor * determinatFactor;
			}
			return sum;
		}

		return 0.0f;
	}
	static matrixf adjugate3x3(const matrixf& mat)
	{
		float matData[9];

		int row1, row2, col1, col2;

		for (int column = 1; column <= 3; column++)
		{
			for (int row = 1; row <= 3; row++)
			{
				int element = row * 3 - 3 + column;
				int8_t sign = (element + 1) % 2 == 0 ? 1 : -1;

				switch (column)
				{
				case 1:
					col1 = 2; col2 = 3; break;
				case 2:
					col1 = 1; col2 = 3; break;
				case 3:
					col1 = 1; col2 = 2; break;
				}
				switch (row)
				{
				case 1:
					row1 = 2; row2 = 3; break;
				case 2:
					row1 = 1; row2 = 3; break;
				case 3:
					row1 = 1; row2 = 2; break;
				}

				matData[element - 1] = sign * determinant2x2(mat.e(row1, col1), mat.e(row1, col2),
														 mat.e(row2, col1), mat.e(row2, col2));
			}
		}

		matrixf cofactor = matrixf(matData, 3, 3);
		cofactor.transpose();
		return cofactor;
	}
	matrixf adjugate(const matrixf& mat)
	{
		if (mat.columns != mat.rows) throw std::invalid_argument("Matrix must be a square to find it's aducate.");
		if (mat.columns == 1) return matrixf(1, 1);
		if (mat.columns == 2)
		{
			float matData[4] =
			{
				 mat.e(2, 2), -mat.e(1, 2),
				-mat.e(2, 1),  mat.e(1, 1)
			}; return matrixf(matData, 2, 2);
		}
		if (mat.columns == 3)
		{
			return adjugate3x3(mat);
		}
		if (mat.columns == 4)
		{
			float matData[16];

			int row1, row2, row3, col1, col2, col3;

			for (int column = 1; column <= 4; column++)
			{
				for (int row = 1; row <= 4; row++)
				{
					int element = row * 4 - 4 + column;
					int8_t sign = (element + 1) % 2 == 0 ? 1 : -1;
					if (row % 2 == 0) sign *= -1;

					switch (column)
					{
					case 1:
						col1 = 2; col2 = 3; col3 = 4; break;
					case 2:
						col1 = 1; col2 = 3; col3 = 4; break;
					case 3:
						col1 = 1; col2 = 2; col3 = 4; break;
					case 4:
						col1 = 1; col2 = 2; col3 = 3; break;
					}
					switch (row)
					{
					case 1:
						row1 = 2; row2 = 3; row3 = 4; break;
					case 2:
						row1 = 1; row2 = 3; row3 = 4; break;
					case 3:
						row1 = 1; row2 = 2; row3 = 4; break;
					case 4:
						row1 = 1; row2 = 2; row3 = 3; break;
					}

					matData[element - 1] = sign * determinant3x3(mat.e(row1, col1), mat.e(row1, col2), mat.e(row1, col3),
																 mat.e(row2, col1), mat.e(row2, col2), mat.e(row2, col3),
																 mat.e(row3, col1), mat.e(row3, col2), mat.e(row3, col3));
				}
			}

			matrixf cofactor = matrixf(matData, 4, 4);
			cofactor.transpose();
			return cofactor;
		}
		return matrixf(1, 1);
	}
	matrixf inverse(const matrixf& mat)
	{
		if (mat.columns != mat.rows || mat.columns == 1) throw std::invalid_argument("Matrix must be a square to find it's inverse.");
		if (mat.columns > 4) throw std::invalid_argument("Matricies over 4x4 in size is not supported yet.");

		return (1.0f / determinant(mat)) * adjugate(mat);

	}
	vec2f operator*(const vec2f& vec, const matrixf& mat)
	{
		float data[2] = { vec.x, vec.y };
		matrixf vecAsMat(data, 1, 2);
		vecAsMat = vecAsMat * mat;
		return vec2f(vecAsMat.e(1, 1), vecAsMat.e(1, 2));
	}
	vec2f operator*(const matrixf& mat, const vec2f& vec)
	{
		float data[2] = { vec.x, vec.y };
		matrixf vecAsMat(data, 2, 1);
		vecAsMat = mat * vecAsMat;
		return vec2f(vecAsMat.e(1, 1), vecAsMat.e(1, 2));
	}
	vec3f operator*(const vec3f& vec, const matrixf& mat)
	{
		float data[3] = { vec.x, vec.y, vec.z };
		matrixf vecAsMat(data, 1, 3);
		vecAsMat = vecAsMat * mat;
		return vec3f(vecAsMat.e(1, 1), vecAsMat.e(1, 2), vecAsMat.e(1, 3));
	}
	vec3f operator*(const matrixf& mat, const vec3f& vec)
	{
		float data[3] = { vec.x, vec.y, vec.z };
		matrixf vecAsMat(data, 3, 1);
		vecAsMat = mat * vecAsMat;
		return vec3f(vecAsMat.e(1, 1), vecAsMat.e(1, 2), vecAsMat.e(1, 3));
	}
	vec4f operator*(const vec4f& vec, const matrixf& mat)
	{
		float data[4] = { vec.x, vec.y, vec.z, vec.w };
		matrixf vecAsMat(data, 1, 4);
		vecAsMat = vecAsMat * mat;
		return vec4f(vecAsMat.e(1, 1), vecAsMat.e(1, 2), vecAsMat.e(1, 3), vecAsMat.e(1, 4));
	}
	vec4f operator*(const matrixf& mat, const vec4f& vec)
	{
		float data[4] = { vec.x, vec.y, vec.z, vec.w };
		matrixf vecAsMat(data, 4, 1);
		vecAsMat = mat * vecAsMat;
		return vec4f(vecAsMat.e(1, 1), vecAsMat.e(1, 2), vecAsMat.e(1, 3), vecAsMat.e(1, 4));
	}
}