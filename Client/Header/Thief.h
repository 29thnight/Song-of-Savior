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


		Mathf::Vector2 _vInitPosition{}; //캐릭터 초기 위치
		Mathf::Vector2 _vInitKnifePosition{}; //캐릭터 초기 위치

		Boss* _pBoss{ nullptr };

		long double prevPosition{}; //스킬 틱타임계산
		float _fSkillTime{}; //스킬 진행 시간

		long double _fKnifePrevTime{}; //칼 던지기 틱 타임 계산
		long double _fThrowKnifeTime{}; //칼던지기 진행 시간

		bool _isHideAndBush{ false };


	public:
		void Idle(); //대기모션
		void Attack();  //공격 모션 시작
		void AttackUpdate(); // 공격 모션 업데이트 및 종료
		//void SoundAttack(); //공격 사운드
		void Skill(); //스킬 모션 시작
		void SkillUpdate(); //스킬 모션 업데이트 및 종료
		void SoundSkill(); //스킬 사운드
		//void Damage(); //피격
		//void Dead(); //죽음

		void InitPosition();

		void VisibleKnife();
		void ThrowKnife(float duration);

	private:
		int  _iSkillDemandNote = { 0 };
		int  _iNotePoint = { 0 };
	};
}
