#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <string>

#define M_PI_F 3.14159265358979323846f
#define M_PI2_F 1.57079632679489661923f;
#define M_PI4_F 0.78539816339744830961f;

namespace espi 
{
	float toRadians(float degrees);
	float toDegrees(float radians);
	double toRadians(double degrees);
	double toDegrees(double radians);
	unsigned int factorial(unsigned int factors);

	struct vec2f {
		union
		{
			struct
			{
				float x, y;
			};
			struct
			{
				float u, v;
			};
		};
		vec2f()
		{
			x = y = 0.0f;
		}
		vec2f(float v)
		{
			this->x = this->y = v;
		}
		vec2f(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		vec2f operator+(const vec2f& vec) const
		{
			return vec2f(x + vec.x, y + vec.y);
		}
		vec2f operator-(const vec2f& vec) const
		{
			return vec2f(x - vec.x, y - vec.y);
		}
		vec2f operator*(const vec2f& vec) const
		{
			return vec2f(x * vec.x, y * vec.y);
		}
		vec2f operator/(const vec2f& vec) const
		{
			return vec2f(x / vec.x, y / vec.y);
		}
		vec2f operator+(float v) const
		{
			return vec2f(x + v, y + v);
		}
		vec2f operator-(float v) const
		{
			return vec2f(x - v, y - v);
		}
		vec2f operator*(float v) const
		{
			return vec2f(x * v, y * v);
		}
		vec2f operator/(float v) const
		{
			return vec2f(x / v, y / v);
		}
		void operator+=(const vec2f& vec)
		{
			*this = *this + vec;
		}
		void operator-=(const vec2f& vec)
		{
			*this = *this - vec;
		}
		void operator*=(const vec2f& vec)
		{
			*this = *this * vec;
		}
		void operator/=(const vec2f& vec)
		{
			*this = *this / vec;
		}
		void operator+=(float v)
		{
			*this = *this + v;
		}
		void operator-=(float v)
		{
			*this = *this - v;
		}
		void operator*=(float v)
		{
			*this = *this * v;
		}
		void operator/=(float v)
		{
			*this = *this / v;
		}
		bool operator==(const vec2f& vec) const
		{
			return x == vec.x && y == vec.y ? true : false;
		}
		bool operator!=(const vec2f& vec) const
		{
			return x == vec.x && y == vec.y ? false : true;
		}
	};
	vec2f operator+(float v, const vec2f& vec);
	vec2f operator-(float v, const vec2f& vec);
	vec2f operator*(float v, const vec2f& vec);
	vec2f operator/(float v, const vec2f& vec);
	std::ostream& operator<<(std::ostream& stream, const vec2f& vec);

	struct vec3f {
		union
		{
			struct
			{
				float x, y, z;
			};
			struct
			{
				float r, g, b;
			};
		};
		vec3f()
		{
			x = y = z = 0.0f;
		}
		vec3f(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		vec3f(float v)
		{
			this->x = this->y = this->z = v;
		}
		vec3f(const vec2f& vec, float z)
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = z;
		}
		vec3f(float x, const vec2f& vec)
		{
			this->x = x;
			this->y = vec.x;
			this->z = vec.y;
		}

