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
		int _iSkillCancelType = { 0 }; //��ų ĵ�� Ÿ��
		//���� 2��° ��ų
		int _iBossSkill = { 0 };  
		long double _ldBossSkillOffset = { 0 };
		long double _ldBossSkillCharging = { 0 };
		int _iBossSkillCancelType = { 0 }; //���� 2��° ��ų ĵ�� Ÿ��

		OnAttackFail _OnAttackFail;
		OnSkillFail _OnSkillFail;
		
		int _iCharginCancel = { -1 };	//���� ��¡ ���� ��ų�� ĵ��Ÿ�� //4��::All �ɸ��� �Ҵ� ����

		int _iIndicateCount = { -1 }; //��ų ĵ���� ǥ�ÿ� ī��Ʈ
		int _SkillIndicateCount = { -1 }; //��ų ĵ���� ǥ�ÿ� ī��Ʈ
		int _BossSkillIndicateCount = { -1 }; //��ų ĵ���� ǥ�ÿ� ī��Ʈ
	protected:
		Engine::BasicAnimationComponent* _pSkill2EffectComponent = nullptr;

	protected:
		int  _attackFailCount = { 0 }; // ��ȹ������ �����ص� ����
		int  _skillFailCount = { 0 };

	protected:
		bool _bIsAttackFail = { false }; //���� �����
		bool _bIsSkillFail = { false };  //��ų ĵ����
		bool _bIsBossSkillFail = { false };  //��ų ĵ����

		bool _bIsSkillCharging = { false }; //��ų ��¡������
		long double _ldChagingTime = { 0 }; // ��¡ �ð� ����� 
		int _skillPatten{};
	};
};