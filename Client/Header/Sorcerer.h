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
	class Sorcerer final : public Character
	{
	private:
		Sorcerer() = default;
		virtual ~Sorcerer() = default;

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override;

		virtual void SetupInputComponent(Engine::InputComponent* inputComponent) override;
	public:
		static Sorcerer* Create();
		void Destroy() override;

		//test
		Engine::BitmapComponent* _pBitmapComponent{ nullptr };
		Engine::BasicAnimationComponent* _pSkillEffectMagicCircleComponent{ nullptr };

		NoteManager* _pNoteManager{ nullptr };
		Engine::InputComponent* _pInputComponent{ nullptr };

		int lightningCount{};
		long double lightningStartPosition{};

		Mathf::Vector2 bossPosition{};

		bool _isUseSkill = false;

		Boss* _pBoss{ nullptr };

	public:
		void Idle();
		void Attack();
		void SoundAttack();
		void Skill();
		void SoundSkill();
		//void Dead();

	private:
		int  _iSkillDemandNote = { 0 };
		int  _iNotePoint = { 0 };
	};
}