		vec3f operator+(const vec3f& vec) const
		{
			return vec3f(x + vec.x, y + vec.y, z + vec.z);
		}
		vec3f operator-(const vec3f& vec) const
		{
			return vec3f(x - vec.x, y - vec.y, z - vec.z);
		}
		vec3f operator*(const vec3f& vec) const
		{
			return vec3f(x * vec.x, y * vec.y, z * vec.z);
		}
		vec3f operator/(const vec3f& vec) const
		{
			return vec3f(x / vec.x, y / vec.y, z / vec.z);
		}
		vec3f operator+(float v) const
		{
			return vec3f(x + v, y + v, z + v);
		}
		vec3f operator-(float v) const
		{
			return vec3f(x - v, y - v, z - v);
		}
		vec3f operator*(float v) const
		{
			return vec3f(x * v, y * v, z * v);
		}
		vec3f operator/(float v) const
		{
			return vec3f(x / v, y / v, z / v);
		}
		void operator+=(const vec3f& vec)
		{
			*this = *this + vec;
		}
		void operator-=(const vec3f& vec)
		{
			*this = *this - vec;
		}
		void operator*=(const vec3f& vec)
		{
			*this = *this * vec;
		}
		void operator/=(const vec3f& vec)
		{
			*this = *this / vec;
		}
		void operator+=(float v)
		{
			*this = *this + v;
		}
		void operator-=(float v)
		{
			*this = *this - v;
		}
		void operator*=(float v)
		{
			*this = *this * v;
		}
		void operator/=(float v)
		{
			*this = *this / v;
		}
		bool operator==(const vec3f& vec) const
		{
			return (x == vec.x && y == vec.y) && z == vec.z ? true : false;
		}
		bool operator!=(const vec3f& vec) const
		{
			return (x == vec.x && y == vec.y) && z == vec.z ? false : true;
		}
	};
	vec3f operator+(float v, const vec3f& vec);
	vec3f operator-(float v, const vec3f& vec);
	vec3f operator*(float v, const vec3f& vec);
	vec3f operator/(float v, const vec3f& vec);
	std::ostream& operator<<(std::ostream& stream, const vec3f& vec);

