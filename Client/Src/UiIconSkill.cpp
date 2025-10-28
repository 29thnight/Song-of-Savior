#include <BitmapComponent.h>
#include <TextRenderComponent.h>
#include "GameManager.h"
#include "Character.h"
#include "Boss.h"
#include "UiIconSkill.h"

void Client::UiIconSkill::BeginPlay()
{
	if (_pCharacter)  //연결된 케릭터가 있다면
	{
		switch (_pCharacter->Type) {  //케릭터 타입에 따라 텍스쳐 이름 변경
			case (int)CharacterType::Warrior:	//전사
				_textureName = L"WarriorSkillIcon";
				break;
			case (int)CharacterType::Thief:	//도적
				_textureName = L"ThiefSkillIcon";
				break;
			case (int)CharacterType::Sorcerer:	//마법사
				_textureName = L"SorcererSkillIcon";
				break;
			case (int)CharacterType::Bard:	//음유시인
				_textureName = L"BardSkillIcon";
				break;
		}
	}

	UI::BeginPlay();  //UI BeginPlay  => _textureName으로 텍스쳐 로드

	_pPrimaryBitmap->SetBitmapIndex(0); // defualt => 쿨다운 상태 텍스쳐
	_pSecondaryBitmap->SetBitmapIndex(4); // defualt => 키 버튼 UI 텍스쳐
	_pSecondaryBitmap->SetRelativeLocation({ -80.f, 0.f }); //키 버튼 UI 위치 조정
	_pSecondaryBitmap->SetOpacity(1.f); //키 버튼 UI 투명도 조정

	_pTextRenderComponent = AddComponent<Engine::TextRenderComponent>("TextRenderComponent"); //텍스트 렌더 컴포넌트 추가
	_pTextRenderComponent->SetRelativeLocation({ -80.f, 0.f }); //텍스트 렌더 위치 조정
	_pTextRenderComponent->ChooseBrush("Default");
	_pTextRenderComponent->SetFont(L"MapoGoldenPier");
	_pTextRenderComponent->SetFontSize(30.f);
	_pTextRenderComponent->SetSize({ 25.f, 25.f });
	_pTextRenderComponent->SetHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	_pTextRenderComponent->SetVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}


void Client::UiIconSkill::Tick(_duration deltaSeconds)
{
	UI::Tick(deltaSeconds);

	_pPrimaryBitmap->SetVisible(true);  //스킬 아이콘 항시 보임
	_pSecondaryBitmap->SetVisible(true); //키 버튼 UI 항시 보임

	if (_pCharacter)  //연결된 케릭터가 있다면
	{
		if (_pCharacter)
		{
			_pTextRenderComponent->Text = _pCharacter->GetKeySetting();
		}

		if(_pBoss)  //연결된 보스가 있다면
		{
			//todo : 
			// if(캐릭터 스킬 사용가능 여부 확인)
			if (_pCharacter->IsSkillOn)
			{
			//		if(todo : 보스 차지 상태 확인){
				if (_pBoss->CharginCancel == (int)_pCharacter->Type)
				{
		 			_skillState = SkillState::ACTIVE_CHARGE;  //활성화 중에 보스 차지 상태로 변경	
				}else{
					_skillState = SkillState::ACTIVE;			//활성화 상태로 변경
				}
			}
			else{
				if (_pBoss->CharginCancel == (int)_pCharacter->Type){

					_skillState = SkillState::COLLDOWN_CHARGE;  //쿨다운 중에 보스 차지 상태로 변경
				}else{
					_skillState = SkillState::COLLDOWN;  //쿨다운 상태로 변경
				}
			}	
		}
		else
		{	
			//보스가 없다면
			_skillState = SkillState::COLLDOWN;  //비활성화 상태로 변경
		}
	}
	else {
		_pPrimaryBitmap->SetVisible(false);  //연결된 케릭터가 없다면 비활성화
		_pSecondaryBitmap->SetVisible(false); //연결된 케릭터가 없다면 비활성화
	}

	switch (_skillState)  //스킬 상태에 따라 텍스쳐 변경
	{
	case SkillState::COLLDOWN:               //쿨다운 상태
		_pPrimaryBitmap->SetBitmapIndex(0);
		break;
	case SkillState::COLLDOWN_CHARGE:		 //쿨다운 중에 보스 차지
		_pPrimaryBitmap->SetBitmapIndex(1);
		break;
	case SkillState::ACTIVE:				 //활성화
		_pPrimaryBitmap->SetBitmapIndex(2);
		break;
	case SkillState::ACTIVE_CHARGE:			 //활성화 중에 보스 차지
		_pPrimaryBitmap->SetBitmapIndex(3);
		break;
	}
}

void Client::UiIconSkill::Fixed()
{
}

void Client::UiIconSkill::EndPlay()
{
}

void Client::UiIconSkill::Destroy()
{
	UI::Destroy();
}
