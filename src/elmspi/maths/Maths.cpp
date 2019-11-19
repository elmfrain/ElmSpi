#include "Maths.h"

namespace ElmSpiMaths
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