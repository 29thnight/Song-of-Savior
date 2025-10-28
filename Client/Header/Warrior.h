#pragma once
#include <MainEntry.h>
#include "Character.h"

namespace Engine
{
	CORECLASS()
	class BasicAnimationComponent;
}

namespace Client
{
	class NoteManager;
	class Boss;
	class HPComponent;
	class Warrior final : public Character
	{
	private:
		Warrior() = default;
		virtual ~Warrior() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;
		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;

	public:
		static Warrior* Create();
		void Destroy() override;

	public:
 		void Idle();
		void Attack();
		void VFXAttack();
		void SoundAttack();
		void Skill();
		void SkillQuake();
		void DeleteSkill();
		

	private:
		NoteManager* _pNoteManager{ nullptr };
		Engine::BasicAnimationComponent* _pAttackEffectComponent{ nullptr };
		Engine::TimerComponent* _pTimerComponent{ nullptr };
		Engine::BitmapComponent* _pBitmapComponent{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };
		Boss* _pBoss{ nullptr };

	private:
		int  _iSkillDemandNote = { 0 };
		int  _iNotePoint = { 0 };
	};
}
