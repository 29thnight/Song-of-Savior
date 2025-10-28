#pragma once
#include <CoreDefine.h>
#include <UI.h>

namespace Engine
{
	CORECLASS();
}

namespace Client
{
	enum class SkillState  //표시되어야 할 스킬 상태
	{
		COLLDOWN,			//쿨다운
		COLLDOWN_CHARGE,	//쿨다운 중에 보스 차지
		ACTIVE,				//활성화
		ACTIVE_CHARGE,		//활성화 중에 보스 차지
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
		// UI::_pPrimaryBitmap  //스킬 아이콘
		// UI::_pSecondaryBitmap //키 버튼 UI 

		//Texture : 0~4 
		//0 : 쿨다운 상태 텍스쳐
		//1 : 쿨다운 중에 보스 차지 텍스쳐
		//2 : 활성화 텍스쳐
		//3 : 활성화 중에 보스 차지 텍스쳐
		//4 : 키 버튼 UI 텍스쳐

		Boss* _pBoss{ nullptr };	//차지 상태를 알려줄 보스
		Character* _pCharacter{ nullptr };	//스킬 상태를 알려줄 캐릭터
		SkillState _skillState{ SkillState::COLLDOWN };	//해당 아이콘 스킬 상태

		Engine::TextRenderComponent* _pTextRenderComponent{ nullptr };	//바인딩 키 표시용 
	};
}
