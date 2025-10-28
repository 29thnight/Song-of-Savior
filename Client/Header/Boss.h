#pragma once
#include <MainEntry.h>

#include "Character.h"

namespace Engine
{
	CORECLASS()
}

namespace Client
{
	class Boss abstract : public Character
	{
	protected:
		Boss() = default;
		virtual ~Boss() = default;

	private:
		DECLARE_DELEGATE(OnAttackFail, bool);
		DECLARE_DELEGATE(OnSkillFail, int);

	public:
		virtual void BeginPlay() override;
		virtual void Tick(_duration deltaSeconds) override;
		virtual void Fixed() override;
		virtual void EndPlay() override;
		virtual void ReviveInitialize() override abstract; 

	public:
		void Destroy() override abstract;
		
	public:
		virtual void StartPattern(int PatternNumber);
		long double GetPattenOffset(int pattenType);

		void Dead() override;

		void SetAttackFail(bool isFail);
		void SetSkillFail(int _CharacterType);

		void Charging();

		virtual void UseSkill1() abstract;
		virtual void UseSkill2() abstract;

		ReadOnly_Property(int, CharginCancel);
		_Get(CharginCancel) { return _iCharginCancel; }

		ReadOnly_Property(int, IndicateCount);
		_Get(IndicateCount) { return _iIndicateCount; }

		
		long double _ldSkillCharging= { 0 };
		int _iSkillCancelType = { 0 }; //스킬 캔슬 타입
		//보스 2번째 스킬
		int _iBossSkill = { 0 };  
		long double _ldBossSkillOffset = { 0 };
		long double _ldBossSkillCharging = { 0 };
		int _iBossSkillCancelType = { 0 }; //보스 2번째 스킬 캔슬 타입

		OnAttackFail _OnAttackFail;
		OnSkillFail _OnSkillFail;
		
		int _iCharginCancel = { -1 };	//현제 차징 중인 스킬의 캔슬타입 //4번::All 케릭터 할당 없음

		int _iIndicateCount = { -1 }; //스킬 캔슬시 표시용 카운트
		int _SkillIndicateCount = { -1 }; //스킬 캔슬시 표시용 카운트
		int _BossSkillIndicateCount = { -1 }; //스킬 캔슬시 표시용 카운트
	protected:
		Engine::BasicAnimationComponent* _pSkill2EffectComponent = nullptr;

	protected:
		int  _attackFailCount = { 0 }; // 기획변경을 염두해둔 변수
		int  _skillFailCount = { 0 };

	protected:
		bool _bIsAttackFail = { false }; //공격 가드시
		bool _bIsSkillFail = { false };  //스킬 캔슬시
		bool _bIsBossSkillFail = { false };  //스킬 캔슬시

		bool _bIsSkillCharging = { false }; //스킬 차징중인지
		long double _ldChagingTime = { 0 }; // 차징 시간 세기용 
		int _skillPatten{};
	};
};