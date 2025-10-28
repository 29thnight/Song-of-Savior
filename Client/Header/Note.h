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
		long double _ldStartPosition = { 0 }; //���� ����
		long double _ldTargetPosition = { 0 };  //�Ҹ� ����
		long double _ldMissOffset = { 0 }; //�̽� ���� ����
		float _fMaxScale = { 3 }; //�ִ� ũ��
		float _fMinScale = { 1 }; //�ּ� ũ��
	private:

		Engine::BitmapComponent* _pBitmapComponent = nullptr; 
		Engine::InputComponent* _pInputComponent = nullptr;
	};
}
