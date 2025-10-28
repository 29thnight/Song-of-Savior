#pragma once
#include <Actor.h>

namespace Engine
{
	CORECLASS()
	class CameraActor : public Actor
	{
	public:
		enum MoveDirectionFlag
		{
			Up = 1 << 0,
			Down = 1 << 1,
			Left = 1 << 2,
			Right = 1 << 3,
		};
	protected:
		explicit CameraActor() = default;
		virtual ~CameraActor() = default;
	public:
		//Actor
		virtual void BeginPlay() override {};
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override {};
		CameraComponent* GetCameraComponent() { return _cameraComponent; }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void TrackTarget(Actor* pTarget);
		void SetCameraLerpFactor(float lerpFactor) { _cameraLerpFactor = lerpFactor; }
		void SettingTracking(bool isTracking) { _isTracking = isTracking; }
		bool IsTracking() const { return _isTracking; }
		bool IsMove() const { return _isMove; }

	public:
		void FadeIn(float duration);
		void FadeOut(float duration);
		void FadeOutIn(float totalDuration, Color color = Color::Black);
		void FadeEnd();
		void CameraShake(float duration, float magnitude);
		void ZoomIn();
		void ZoomOut();

	public:
		bool IsFadeIn() const;
		bool IsFadeOut() const;


	protected:
		virtual bool Initialize();
		virtual void Destroy() override;

	public:
		static CameraActor* Create();

	protected:
		CameraComponent*	_cameraComponent{ nullptr };
		Mathf::Vector2		_oldLocation{ UnitVector::One };
		float				_cameraLerpFactor{ 3.5f };
		_uint				_moveDirectionFlag{};
		bool				_isTracking{ false };
		bool				_isMove{ false };
	};
}