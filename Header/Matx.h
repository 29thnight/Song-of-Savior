#pragma once
#include <CoreDefine.h>
#include <DirectXMath.h>
#include <Vector2.h>

namespace Mathf
{
	typedef float AngleF;
	typedef D2D1_MATRIX_3X2_F Matx3F;
	typedef D2D1_POINT_2F Point2F;

	inline Matx3F Identity()
	{
		return D2D1::Matrix3x2F::Identity();
	}

	inline Matx3F Translation(Vector2 translationVector)
	{
		return D2D1::Matrix3x2F::Translation(translationVector.x, translationVector.y);
	}

	inline Matx3F Rotation(AngleF angle)
	{
		return D2D1::Matrix3x2F::Rotation(angle);
	}

	inline Matx3F Scale(Vector2 scaleVector)
	{
		return D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleVector.x, scaleVector.y));
	}

	inline Matx3F Scale(Vector2 scaleVector, Point2F point)
	{
		return D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleVector.x, scaleVector.y),D2D1::Point2F(point.x, point.y));
	}

	inline Matx3F Scale(float scale)
	{
		return D2D1::Matrix3x2F::Scale(D2D1::SizeF(scale, scale));
	}
}

namespace Matx
{
	const Mathf::Matx3F Identity = Mathf::Identity();
}