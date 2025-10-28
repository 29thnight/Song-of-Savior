#pragma once
#include <CoreDefine.h>
#include <Mathf.h>

namespace Engine
{
	CORECLASS()
	class Transform abstract
	{
	protected:
		explicit Transform() = default;
		virtual ~Transform() = default;

	public:
		virtual void UpdateTransform() PURE;
		virtual void UpdateVelocity() PURE;

		//��� ������ ��
		void SetRelativeScale(Mathf::Vector2 scale) { _RelativeScale = scale; }
		void AddRelativeScale(Mathf::Vector2 scale) { _RelativeScale += scale; }
		const Mathf::Vector2& GetRelativeScale() const { return _RelativeScale; }

		//��� ȸ��
		void SetRelativeRotation(float rotation) { _RelativeRotation = rotation; }
		void AddRelativeRotation(float rotation);
		float GetRelativeRotation() const { return _RelativeRotation; }

		//��� ��ġ
		void SetRelativeY(float y) { _RelativeLocation.y = y; }
		void SetRelativeLocation(Mathf::Vector2 location) { _RelativeLocation = location; }
		void AddRelativeLocation(Mathf::Vector2 location) { _RelativeLocation += location; }
		const Mathf::Vector2& GetRelativeLocation() const { return _RelativeLocation; }

		//��� ��ġ�� �������� ȸ��, ������, ��ġ�� ������ ���
		const Mathf::Matx3F& GetWorldTransform() const { return _WorldTransform; }
		Mathf::Vector2 GetWorldLocation();

		//�ӵ� �� ���⺤��
		void SetVelocity(const Mathf::Vector2& velocity) { _ComponentVelocity = velocity; }
		const Mathf::Vector2& GetVelocity() const { return _ComponentVelocity; }
	
	protected:
		Mathf::Matx3F		_MidCalculateTransform{ Matx::Identity };
		Mathf::Matx3F		_CenterTransform{ Matx::Identity };
		Mathf::Matx3F		_RelativeTransform{ Matx::Identity };
		Mathf::Matx3F		_WorldTransform{ Matx::Identity };
		Mathf::Vector2		_WorldLocation{ UnitVector::Zero };
		Mathf::Vector2		_RelativeLocation{ UnitVector::Zero };
		Mathf::Vector2		_RelativeScale{ UnitVector::One };
		Mathf::Vector2		_ComponentVelocity{ UnitVector::Zero };
		float				_RelativeRotation{ 0.f };
		float				_LocalRotation{ 0.f };
	};
}