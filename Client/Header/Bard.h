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

	class Bard final : public Character
	{
	private:
		Bard() = default;
		virtual ~Bard() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
	public:
		static Bard* Create();
		void Destroy() override;

		//test
		Engine::TimerComponent* _pTimerComponent{ nullptr };
		Engine::BasicAnimationComponent* _pMagicCircleEffectComponent{ nullptr };
		NoteManager* _pNoteManager{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };

		Boss* _pBoss{ nullptr };

		Character* _pWarrior{ nullptr };
		Character* _pThief{ nullptr };
		Character* _pSorcerer{ nullptr };

	public:
		void Idle();
		void Attack();
		void SoundAttack();
		void Skill();
		void SkillExit();
		void SoundSkill();
		/*void Damage();
		void Dead();*/
		void Surprised();

	private:
		int  _iSkillDemandNote = { 0 };
		int  _iNotePoint = { 0 };
	};
}
