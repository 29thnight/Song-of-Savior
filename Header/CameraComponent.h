#pragma once
//#include <SceneComponent.h>
#include <RenderComponent.h>
#include <ObjectPtr.h>
#include <ACollision.h>
#include <Random.h>

namespace Engine
{
	CORECLASS()
	class CameraComponent : public RenderComponent
	{
	protected:
		explicit CameraComponent() = default;
		virtual ~CameraComponent() = default;
	public:
		struct FadeSetting
		{
			float	opacity{};
			float	fadeInDuration{};
			float   fadeOutDuration{};
			float	currentTime{};
			float	fadeSize{};
			bool	isFadeIn{ false };
			bool	isFadeOut{ false };
		};

		struct CameraShakeSetting
		{
			std::vector<float> shakeOffset{};
			float	duration{};
			float	magnitude{};
			float	currentTime{};
			float	cycleTime{};
			float	cycleTick{};
			int		shakeIndex{};
			bool	isShake{ false };
		};

		enum class CameraZoom
		{
			Idle,
			ZoomIn,
			ZoomOut
		};

	public:
		virtual bool InitializeComponent() override;
		virtual void BeginPlay() override {};
		virtual void TickComponent(_duration deltaSeconds) override;
		virtual void Render(_RenderTarget pRenderTarget) override;
		virtual void EndPlay() override {};

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void SetCenterPosition(Mathf::Vector2 centerPosition) { _centerPosition = centerPosition; }
		Mathf::Vector2 GetCenterPosition() const { return _centerPosition; }

		void SetCameraSize(Mathf::Vector2 cameraSize) { _cameraSize = cameraSize; }
		Mathf::Vector2 GetCameraSize() const { return _cameraSize; }

		void SetCameraOffset(Mathf::Vector2 cameraOffset) { _cameraOffset = cameraOffset; }
		Mathf::Vector2 GetCameraOffset() const { return _cameraOffset; }

		void SetCameraZoomScale(Mathf::Vector2 cameraZoomScale) { _cameraZoomScale = cameraZoomScale; }
		Mathf::Vector2 GetCameraZoomScale() const { return _cameraZoomScale; }

		bool CheckCollision(ACollision* pCollision);

	public:
		void CameraShake(float duration, float magnitude, float cycleTick = 0.01f, int randomAngle = 10);

	public:
		void StartFadeIn(float duration);
		void StartFadeOut(float duration);
		void StartFadeOutIn(float duration, Color color);
		void SetColor(Color color);
		void EndFade();

	public:
		bool IsFadeIn() const { return _fadeSetting.isFadeIn; }
		bool IsFadeOut() const { return _fadeSetting.isFadeOut; }

	protected:
		virtual void Destroy();

	public:
		static CameraComponent* Create();

	private:
		Mathf::Matx3F		  _cameraMatrix	   { Matx::Identity   };
		Mathf::Vector2		  _cameraZoomScale { UnitVector::One  };
		Mathf::Vector2		  _centerPosition  { UnitVector::Zero };
		Mathf::Vector2		  _cameraSize	   { UnitVector::Zero };
		Mathf::Vector2		  _cameraOffset	   { UnitVector::Zero };

	private:
		ACollision*			  _pCollision{ nullptr };
		FadeSetting			  _fadeSetting{};
		CameraZoom			  _cameraZoom{ CameraZoom::Idle };
		CameraShakeSetting	  _cameraShakeSetting{};
		Brush				  _pBrush{ nullptr };
		Random<float>		  _random{ 0.f, 360.f };

	private:
		bool				  _isBlack{ false };
	};
}