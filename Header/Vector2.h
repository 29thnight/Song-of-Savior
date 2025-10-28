#pragma once
#include <CoreDefine.h>
#include <DirectXMath.h>
#include <d2d1_1helper.h>

namespace Mathf
{
	struct Vector2 : public D2D1_VECTOR_2F
	{
		explicit Vector2(){ x = 0; y = 0; };
		Vector2(float x, float y) : D2D1_VECTOR_2F{ x, y } {}
		Vector2(long x, long y) : D2D1_VECTOR_2F{ static_cast<float>(x), static_cast<float>(y) } {}
		Vector2(const D2D1_VECTOR_2F& rhs) : D2D1_VECTOR_2F{ rhs.x, rhs.y } {}

		Vector2(const Vector2& rhs) = default;
		Vector2(Vector2&& rhs) = default;
		~Vector2() = default;

		ReadOnly_Property(float, Length)
		_Get(Length)
		{
			return sqrtf(x * x + y * y);
		}
		
		Vector2 Normalize()
		{
			Vector2 result{ 0.f, 0.f };
			float length = (*this).Length;
			if(Vector2{0.f, 0.f} == *this)
			{
				return result;
			}

			result.x = x / length;
			result.y = y / length;

			return result;
		}

		Vector2 operator=(const Vector2& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}

		bool operator==(const Vector2& rhs) const
		{
			return (x == rhs.x) && (y == rhs.y);
		}

		bool operator!=(const Vector2& rhs) const
		{
			return (x != rhs.x) || (y != rhs.y);
		}

		bool operator<(const Vector2& rhs) const
		{
			return (x < rhs.x) && (y < rhs.y);
		}

		bool operator<=(const Vector2& rhs) const
		{
			return (x <= rhs.x) && (y <= rhs.y);
		}

		bool operator>(const Vector2& rhs) const
		{
			return (x > rhs.x) && (y > rhs.y);
		}

		bool operator>=(const Vector2& rhs) const
		{
			return (x >= rhs.x) && (y >= rhs.y);
		}

		Vector2 operator+(const Vector2& rhs) 
		{
			return Vector2{ x + rhs.x, y + rhs.y };
		}

		Vector2 operator+(const float& rhs) 
		{
			return Vector2{ x + rhs, y + rhs };
		}

		Vector2 operator-(const Vector2& rhs) 
		{
			return Vector2{ x - rhs.x, y - rhs.y };
		}

		Vector2 operator-(const float& rhs) 
		{
			return Vector2{ x - rhs, y - rhs };
		}

		Vector2 operator*(const float& rhs)
		{
			return Vector2{ x * rhs, y * rhs };
		}

		Vector2 operator/(const float& rhs)
		{
			return Vector2{ x / rhs, y / rhs };
		}

		Vector2 operator+=(const Vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2 operator-=(const Vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2 operator*=(const float& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		Vector2 operator/=(const float& rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

		Vector2 operator-()
		{
			return Vector2{ -x, -y };
		}
	};

	inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
	}

	inline Vector2 operator*(const Vector2& lhs, const float& rhs)
	{
		return Vector2{ lhs.x * rhs, lhs.y * rhs };
	}

	inline float Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	inline Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
	{
		return start + (end - start) * t;
	}

	inline Vector2 Reflect(const Vector2& in, const Vector2& normal)
	{
		return in - normal * 2.f * Dot(in, normal);
	}

	inline Vector2 Abs(const Vector2& absVector)
	{
		return Vector2{ abs(absVector.x), abs(absVector.y) };
	}
}

namespace UnitVector
{
	const Mathf::Vector2 Zero{ 0.f, 0.f };
	const Mathf::Vector2 One{ 1.f, 1.f };
	const Mathf::Vector2 Up{ 0.f, -1.f };
	const Mathf::Vector2 Down{ 0.f, 1.f };
	const Mathf::Vector2 Left{ -1.f, 0.f };
	const Mathf::Vector2 Right{ 1.f, 0.f };
}