	struct vec4f {
		union
		{
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				float r, g, b, a;
			};
		};
		vec4f()
		{
			x = y = z = w = 0.0f;
		}
		vec4f(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		vec4f(float v)
		{
			this->x = this->y = this->z = this->w = v;
		}
		vec4f(const vec2f& vec, float z, float w)
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = z;
			this->w = w;
		}
		vec4f(float x, const vec2f& vec, float w)
		{
			this->x = x;
			this->y = vec.x;
			this->z = vec.y;
			this->w = w;
		}
		vec4f(float x, float y, const vec2f& vec)
		{
			this->x = x;
			this->y = y;
			this->z = vec.x;
			this->w = vec.y;
		}
		vec4f(const vec2f& vec1, const vec2f& vec2)
		{
			this->x = vec1.x;
			this->y = vec1.y;
			this->z = vec2.x;
			this->w = vec2.y;
		}
		vec4f(const vec3f& vec, float w)
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = w;
		}
		vec4f(float x, const vec3f& vec)
		{
			this->x = x;
			this->y = vec.x;
			this->z = vec.y;
			this->w = vec.z;
		}
		vec4f operator+(const vec4f& vec) const
		{
			return vec4f(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		vec4f operator-(const vec4f& vec) const
		{
			return vec4f(x + vec.x, y + vec.y, z + vec.z, w - vec.w);
		}
		vec4f operator*(const vec4f& vec) const
		{
			return vec4f(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
		}
		vec4f operator/(const vec4f& vec) const
		{
			return vec4f(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
		}
		vec4f operator+(float v) const
		{
			return vec4f(x + v, y + v, z + v, w + v);
		}
		vec4f operator-(float v) const
		{
			return vec4f(x - v, y - v, z - v, w - v);
		}
		vec4f operator*(float v) const
		{
			return vec4f(x * v, y * v, z * v, w * v);
		}
		vec4f operator/(float v) const
		{
			return vec4f(x / v, y / v, z / v, w / v);
		}
		void operator+=(const vec4f& vec)
		{
			*this = *this + vec;
		}
		void operator-=(const vec4f& vec)
		{
			*this = *this - vec;
		}
		void operator*=(const vec4f& vec)
		{
			*this = *this * vec;
		}
		void operator/=(const vec4f& vec)
		{
			*this = *this / vec;
		}
		void operator+=(float v)
		{
			*this = *this + v;
		}
		void operator-=(float v)
		{
			*this = *this - v;
		}
		void operator*=(float v)
		{
			*this = *this * v;
		}
		void operator/=(float v)
		{
			*this = *this / v;
		}
		bool operator==(const vec4f& vec) const
		{
			return (x == vec.x && y == vec.y) && z == vec.z ? true : false;
		}
		bool operator!=(const vec4f& vec) const
		{
			return (x == vec.x && y == vec.y) && z == vec.z ? false : true;
		}
	};
	vec4f operator+(float v, const vec4f& vec);
	vec4f operator-(float v, const vec4f& vec);
	vec4f operator*(float v, const vec4f& vec);
	vec4f operator/(float v, const vec4f& vec);
	std::ostream& operator<<(std::ostream& stream, const vec4f& vec);

	struct matrixf
	{
		float* const array = nullptr;
		const uint8_t rows = 0;
		const uint8_t columns = 0;

		float& e(uint8_t row, uint8_t column) const
		{
			return array[column - 1 + (row - 1) * columns];
		}
		void set(float values[])
		{
			memcpy(array, values, (rows * columns) * sizeof(float));
		}

		/**
			Creates a matrix with the specified size.

			-If the dimensions are equal, an identity matrix is automatically made.
		*/
		matrixf(uint8_t rows, uint8_t columns)
		{
			*((float**) & array) = new float[rows * columns];
			*((uint8_t*)&this->rows) = rows;
			*((uint8_t*)&this->columns) = columns;
			memset(array, 0, (rows * columns) * sizeof(float));
			if (rows == columns)
			{
				for (uint8_t i = 0; i < rows; i++)
				{
					e(i + 1, i + 1) = 1.0f;
				}
			}
		}
		matrixf(float values[], uint8_t rows, uint8_t columns)
		{
			*((float**) & array) = new float[rows * columns];
			*((uint8_t*)&this->rows) = rows;
			*((uint8_t*)&this->columns) = columns;
			memset(array, 0, (rows * columns) * sizeof(float));
			set(values);
		}
		matrixf(const matrixf& copy) : rows(copy.rows), columns(copy.columns)
		{
			*((float**) & array) = new float[rows * columns];
			memcpy(array, copy.array, (rows * columns) * sizeof(float));
		}
		~matrixf()
		{
			delete[] array;
		}

		matrixf operator+(const matrixf& mat) const
		{
			matrixf result = matrixf(rows, columns);
			if (rows != mat.rows && columns != mat.columns) throw std::invalid_argument("Addition of matricies with different sizes");

			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					result.e(row + 1, column + 1) = e(row + 1, column + 1) + mat.e(row + 1, column + 1);
				}
			}
			return matrixf(result);
		}
		matrixf operator-(const matrixf& mat) const
		{
			matrixf result = matrixf(rows, columns);
			if (rows != mat.rows && columns != mat.columns) throw std::invalid_argument("Subtraction of matricies with different sizes");

			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					result.e(row + 1, column + 1) = e(row + 1, column + 1) - mat.e(row + 1, column + 1);
				}
			}
			return matrixf(result);
		}
		matrixf operator*(const matrixf& mat) const
		{
			matrixf result = matrixf(rows, mat.columns);
			if (columns != mat.rows) throw std::invalid_argument("Multiplication of incompatable matricies");

			for (uint8_t row = 0; row < rows; row++)
			{
				for (uint8_t column = 0; column < mat.columns; column++)
				{
					result.e(row + 1, column + 1) = 0.0f;
					for (uint8_t value = 0; value < mat.rows; value++)
					{
						result.e(row + 1, column + 1) += e(row + 1, value + 1) * mat.e(value + 1, column + 1);
					}
				}
			}
			return matrixf(result);
		}
		matrixf operator*(float value) const
		{
			matrixf result = matrixf(rows, columns);

			for (uint8_t row = 0; row < rows; row++)
			{
				for (uint8_t column = 0; column < columns; column++)
				{
					result.e(row + 1, column + 1) = e(row + 1, column + 1) * value;
				}
			}
			return matrixf(result);
		}
		void operator+=(const matrixf& mat)
		{
			if (rows != mat.rows && columns != mat.columns) throw std::invalid_argument("Addition of matricies with different sizes");

			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					e(row + 1, column + 1) = e(row + 1, column + 1) + mat.e(row + 1, column + 1);
				}
			}
		}
		void operator-=(const matrixf& mat)
		{
			if (rows != mat.rows && columns != mat.columns) throw std::invalid_argument("Subtraction of matricies with different sizes");

			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					e(row + 1, column + 1) = e(row + 1, column + 1) - mat.e(row + 1, column + 1);
				}
			}
		}
		void operator+=(float v)
		{
			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					e(row + 1, column + 1) = e(row + 1, column + 1) + v;
				}
			}
		}
		void operator-=(float v)
		{
			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					e(row + 1, column + 1) = e(row + 1, column + 1) - v;
				}
			}
		}
		void operator*=(float v)
		{
			for (uint8_t column = 0; column < columns; column++)
			{
				for (uint8_t row = 0; row < rows; row++)
				{
					e(row + 1, column + 1) = e(row + 1, column + 1) * v;
				}
			}
		}
		void operator=(const matrixf& mat)
		{
			if (rows * columns != mat.rows * mat.columns)
			{
				delete[] array;
				*((uint8_t*)&this->rows) = mat.rows;
				*((uint8_t*)&this->columns) = mat.columns;
				*((float**) & array) = new float[rows * columns];
			}
			set(mat.array);
		}
		void transpose()
		{
			float* prevArray = (float*)_alloca(rows * columns * sizeof(float));
			memcpy(prevArray, array, rows * columns * sizeof(float));

			for (uint8_t row = 0; row < rows; row++)
			{
				for (uint8_t column = 0; column < columns; column++)
				{
					e(column + 1, row + 1) = prevArray[column + row * columns];
				}
			}

			uint8_t rows = this->rows;
			uint8_t columns = this->columns;
			*((uint8_t*)&this->rows) = columns;
			*((uint8_t*)&this->columns) = rows;
		}
	};
	std::ostream& operator<<(std::ostream& stream, const matrixf& matrix);
	matrixf operator+(float v, const matrixf& mat);
	matrixf operator-(float v, const matrixf& mat);
	matrixf operator*(float v, const matrixf& mat);
	matrixf transpose(const matrixf& mat);
	/*Currently Only Supports Matrcies up to 4x4*/
	matrixf adjugate(const matrixf& mat);
	/*Currently Only Supports Matrcies up to 4x4*/
	float determinant(const matrixf& mat);
	matrixf inverse(const matrixf& mat);
	vec2f operator*(const vec2f& vec, const matrixf& mat);
	vec2f operator*(const matrixf& mat, const vec2f& vec);
	vec3f operator*(const vec3f& vec, const matrixf& mat);
	vec3f operator*(const matrixf& mat, const vec3f& vec);
	vec4f operator*(const vec4f& vec, const matrixf& mat);
	vec4f operator*(const matrixf& mat, const vec4f& vec);

	struct entity
	{
		vec3f pos = vec3f(0.0f, 0.0f, 0.0f);
		float rotPitch = 0.0f, rotYaw = 0.0f, rotRoll = 0.0f;
		float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
		entity()
		{
		}
		void scale(float scale)
		{
			scaleX = scaleY = scaleZ = scale;
		}
		void reset()
		{
			pos.x = pos.y = pos.z = rotPitch = rotYaw = rotRoll = 0.0f;
			scaleX = scaleY = scaleZ = 1.0f;
		}
		matrixf getNormalsMatrix()
		{
			float rotPitch = toRadians(this->rotPitch);
			float rotYaw = toRadians(this->rotYaw);
			float rotRoll = toRadians(this->rotRoll);

			float cosZ = cosf(rotRoll);
			float sinZ = sinf(rotRoll);

			float cosY = cosf(rotYaw);
			float sinY = sinf(rotYaw);

			float cosX = cosf(rotPitch);
			float sinX = sinf(rotPitch);

			float rollFactor[] =
			{
				cosZ,-sinZ, 0,
				sinZ, cosZ, 0,
				   0,    0, 1
			}; matrixf roll(rollFactor, 3, 3);
			float yawFactor[] =
			{
				cosY, 0, sinY,
				   0, 1,    0,
			   -sinY, 0, cosY
			}; matrixf yaw(yawFactor, 3, 3);
			float pitchFactor[] =
			{
				1,    0,   0,
				0, cosX,-sinX,
				0, sinX, cosX
			}; matrixf pitch(pitchFactor, 3, 3);

			return roll * pitch * yaw;
		}
		vec3f getUpVec()
		{
			vec3f dir(0, 0, -1);
			float rotPitch = toRadians(this->rotPitch - 90.0f);
			float rotYaw = toRadians(this->rotYaw);
			float rotRoll = toRadians(this->rotRoll);

			float cosZ = cosf(rotRoll);
			float sinZ = sinf(rotRoll);

			float cosY = cosf(rotYaw);
			float sinY = sinf(rotYaw);

			float cosX = cosf(rotPitch);
			float sinX = sinf(rotPitch);

			float rollFactor[] =
			{
				cosZ,-sinZ, 0,
				sinZ, cosZ, 0,
				   0,    0, 1
			}; matrixf roll(rollFactor, 3, 3);
			float yawFactor[] =
			{
				cosY, 0, sinY,
				   0, 1,    0,
			   -sinY, 0, cosY
			}; matrixf yaw(yawFactor, 3, 3);
			float pitchFactor[] =
			{
				1,    0,   0,
				0, cosX,-sinX,
				0, sinX, cosX
			}; matrixf pitch(pitchFactor, 3, 3);
			dir = dir * pitch * yaw * roll;
			return vec3f(dir.x, dir.y, dir.z);
		}
		vec3f getDirectionVec()
		{
			vec4f dir(0, 0, -1, 1);
			float rotPitch = -toRadians(this->rotPitch);
			float rotYaw = toRadians(this->rotYaw);
			float rotRoll = toRadians(this->rotRoll);

			float cosZ = cosf(rotRoll);
			float sinZ = sinf(rotRoll);

			float cosY = cosf(rotYaw);
			float sinY = sinf(rotYaw);

			float cosX = cosf(rotPitch);
			float sinX = sinf(rotPitch);

			float rollFactor[] =
			{
				cosZ,-sinZ, 0, 0,
				sinZ, cosZ, 0, 0,
				   0,    0, 1, 0,
				   0,    0, 0, 1
			}; matrixf roll(rollFactor, 4, 4);
			float yawFactor[] =
			{
				cosY, 0, sinY, 0,
				   0, 1,    0, 0,
			   -sinY, 0, cosY, 0,
			       0, 0,    0, 1
			}; matrixf yaw(yawFactor, 4, 4);
			float pitchFactor[] =
			{
				1,    0,    0, 0,
				0, cosX,-sinX, 0,
				0, sinX, cosX, 1,
				0,    0,    0, 1
			}; matrixf pitch(pitchFactor, 4, 4);

			dir = dir * roll * pitch * yaw;
			return vec3f(dir.x, dir.y, dir.z);
		}
		matrixf getModelMatrix()
		{
			float translationFactor[] =
			{
				1, 0, 0, pos.x,
				0, 1, 0, pos.y,
				0, 0, 1,-pos.z,
				0, 0, 0,    1
			}; matrixf translation(translationFactor, 4, 4);

			float rotPitch = -toRadians(this->rotPitch);
			float rotYaw = toRadians(this->rotYaw);
			float rotRoll = toRadians(this->rotRoll);

			float cosZ = cosf(rotRoll);
			float sinZ = sinf(rotRoll);

			float cosY = cosf(rotYaw);
			float sinY = sinf(rotYaw);

			float cosX = cosf(rotPitch);
			float sinX = sinf(rotPitch);

			float rollFactor[] =
			{
				cosZ,-sinZ, 0, 0,
				sinZ, cosZ, 0, 0,
				   0,    0, 1, 0,
				   0,    0, 0, 1
			}; matrixf roll(rollFactor, 4, 4);
			float yawFactor[] =
			{
				cosY, 0, sinY, 0,
				   0, 1,    0, 0,
			   -sinY, 0, cosY, 0,
				   0, 0,    0, 1
			}; matrixf yaw(yawFactor, 4, 4);
			float pitchFactor[] =
			{
				1,    0,    0, 0,
				0, cosX,-sinX, 0,
				0, sinX, cosX, 1,
				0,    0,    0, 1
			}; matrixf pitch(pitchFactor, 4, 4);
			float scaleFactor[] =
			{
				scaleX,      0,      0,    0,
				     0, scaleY,      0,    0,
					 0,      0, scaleZ,    0,
					 0,      0,      0,    1
			}; matrixf scale(scaleFactor, 4, 4);

			return translation * scale * roll * yaw * pitch;
		}


	};
}