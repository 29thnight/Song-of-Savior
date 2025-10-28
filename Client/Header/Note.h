#pragma once
#include <MainEntry.h>
#include <Actor.h>

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Note : public Engine::Actor
	{
		using Base = Engine::Actor;
	private:
		Note() = default;
		virtual ~Note() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;

	public:
		void SetVisible(bool visible);
		bool GetVisible() const;

	public:
		static Note* Create();
		void Destroy() override;

	public:
		long double _ldStartPosition = { 0 }; //생성 지점
		long double _ldTargetPosition = { 0 };  //소멸 지점
		long double _ldMissOffset = { 0 }; //미스 판정 범위
		float _fMaxScale = { 3 }; //최대 크기
		float _fMinScale = { 1 }; //최소 크기
	private:

		Engine::BitmapComponent* _pBitmapComponent = nullptr; 
		Engine::InputComponent* _pInputComponent = nullptr;
	};
}
