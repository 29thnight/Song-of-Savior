#pragma once
#include <Vector2.h>
#include <Matx.h>

namespace Mathf
{
	typedef D2D1_RECT_F RectF;
	typedef D2D1_SIZE_F SizeF;
}

namespace Mathf
{
	const float Rad2Deg = 180.f / DirectX::XM_PI;
	const float Deg2Rad = DirectX::XM_PI / 180.f;
}

namespace Mathf
{
	constexpr long double SecToNano = (long double)1e9;
	constexpr long double NanoToSec = 1 / (long double)1e9;
}

namespace Mathf
{
	template<typename T>
	T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}
}

namespace Mathf
{
	template<typename T>
	T Sin(const T& value)
	{
		return std::sin(value);
	}

	template<typename T>
	T Cos(const T& value)
	{
		return std::cos(value);
	}
}