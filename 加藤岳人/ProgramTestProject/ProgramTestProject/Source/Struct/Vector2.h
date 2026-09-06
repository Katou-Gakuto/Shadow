#pragma once

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

	Vector2_Int operator +(Vector2_Int src)
	{
		Vector2_Int vec;

		vec.x = this->x + src.x;
		vec.y = this->y + src.y;

		return vec;
	}

	Vector2_Int operator -(Vector2_Int src)
	{
		Vector2_Int vec;

		vec.x = this->x - src.x;
		vec.y = this->y - src.y;

		return vec;
	}

	Vector2_Int operator +=(Vector2_Int src)
	{
		this->x += src.x;
		this->y += src.y;

		return *this;
	}

	Vector2_Int operator -=(Vector2_Int src)
	{
		this->x -= src.x;
		this->y -= src.y;

		return *this;
	}

	Vector2_Int operator *(float src)
	{
		Vector2_Int vec;

		vec.x = static_cast<int>(static_cast<float>(this->x) * src);
		vec.y = static_cast<int>(static_cast<float>(this->y) * src);

		return vec;
	}

	Vector2_Int operator *(int src)
	{
		Vector2_Int vec;

		vec.x = this->x * src;
		vec.y = this->y * src;

		return vec;
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

	Vector2 operator +(Vector2 src) const
	{
		Vector2 vec;

		vec.x = this->x + src.x;
		vec.y = this->y + src.y;

		return vec;
	}

	Vector2 operator +=(Vector2 src)
	{
		this->x += src.x;
		this->y += src.y;

		return *this;
	}

	Vector2 operator -(Vector2 src) const
	{
		Vector2 vec;

		vec.x = this->x - src.x;
		vec.y = this->y - src.y;

		return vec;
	}

	Vector2 operator -=(Vector2 src)
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