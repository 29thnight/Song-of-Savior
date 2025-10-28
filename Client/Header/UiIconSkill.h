#pragma once
#include <CoreDefine.h>
#include <UI.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	enum class SkillState  //ǥ�õǾ�� �� ��ų ����
	{
		COLLDOWN,			//��ٿ�
		COLLDOWN_CHARGE,	//��ٿ� �߿� ���� ����
		ACTIVE,				//Ȱ��ȭ
		ACTIVE_CHARGE,		//Ȱ��ȭ �߿� ���� ����
	};

	class UiIconSkill : public UI
	{
	private:
		UiIconSkill() = default;
		virtual ~UiIconSkill() = default;

	public:
		void BeginPlay() override;
		void Tick(_duration deltaSeconds) override;
		void Fixed() override;
		void EndPlay() override;

	public:
		static UiIconSkill* Create() { return new UiIconSkill; }
		void Destroy() override;

	public:
		void SetCharacter(Character* pCharacter) { _pCharacter = pCharacter; }
		void SetBoss(Boss* pBoss) { _pBoss = pBoss; }

	private:
		// UI::_pPrimaryBitmap  //��ų ������
		// UI::_pSecondaryBitmap //Ű ��ư UI 

		//Texture : 0~4 
		//0 : ��ٿ� ���� �ؽ���
		//1 : ��ٿ� �߿� ���� ���� �ؽ���
		//2 : Ȱ��ȭ �ؽ���
		//3 : Ȱ��ȭ �߿� ���� ���� �ؽ���
		//4 : Ű ��ư UI �ؽ���

		Boss* _pBoss{ nullptr };	//���� ���¸� �˷��� ����
		Character* _pCharacter{ nullptr };	//��ų ���¸� �˷��� ĳ����
		SkillState _skillState{ SkillState::COLLDOWN };	//�ش� ������ ��ų ����

		Engine::TextRenderComponent* _pTextRenderComponent{ nullptr };	//���ε� Ű ǥ�ÿ� 
	};
}
