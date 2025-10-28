#include <BitmapComponent.h>
#include <TextRenderComponent.h>
#include "GameManager.h"
#include "Character.h"
#include "Boss.h"
#include "UiIconSkill.h"

void Client::UiIconSkill::BeginPlay()
{
	if (_pCharacter)  //����� �ɸ��Ͱ� �ִٸ�
	{
		switch (_pCharacter->Type) {  //�ɸ��� Ÿ�Կ� ���� �ؽ��� �̸� ����
			case (int)CharacterType::Warrior:	//����
				_textureName = L"WarriorSkillIcon";
				break;
			case (int)CharacterType::Thief:	//����
				_textureName = L"ThiefSkillIcon";
				break;
			case (int)CharacterType::Sorcerer:	//������
				_textureName = L"SorcererSkillIcon";
				break;
			case (int)CharacterType::Bard:	//��������
				_textureName = L"BardSkillIcon";
				break;
		}
	}

	UI::BeginPlay();  //UI BeginPlay  => _textureName���� �ؽ��� �ε�

	_pPrimaryBitmap->SetBitmapIndex(0); // defualt => ��ٿ� ���� �ؽ���
	_pSecondaryBitmap->SetBitmapIndex(4); // defualt => Ű ��ư UI �ؽ���
	_pSecondaryBitmap->SetRelativeLocation({ -80.f, 0.f }); //Ű ��ư UI ��ġ ����
	_pSecondaryBitmap->SetOpacity(1.f); //Ű ��ư UI ���� ����

	_pTextRenderComponent = AddComponent<Engine::TextRenderComponent>("TextRenderComponent"); //�ؽ�Ʈ ���� ������Ʈ �߰�
	_pTextRenderComponent->SetRelativeLocation({ -80.f, 0.f }); //�ؽ�Ʈ ���� ��ġ ����
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

	_pPrimaryBitmap->SetVisible(true);  //��ų ������ �׽� ����
	_pSecondaryBitmap->SetVisible(true); //Ű ��ư UI �׽� ����

	if (_pCharacter)  //����� �ɸ��Ͱ� �ִٸ�
	{
		if (_pCharacter)
		{
			_pTextRenderComponent->Text = _pCharacter->GetKeySetting();
		}

		if(_pBoss)  //����� ������ �ִٸ�
		{
			//todo : 
			// if(ĳ���� ��ų ��밡�� ���� Ȯ��)
			if (_pCharacter->IsSkillOn)
			{
			//		if(todo : ���� ���� ���� Ȯ��){
				if (_pBoss->CharginCancel == (int)_pCharacter->Type)
				{
		 			_skillState = SkillState::ACTIVE_CHARGE;  //Ȱ��ȭ �߿� ���� ���� ���·� ����	
				}else{
					_skillState = SkillState::ACTIVE;			//Ȱ��ȭ ���·� ����
				}
			}
			else{
				if (_pBoss->CharginCancel == (int)_pCharacter->Type){

					_skillState = SkillState::COLLDOWN_CHARGE;  //��ٿ� �߿� ���� ���� ���·� ����
				}else{
					_skillState = SkillState::COLLDOWN;  //��ٿ� ���·� ����
				}
			}	
		}
		else
		{	
			//������ ���ٸ�
			_skillState = SkillState::COLLDOWN;  //��Ȱ��ȭ ���·� ����
		}
	}
	else {
		_pPrimaryBitmap->SetVisible(false);  //����� �ɸ��Ͱ� ���ٸ� ��Ȱ��ȭ
		_pSecondaryBitmap->SetVisible(false); //����� �ɸ��Ͱ� ���ٸ� ��Ȱ��ȭ
	}

	switch (_skillState)  //��ų ���¿� ���� �ؽ��� ����
	{
	case SkillState::COLLDOWN:               //��ٿ� ����
		_pPrimaryBitmap->SetBitmapIndex(0);
		break;
	case SkillState::COLLDOWN_CHARGE:		 //��ٿ� �߿� ���� ����
		_pPrimaryBitmap->SetBitmapIndex(1);
		break;
	case SkillState::ACTIVE:				 //Ȱ��ȭ
		_pPrimaryBitmap->SetBitmapIndex(2);
		break;
	case SkillState::ACTIVE_CHARGE:			 //Ȱ��ȭ �߿� ���� ����
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
