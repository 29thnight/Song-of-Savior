#pragma once
#include <MainEntry.h>
#include "Character.h"

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class NoteManager;
	class Boss;
	class Thief final : public Character
	{
	private:
		Thief() = default;
		virtual ~Thief() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
	public:
		static Thief* Create();
		void Destroy() override;

		//test
		Engine::BitmapComponent* _pBitmapComponent{ nullptr };

		Engine::BitmapComponent* _pBitmapKnifeComponent{ nullptr };

		NoteManager* _pNoteManager{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };

		Engine::TimerComponent* _pTimerManager = nullptr;


		Mathf::Vector2 _vInitPosition{}; //ĳ���� �ʱ� ��ġ
		Mathf::Vector2 _vInitKnifePosition{}; //ĳ���� �ʱ� ��ġ

		Boss* _pBoss{ nullptr };

		long double prevPosition{}; //��ų ƽŸ�Ӱ��
		float _fSkillTime{}; //��ų ���� �ð�

		long double _fKnifePrevTime{}; //Į ������ ƽ Ÿ�� ���
		long double _fThrowKnifeTime{}; //Į������ ���� �ð�

		bool _isHideAndBush{ false };


	public:
		void Idle(); //�����
		void Attack();  //���� ��� ����
		void AttackUpdate(); // ���� ��� ������Ʈ �� ����
		//void SoundAttack(); //���� ����
		void Skill(); //��ų ��� ����
		void SkillUpdate(); //��ų ��� ������Ʈ �� ����
		void SoundSkill(); //��ų ����
		//void Damage(); //�ǰ�
		//void Dead(); //����

		void InitPosition();

		void VisibleKnife();
		void ThrowKnife(float duration);

	private:
		int  _iSkillDemandNote = { 0 };
		int  _iNotePoint = { 0 };
	};
}
