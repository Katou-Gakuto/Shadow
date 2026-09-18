#pragma once

/*--------------------------*/
/*          構造体          */
/*--------------------------*/

// 2Dベクトル(int)
struct Vector2_Int
{
	int x;
	int y;

	Vector2_Int()
	{
		this->x = 0;
		this->y = 0;
	}
	Vector2_Int(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2_Int(float x, float y)
	{
		this->x = (int)x;
		this->y = (int)y;
	}

	Vector2_Int& operator +=(Vector2_Int &src)
	{
		this->x += src.x;
		this->y += src.y;

		return *this;
	}

	Vector2_Int& operator -=(Vector2_Int &src)
	{
		this->x -= src.x;
		this->y -= src.y;

		return *this;
	}
};

// 2Dベクトル
struct Vector2
{
	float x;
	float y;

	const static Vector2 Zero() { return Vector2(0.0f, 0.0f); }

	Vector2()
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}
	Vector2(float X, float Y)
	{
		this->x = X;
		this->y = Y;
	}

	Vector2& operator +=(Vector2& src)
	{
		this->x += src.x;
		this->y += src.y;

		return *this;
	}

	Vector2& operator -=(Vector2& src)
	{
		this->x -= src.x;
		this->y -= src.y;

		return *this;
	}

	Vector2_Int GetVecInt() const
	{
		Vector2_Int vecInt;
		vecInt.x = static_cast<int>(this->x);
		vecInt.y = static_cast<int>(this->y);
		return vecInt;
	}

	int IntX() const
	{
		return static_cast<int>(this->x);
	}

	int IntY() const
	{
		return static_cast<int>(this->y);
	}
};


/*--------------------------*/
/*          演算子          */
/*--------------------------*/

/*---------------------*/
/*     Vector2_Int     */
/*---------------------*/

// Vector2_Int + Vector2_Int
inline Vector2_Int operator+(const Vector2_Int& lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Vector2_Int + int
inline Vector2_Int operator+(const Vector2_Int& lhs, int rhs)
{
	return Vector2_Int(lhs.x + rhs, lhs.y + rhs);
}

inline Vector2_Int operator+(int lhs, const Vector2_Int& rhs)
{
	return rhs + lhs;
}

// Vector2_Int + float
inline Vector2_Int operator+(const Vector2_Int& lhs, float rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs.x + rhs),
		static_cast<int>(lhs.y + rhs)
	);
}

inline Vector2_Int operator+(float lhs, const Vector2_Int& rhs)
{
	return rhs + lhs;
}

// Vector2_Int - Vector2_Int
inline Vector2_Int operator-(const Vector2_Int& lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Vector2_Int - int
inline Vector2_Int operator-(const Vector2_Int& lhs, int rhs)
{
	return Vector2_Int(lhs.x - rhs, lhs.y - rhs);
}

inline Vector2_Int operator-(int lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs - rhs.x, lhs - rhs.y);
}

// Vector2_Int - float
inline Vector2_Int operator-(const Vector2_Int& lhs, float rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs.x - rhs),
		static_cast<int>(lhs.y - rhs)
	);
}

inline Vector2_Int operator-(float lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs - rhs.x),
		static_cast<int>(lhs - rhs.y)
	);
}

// Vector2_Int * Vector2_Int
inline Vector2_Int operator*(const Vector2_Int& lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs.x * rhs.x, lhs.y * rhs.y);
}

// Vector2_Int * int
inline Vector2_Int operator*(const Vector2_Int& lhs, int rhs)
{
	return Vector2_Int(lhs.x * rhs, lhs.y * rhs);
}

inline Vector2_Int operator*(int lhs, const Vector2_Int& rhs)
{
	return rhs * lhs;
}

// Vector2_Int * float
inline Vector2_Int operator*(const Vector2_Int& lhs, float rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs.x * rhs),
		static_cast<int>(lhs.y * rhs)
	);
}

inline Vector2_Int operator*(float lhs, const Vector2_Int& rhs)
{
	return rhs * lhs;
}

// Vector2_Int / Vector2_Int
inline Vector2_Int operator/(const Vector2_Int& lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs.x / rhs.x, lhs.y / rhs.y);
}

// Vector2_Int / int
inline Vector2_Int operator/(const Vector2_Int& lhs, int rhs)
{
	return Vector2_Int(lhs.x / rhs, lhs.y / rhs);
}

inline Vector2_Int operator/(int lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(lhs / rhs.x, lhs / rhs.y);
}

// Vector2_Int / float
inline Vector2_Int operator/(const Vector2_Int& lhs, float rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs.x / rhs),
		static_cast<int>(lhs.y / rhs)
	);
}
inline Vector2_Int operator/(float lhs, const Vector2_Int& rhs)
{
	return Vector2_Int(
		static_cast<int>(lhs / rhs.x),
		static_cast<int>(lhs / rhs.y)
	);
}


/*-----------------*/
/*     Vector2     */
/*-----------------*/

// Vector2 + Vector2
inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Vector2 + int
inline Vector2 operator+(const Vector2& lhs, int rhs)
{
	return Vector2(lhs.x + rhs, lhs.y + rhs);
}

inline Vector2 operator+(int lhs, const Vector2& rhs)
{
	return rhs + lhs;
}

// Vector2 + float
inline Vector2 operator+(const Vector2& lhs, float rhs)
{
	return Vector2(
		lhs.x + rhs,
		lhs.y + rhs
	);
}

inline Vector2 operator+(float lhs, const Vector2& rhs)
{
	return rhs + lhs;
}

// Vector2 - Vector2
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Vector2 - int
inline Vector2 operator-(const Vector2& lhs, int rhs)
{
	return Vector2(lhs.x - rhs, lhs.y - rhs);
}

inline Vector2 operator-(int lhs, const Vector2& rhs)
{
	return Vector2(lhs - rhs.x, lhs - rhs.y);
}

// Vector2 - float
inline Vector2 operator-(const Vector2& lhs, float rhs)
{
	return Vector2(
		lhs.x - rhs,
		lhs.y - rhs
	);
}

inline Vector2 operator-(float lhs, const Vector2& rhs)
{
	return Vector2(
		lhs - rhs.x,
		lhs - rhs.y
	);
}

// Vector2 * Vector2
inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

// Vector2 * int
inline Vector2 operator*(const Vector2& lhs, int rhs)
{
	return Vector2(lhs.x * rhs, lhs.y * rhs);
}

inline Vector2 operator*(int lhs, const Vector2& rhs)
{
	return rhs * lhs;
}

// Vector2 * float
inline Vector2 operator*(const Vector2& lhs, float rhs)
{
	return Vector2(
		lhs.x * rhs,
		lhs.y * rhs
	);
}

inline Vector2 operator*(float lhs, const Vector2& rhs)
{
	return rhs * lhs;
}

// Vector2 / Vector2
inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
}

// Vector2 / int
inline Vector2 operator/(const Vector2& lhs, int rhs)
{
	return Vector2(lhs.x / rhs, lhs.y / rhs);
}

inline Vector2 operator/(int lhs, const Vector2& rhs)
{
	return Vector2(lhs / rhs.x, lhs / rhs.y);
}

// Vector2 / float
inline Vector2 operator/(const Vector2& lhs, float rhs)
{
	return Vector2(
		lhs.x / rhs,
		lhs.y / rhs
	);
}

inline Vector2 operator/(float lhs, const Vector2& rhs)
{
	return Vector2(
		lhs / rhs.x,
		lhs / rhs.y
	);
}