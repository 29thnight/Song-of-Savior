#include <World.h>
#include <Level.h>
#include <CoreManager.h>
#include <SoundManager.h>
#include <InputComponent.h>
#include <TimerComponent.h>
#include <TimeManager.h>
#include <GameManager.h>
#include <BasicAnimationComponent.h>
#include <UtfConvert.h>
#include "../ThirdParty/nlohmann/json.hpp"

#include "Boss.h"
#include "Warrior.h"
#include "Thief.h"
#include "Sorcerer.h"
#include "Bard.h"
#include "GameManager.h"
#include "Character.h"
#include "NoteManager.h"
#include "BossManager.h"
#include "BGMManager.h"
#include "BackGround.h"
#include "UiIconSkill.h"
#include "GameOverLevel.h"
#include "Setting.h"
#include "BossHP.h"
#include "CraditLevel.h"

void Client::GameManager::SpawnStageActor(Engine::Level* targetLevel)
{
	_isDefeat = false;

	if (!_isFirstSpawn)
	{
		_pSetting = targetLevel->SpawnActor(LAYER::UI, "Setting", Setting::Create(), Mathf::Vector2(960.f, 540.f));
		targetLevel->SpawnActor(LAYER::OBJECT, "NoteManager", NoteManager::Create(), Mathf::Vector2(433.f, 215.f));
		targetLevel->SpawnActor(LAYER::OBJECT, "BGMManager", BGMManager::Create(), Mathf::Vector2(500.f, 500.f));
		targetLevel->SpawnActor(LAYER::OBJECT, "BossManager", BossManager::Create(), Mathf::Vector2(500.f, 500.f));

		targetLevel->SpawnActor(LAYER::OBJECT, "Warrior", Warrior::Create(), Mathf::Vector2{ 960.f, 710.f });
		targetLevel->SpawnActor(LAYER::OBJECT, "Thief", Thief::Create(), Mathf::Vector2{ 750.f, 600.f });
		targetLevel->SpawnActor(LAYER::OBJECT, "Sorcerer", Sorcerer::Create(), Mathf::Vector2{ 500.f, 770.f });
		targetLevel->SpawnActor(LAYER::OBJECT, "Bard", Bard::Create(), Mathf::Vector2{ 280.f, 700.f });

		targetLevel->SpawnActor(LAYER::BACKGROUND, "BackGround", BackGround::Create(), Mathf::Vector2{ 960.f, 540.f });

		targetLevel->SpawnActor(LAYER::UI, "BardSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 280.f, 431.f });
		targetLevel->SpawnActor(LAYER::UI, "SorcererSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 518.f, 535.f });
		targetLevel->SpawnActor(LAYER::UI, "ThiefSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 742.f, 345.f });
		targetLevel->SpawnActor(LAYER::UI, "WarriorSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 961.f, 453.f });

		InitializeStage();
		BeginPlay();
		_isFirstSpawn = true;
	}
	else
	{
		_pSetting = targetLevel->SpawnActor(LAYER::UI, "Setting", Setting::Create(), Mathf::Vector2(960.f, 540.f));
		_pNoteManager = targetLevel->SpawnActor(LAYER::OBJECT, "NoteManager", NoteManager::Create(), Mathf::Vector2(300.f, 300.f));
		//targetLevel->SpawnActor(LAYER::BACKGROUND, "BackGround", BackGround::Create(), Mathf::Vector2{ 960.f, 540.f });
		targetLevel->ReviveActor("BackGround");
		targetLevel->ReviveActor("BGMManager");
		targetLevel->ReviveActor("BossManager");
		targetLevel->ReviveActor("BossHP");

		_pUiBardSkill = targetLevel->SpawnActor(LAYER::UI, "BardSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 280.f, 431.f });
		_pUiBardSkill->SetCharacter(_pBard);
		_pUiSorcererSkill = targetLevel->SpawnActor(LAYER::UI, "SorcererSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 518.f, 535.f });
		_pUiSorcererSkill->SetCharacter(_pSorcerer);
		_pUiThiefSkill = targetLevel->SpawnActor(LAYER::UI, "ThiefSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 742.f, 345.f });
		_pUiThiefSkill->SetCharacter(_pThief);
		_pUiWarriorSkill = targetLevel->SpawnActor(LAYER::UI, "WarriorSkillIcon", UiIconSkill::Create(), Mathf::Vector2{ 960.f, 580.f });
		_pUiWarriorSkill->SetCharacter(_pWarrior);

		ReviveAllCharacter();
		BeginPlay();
	}
}

void Client::GameManager::InitializeStage()
{
	_pWarrior = static_cast<Character*>(GetWorld()->FindActor("Warrior"));
	_pThief = static_cast<Character*>(GetWorld()->FindActor("Thief"));
	_pSorcerer = static_cast<Character*>(GetWorld()->FindActor("Sorcerer"));
	_pBard = static_cast<Character*>(GetWorld()->FindActor("Bard"));

	if (_pWarrior == nullptr || _pThief == nullptr || _pSorcerer == nullptr || _pBard == nullptr)
	{
		assert(!"캐릭터가 없습니다.");
	}

	_characterContainer.push_back(_pWarrior);
	_characterContainer.push_back(_pThief);
	_characterContainer.push_back(_pSorcerer);
	_characterContainer.push_back(_pBard);
		
	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	 if(_pNoteManager == nullptr)
	 { 
		 assert(!"노트 매니저가 없습니다.");
	 }

	_pBossManager = dynamic_cast<BossManager*>(GetWorld()->FindActor("BossManager"));
	 if (_pBossManager == nullptr)
	 {
		 assert(!"보스 매니저가 없습니다.");
	 }

	 _pBGMManager = dynamic_cast<BGMManager*>(GetWorld()->FindActor("BGMManager"));
	 if (_pBGMManager == nullptr)
	 {
		 assert(!"BGM 매니저가 없습니다.");
	 }

	 //skill ui 에 캐릭터 붙이기
	 _pUiWarriorSkill = static_cast<UiIconSkill*>(GetWorld()->FindActor("WarriorSkillIcon"));
	 _pUiWarriorSkill->SetCharacter(_pWarrior);
	 _pUiThiefSkill = static_cast<UiIconSkill*>(GetWorld()->FindActor("ThiefSkillIcon"));
	 _pUiThiefSkill->SetCharacter(_pThief);
	 _pUiSorcererSkill = static_cast<UiIconSkill*>(GetWorld()->FindActor("SorcererSkillIcon"));
	 _pUiSorcererSkill->SetCharacter(_pSorcerer);
	 _pUiBardSkill = static_cast<UiIconSkill*>(GetWorld()->FindActor("BardSkillIcon"));
	 _pUiBardSkill->SetCharacter(_pBard);

	 SetBindKeySetting(_currentKeySetting);
}

void Client::GameManager::InitializeGameManagerComponent()
{
	_name = "GameManager";
	_layerIndex = LAYER::UI;
	Super::BeginPlay();

	_pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");
	_pTimerComponent = AddComponent<Engine::TimerComponent>("TimerComponent");
	_pTimerBitmap = AddComponent<Engine::BitmapComponent>("TimerBitmap");
	_pTimerBitmap->SetTextures(&_vecTextures);
	_pTimerBitmap->SetRelativeLocation({ 960.f, 540.f });
	_pTimerBitmap->SetVisible(false);

	_pTimerComponent->AddTimer(6);
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(this, &GameManager::ChangeNextStage);
	_pTimerComponent->GetTimer(1).onTimeOver.AddDynamic(this, &GameManager::LoadingScreenEndAndFadeIn);
	_pTimerComponent->GetTimer(2).onTimeOver.AddDynamic(this, &GameManager::LoadingScreenView);
	_pTimerComponent->GetTimer(3).onTimeOver.AddDynamic(this, &GameManager::DelayedResumeGame);
	_pTimerComponent->GetTimer(4).onTimeOver.AddDynamic(this, &GameManager::PlayCountDown);
	_pTimerComponent->GetTimer(5).onTimeOver.AddDynamic(this, &GameManager::HPDefeat);

	_pTimerComponent->SetSyncTimeScale(3, false);
	_pTimerComponent->SetSyncTimeScale(4, false);
}

void Client::GameManager::ReadyToTransitionStage(Engine::Level* targetLevel)
{
	if(IsSceneChangeTrigger && targetLevel->IsEndPlay())
	{
		targetLevel->TerminateActor("NoteManager");
		targetLevel->TerminateActor("TimelineManager");
		targetLevel->TerminateActor("NoteEditor");
		targetLevel->RemoveActor("BackGround");
		targetLevel->RemoveActor("BGMManager");
		targetLevel->RemoveActor("BossManager");

		targetLevel->TerminateActor("BardSkillIcon");
		targetLevel->TerminateActor("ThiefSkillIcon");
		targetLevel->TerminateActor("SorcererSkillIcon");
		targetLevel->TerminateActor("WarriorSkillIcon");
		targetLevel->TerminateActor("BossHP");
		targetLevel->TerminateActor("Setting");
		targetLevel->TerminateActor("DefaultSetting");
		targetLevel->TerminateActor("KeySetting");
		targetLevel->TerminateActor("SoundBar");

		RemoveAllCharacters();

		targetLevel->SetEndPlay(false);
		Management->ScheduleDestroyPoint();
	}
}

void Client::GameManager::PauseGame()
{
	if (_pBGMManager->IsDelayedPlayBGM())
		return;

	if(!_isPaused)
	{
		Time->SetTimeScale(0.f);
		if (_pBGMManager)
		{
			_pBGMManager->PauseBGM();
		}
		_pSetting->SetVisible(true);
		_isPaused = true;
		DebugPrintf("Pause\n");
	}
}

void Client::GameManager::ResumeGame()
{
	if(_isPaused)
	{
		_pTimerComponent->SetTime(3, 4.f);
		_pTimerComponent->SetTime(4, 1.f, true);
		_isPaused = false;
		_pSetting->SetVisible(false);
	}
}

void Client::GameManager::DelayedResumeGame()
{
	Time->SetTimeScale(1.f);
	if (_pBGMManager)
	{
		_pBGMManager->PauseBGM();
	}
}

void Client::GameManager::PlayCountDown()
{
	DebugPrintf("PlayCountDown\n");
	static int count{};
	if (3 > count)
	{
		_pTimerBitmap->SetVisible(true);
		_pTimerBitmap->SetBitmapIndex(count);
		count++;
	}
	else
	{
		_pTimerBitmap->SetVisible(false);
		_pTimerComponent->StopTime(4);
		_gameStatus = GameStatus::Playing;
		count = 0;
	}
}

void Client::GameManager::SetRestart(bool value)
{
	_isRestart = value;  
	_isPaused = false;
}

void Client::GameManager::ReviveAllCharacter()
{
	for (auto character : _characterContainer)
	{
		_pOwnerWorld->ReviveActor(character->GetName());
	}

	if(!_isRestart)
	{
		switch (currentStage)
		{
		case Client::Stage::Stage3:
			AddAllCharacterMaxHP(1);
			break;
		case Client::Stage::Stage4:
			AddAllCharacterMaxHP(1);
			break;
		};
	}

	for (auto& character : _characterContainer)
	{
		character->IsDead = false;
		character->HP = character->MaxHP;
		character->MP = character->MaxMP;
		character->ReInitializeHPBar();
	}

	SetBindKeySetting(_currentKeySetting);

	_isRestart = false;
}

void Client::GameManager::DealDamageToCharacter(CharacterType target, int damage)
{
	if (strcmp(_pBoss->GetName(),"Lich"))
	{
		if(0 < _pWarrior->_iBlockCount)
		{
			_pWarrior->_iBlockCount--;
			DebugPrintf("iBlockCount %d\n",_pWarrior->_iBlockCount);
			return;
		}
	}
	else if (!strcmp(_pBoss->GetName(), "Lich"))
	{
		if (0 < _pWarrior->_iBlockCount)
		{
			_pWarrior->_iBlockCount = 0; //바로 삭제
			DebugPrintf("iBlockCount %d\n", _pWarrior->_iBlockCount);
		}
	}

	switch (target)
	{
	case CharacterType::Warrior:
		_pWarrior->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Thief:
		_pThief->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Sorcerer:
		_pSorcerer->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Bard:
		_pBard->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::All:
		for (auto character : _characterContainer)
		{
			character->_OnTakeDamage.Broadcast(damage);
		}
		break;
	}
}

void Client::GameManager::DealSkillDamageToCharacter(CharacterType target, int damage)
{
	switch (target)
	{
	case CharacterType::Warrior:
		_pWarrior->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Thief:
		_pThief->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Sorcerer:
		_pSorcerer->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::Bard:
		_pBard->_OnTakeDamage.Broadcast(damage);
		break;
	case CharacterType::All:
		for (auto character : _characterContainer)
		{
			character->_OnTakeDamage.Broadcast(damage);
		}
		break;
	}
}

void Client::GameManager::DealDamageToBoss(int damage, bool ableToAttackCancel)
{
	_pBoss->_OnTakeDamage.Broadcast(damage);
	_pBoss->_OnAttackFail.Broadcast(ableToAttackCancel);

	_pBossManager->GetBossHP()->UpdateHpScale();
}

void Client::GameManager::DealSkillDamageToBoss(int damage, CharacterType target)
{
	_pBoss->_OnTakeDamage.Broadcast(damage);
	_pBoss->_OnSkillFail.Broadcast((int)target);

	_pBossManager->GetBossHP()->UpdateHpScale();
}

void Client::GameManager::BardHeal()
{
	auto iter = std::max_element(_characterContainer.begin(), _characterContainer.end(),
				[](Character* a, Character* b) { 

					int aGap = a->MaxHP - a->HP;
					int bGap = b->MaxHP - b->HP;

					return aGap < bGap;
				});

	(*iter)->_OnHeal.Broadcast((*iter)->MaxHP);
	DebugPrintf("Bard Heal %d\n",(*iter)->HP);

	for (auto character : _characterContainer)
	{
		if (!strcmp((*character).GetName(),(*iter)->GetName()))
		{
			continue;
		}

		character->_OnHeal.Broadcast(1);
	}

}

void Client::GameManager::ChargeMana(CharacterType target, int mana)
{
	switch (target)
	{
	case CharacterType::Warrior:
		_pWarrior->_OnChargeMana.Broadcast(mana);
		break;
	case CharacterType::Thief:
		_pThief->_OnChargeMana.Broadcast(mana);
		break;
	case CharacterType::Sorcerer:
		_pSorcerer->_OnChargeMana.Broadcast(mana);
		break;
	case CharacterType::Bard:
		_pBard->_OnChargeMana.Broadcast(mana);
		break;
	case CharacterType::All:
		for (auto character : _characterContainer)
		{
			DebugPrintf("%s ChargeMana + %d",character->GetName(), mana);
			character->_OnChargeMana.Broadcast(mana);
		}
		break;
	}
}

void Client::GameManager::CancelBossAttack()
{
	_pBoss->_OnAttackFail.Broadcast(true);
}

void Client::GameManager::BeginPlay()
{
	_smashingInI.Load("Assets/Smashing.ini");

	switch (currentStage)
	{
	case Client::Stage::Editor: //에디터 스테이지 초기값
		_pBoss = _pBossManager->SpawnBoss(Stage::Editor);
		_pBGMManager->StopBGM();
		_pBGMManager->CurrentStage = 0;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->DelayedPlayBGM(3.0f);
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.01f * Mathf::SecToNano});
		// skill ui 에 보스붙이기
		_pUiWarriorSkill->SetBoss(_pBoss);
		_pUiThiefSkill->SetBoss(_pBoss);
		_pUiSorcererSkill->SetBoss(_pBoss);
		_pUiBardSkill->SetBoss(_pBoss);
		break;
	case Client::Stage::Stage1:
	case Client::Stage::Stage2:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage2);
		_pBossManager->SetNoteSpawnData("Assets/BossPattenCsv/Stage2/phase1.csv");

		_pBGMManager->StopBGM();
		_pBGMManager->CurrentStage = 0;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->DelayedPlayBGM(3.0f);
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.01f * Mathf::SecToNano });

		_pUiWarriorSkill->SetBoss(_pBoss);
		_pUiThiefSkill->SetBoss(_pBoss);
		_pUiSorcererSkill->SetBoss(_pBoss);
		_pUiBardSkill->SetBoss(_pBoss);

		break;
	case Client::Stage::Stage3:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage3);
		_pBossManager->SetNoteSpawnData("Assets/BossPattenCsv/Stage3/phase1.csv");

		_pBGMManager->StopBGM();
		_pBGMManager->CurrentStage = 1;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->DelayedPlayBGM(3.0f);
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.01f * Mathf::SecToNano });

		_pUiWarriorSkill->SetBoss(_pBoss);
		_pUiThiefSkill->SetBoss(_pBoss);
		_pUiSorcererSkill->SetBoss(_pBoss);
		_pUiBardSkill->SetBoss(_pBoss);

		break;
	case Client::Stage::Stage4:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage4);
		_pBossManager->SetNoteSpawnData("Assets/BossPattenCsv/Stage4/phase1.csv");

		_pBGMManager->StopBGM();
		_pBGMManager->CurrentStage = 2;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->DelayedPlayBGM(3.0f);
		_pNoteManager->SetSmashingPosition( {(Sound->GetLength(CHANNEL_BGM) - (long double)0.05f * Mathf::SecToNano) });

		_pUiWarriorSkill->SetBoss(_pBoss);
		_pUiThiefSkill->SetBoss(_pBoss);
		_pUiSorcererSkill->SetBoss(_pBoss);
		_pUiBardSkill->SetBoss(_pBoss);

		break;
	case Client::Stage::LoadingLevel:
		//do nothing
		break;
	case Client::Stage::Dialouge:
		//_pBGMManager->PauseBGM();
		//do nothing
		break;
	case Client::Stage::Stage4_2:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage4_2);
		_pBossManager->SetNoteSpawnData("Assets/BossPattenCsv/Stage4/phase1.csv");
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.05f * Mathf::SecToNano });
		_pUiWarriorSkill->SetBoss(_pBoss);
		_pUiThiefSkill->SetBoss(_pBoss);
		_pUiSorcererSkill->SetBoss(_pBoss);
		_pUiBardSkill->SetBoss(_pBoss);

		break;
	case Client::Stage::Title:
	default:
		break;
	}
}

void Client::GameManager::Tick(_duration deltaSeconds)
{
	//DebugPrintf("TimeScale : %f\n", Time->GetTimeScale());

	Super::Tick(deltaSeconds);
	ChangeEditorLevel();
	DebugChangeStage();
	UpdateHPDefeatConditions();

	if (_pInputComponent->IsKeyPress(DIK_B))
	{
		for (auto& character : _characterContainer)
		{
			character->MP = character->MaxMP;
		}
	}

	if ((_pInputComponent->IsKeyPress(DIK_ESCAPE) || _pInputComponent->IsKeyPress(DIP_MENU)) &&
		currentStage != Stage::Title && 
		currentStage != Stage::LoadingLevel &&
		currentStage != Stage::Setting &&
		currentStage != Stage::Editor &&
		currentStage != Stage::Credit)
	{
		_gameStatus++;
	}

	if (_gameStatus == GameStatus::Pause)
	{
		PauseGame();
	}
	else if(_gameStatus == GameStatus::Resume && _isPaused)
	{
		ResumeGame();
		DebugPrintf("Resume\n");
	}

	if (8.65E+10 <= Sound->GetPosition(CHANNEL_BGM) && currentStage == Stage::Dialouge)
	{
		ChangeStage(Stage::Stage4_2);
	}
	else if (7.80E+10 <= Sound->GetPosition(CHANNEL_BGM) && currentStage == Stage::Stage4)
	{
 		ChangeStage(Stage::Dialouge);
	}
}

void Client::GameManager::Fixed()
{
}

void Client::GameManager::EndPlay()
{
}

void Client::GameManager::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeys(DIK_LCONTROL, DIK_ESCAPE, DIK_B, DIK_TAB, DIK_PGUP, DIK_PGDN, DIP_MENU, DIP_LX, DIP_LY);
	inputComponent->AttachInputManager();
}

void Client::GameManager::UpdateCharacterHPBarBlink(int willBeDecreaseHP)
{
	for (int index : _hpBarBlinkTargets)
	{
		if (index >= 0 && index < _characterContainer.size())
		{
			_characterContainer[index]->SetHPIndexBlink(willBeDecreaseHP);
		}
	}
}

void Client::GameManager::CancelCharacterHPBarBlink()
{
	for (auto& character : _characterContainer)
	{
		character->CancelHPIndexBlink();
	}
}

void Client::GameManager::SetBindKeySetting(GameKeySettingType setting)
{
	_currentKeySetting = setting;

	if (_characterContainer.empty())
	{
		return;
	}

	switch (setting)
	{
	case Client::GameKeySettingType::A:
		_characterContainer[0]->SettingKey(DIK_R);//워리어, 도적, 소서러, 바드
		_characterContainer[1]->SettingKey(DIK_E);
		_characterContainer[2]->SettingKey(DIK_W);
		_characterContainer[3]->SettingKey(DIK_Q);
		if (_pNoteManager)
		{
			_pNoteManager->ChangeKeySetting(DIK_RETURN);
		}
		break;
	case Client::GameKeySettingType::B:
		_characterContainer[0]->SettingKey(DIK_RIGHT);
		_characterContainer[1]->SettingKey(DIK_LEFT);
		_characterContainer[2]->SettingKey(DIK_UP);
		_characterContainer[3]->SettingKey(DIK_DOWN);
		if (_pNoteManager)
		{
			_pNoteManager->ChangeKeySetting(DIK_SPACE);
		}
		break;
	case Client::GameKeySettingType::GamePad:
		_characterContainer[0]->SettingKey(DIP_DPAD_RIGHT);
		_characterContainer[1]->SettingKey(DIP_DPAD_LEFT);
		_characterContainer[2]->SettingKey(DIP_DPAD_UP);
		_characterContainer[3]->SettingKey(DIP_DPAD_DOWN);
		if (_pNoteManager)
		{
			_pNoteManager->ChangeKeySetting(DIP_B);
		}
		break;
	default:
		break;
	}

}

void Client::GameManager::UpdateStageClearConditions()
{
	if(!_pBoss)
	{
		return;
	}

	nlohmann::json json;
	std::ifstream inFile("Assets/StageClear.json");
	if (inFile.is_open())
	{
		inFile >> json;
		inFile.close();
	}

	// 클리어 조건 업데이트
	if (_pBoss->IsDead && !_isDefeat)
	{
		switch (currentStage)
		{
		case Client::Stage::Stage1:
			json["Stage1"] = true;
			break;
		case Client::Stage::Stage2:
			json["Stage2"] = true;
			break;
		case Client::Stage::Stage3:
			json["Stage3"] = true;
			break;
		case Client::Stage::Stage4:
			json["Stage4"] = true;
			break;
		default:
			break;
		}

		// 파일 쓰기
		std::ofstream outFile("Assets/StageClear.json");
		if (!outFile.is_open())
		{
			std::cerr << "Failed to open StageClear.json for writing!" << std::endl;
			return;
		}

		outFile << json.dump(4);  // pretty print with 4-space indentation
		outFile.close();

		ChainLoadingScreen();
	}
}

void Client::GameManager::UpdateHPDefeatConditions()
{
	if (_isDefeat)
	{
		return;
	}

	for (auto character : _characterContainer)
	{
		if (character->IsDead)
		{
			_isDefeat = true;
			DebugPrintf("Defeat\n");
			_pTimerComponent->SetTime(5, 2.0f);
			break;
		}
	}
}

void Client::GameManager::HPDefeat()
{
	GameOverLevel* gameOver = dynamic_cast<GameOverLevel*>(_pOwnerWorld->GetLevel((int)Stage::GameOver));
	if (nullptr != gameOver)
	{
		gameOver->SetStage(currentStage);
	}
	ChangeStage(Stage::GameOver);
}

void Client::GameManager::UpdateDefeatConditions()
{
	if (_isDefeat)
	{
		return;
	}

	_ldouble currentPosition = Sound->GetPosition(CHANNEL_BGM);
	_ldouble endPosition = Sound->GetLength(CHANNEL_BGM);

	if (!_pBoss->IsDead)
	{
		_isDefeat = true;
		DebugPrintf("Defeat\n");
		GameOverLevel* gameOver = dynamic_cast<GameOverLevel*>(_pOwnerWorld->GetLevel((int)Stage::GameOver));
		if (nullptr != gameOver)
		{
			gameOver->SetStage(currentStage);
		}
		ChangeStage(Stage::GameOver);
		return;
	}

}

void Client::GameManager::SpawnBoss(Stage stage)
{
	if(!_pBackGround)
	{
		_pBackGround = static_cast<BackGround*>(GetWorld()->FindActor("BackGround"));
	}

	switch (stage)
	{
	case Client::Stage::Editor: //에디터 스테이지 초기값
		_pBoss = _pBossManager->SpawnBoss(Stage::Editor);
		_pBGMManager->StopBGM();
		break;
	case Client::Stage::Stage1:
	case Client::Stage::Stage2:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage2);
		_pBackGround->SetStageBackground(Stage::Stage2);
		_pBGMManager->CurrentStage = 0;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->PlayBGM();
		_pBGMManager->PauseBGM();
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.01f * Mathf::SecToNano });
		break;
	case Client::Stage::Stage3:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage3);
		_pBackGround->SetStageBackground(Stage::Stage3);
		_pBGMManager->CurrentStage = 1;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->PlayBGM();
		_pBGMManager->PauseBGM();
		_pNoteManager->SetSmashingPosition({ Sound->GetLength(CHANNEL_BGM) - (long double)0.01f * Mathf::SecToNano });
		break;
	case Client::Stage::Stage4:
		_pBoss = _pBossManager->SpawnBoss(Stage::Stage4);
		_pBackGround->SetStageBackground(Stage::Stage4);
		_pBGMManager->CurrentStage = 2;
		_pBGMManager->CurrentPhase = 0;
		_pBGMManager->PlayBGM();
		_pBGMManager->PauseBGM();
		_pNoteManager->SetSmashingPosition({ (Sound->GetLength(CHANNEL_BGM) - (long double)0.05f * Mathf::SecToNano) });
		break;
	case Client::Stage::LoadingLevel:
	case Client::Stage::Title:
	default:
		break;
	}
}

void Client::GameManager::SetFullHP()
{
	for (auto& character : _characterContainer)
	{
		character->HP = character->MaxHP;
	}
}

void Client::GameManager::ChainLoadingScreen()
{
	if(_pBoss)
	{
		_pBGMManager->StopBGM();
		_pBoss->OnCompleteDestroyMark();
		Management->ScheduleDestroyPoint();
	}

	if(_pBossManager)
	{
		_pBossManager->OffBossHPBar();
	}
	GetWorld()->GetCameraActor()->FadeOut(0.3f);
	_pTimerComponent->SetTime(0, 2.3f);
	_pTimerComponent->SetTime(1, 2.0f);
	_pTimerComponent->SetTime(2, 0.3f);
}

void Client::GameManager::LoadingScreenEndAndFadeIn()
{
	GetWorld()->GetCameraActor()->FadeOutIn(1.3f);
}

void Client::GameManager::LoadingScreenView()
{
	IsSceneChangeTrigger = true;
	GetWorld()->GetCameraActor()->FadeIn(0.3f);
	_pOwnerWorld->ChangeLevel((int)Stage::LoadingLevel);
	IsSceneChangeTrigger = false;
}

void Client::GameManager::ChangeNextStage()
{
	IsSceneChangeTrigger = true;
	_gameStatus = GameStatus::Playing;
	_isPaused = false;

	if(currentStage == Stage::Stage4)
	{
		currentStage = Stage::Title;
	}
	else if(currentStage == Stage::Title)
	{
		currentStage = Stage::Stage2;
	}
	else
	{
		currentStage++;
	}

	//++ 연산 이후 credit이면 엔딩 재생
	if (currentStage == Stage::Credit)
	{
		CraditLevel* level = dynamic_cast<CraditLevel*>(_pOwnerWorld->GetLevel((int)currentStage));
		if(level)
		{
			level->SetPlayEnding(true);
		}
	}

	_pOwnerWorld->ChangeLevel((int)currentStage);
	if(currentStage != Stage::Title)
	{
		_pBossManager->OnBossHPBar();
	}
	IsSceneChangeTrigger = false;
}

void Client::GameManager::ChangeStage(Stage stage)
{
	if (stage == Stage::Title)
	{
		if(_pBGMManager)
		{
			_pBGMManager->StopBGM();
		}
	}
	_gameStatus = GameStatus::Playing;
	_isPaused = false;

	if (0 == Time->GetTimeScale())
	{
		Time->SetTimeScale(1.f);
	}

	GetWorld()->GetCameraActor()->FadeOutIn(1.3f);
	currentStage = stage;

	if(_pBoss)
	{
		_pBoss->OnCompleteDestroyMark();
	}
	IsSceneChangeTrigger = true;
	if(_pBossManager)
	{
		_pBossManager->OffBossHPBar();
	}
	
	_pOwnerWorld->ChangeLevel((int)stage);
	IsSceneChangeTrigger = false;
}

void Client::GameManager::RemoveAllCharacters()
{
	for (auto character : _characterContainer)
	{
		_pOwnerWorld->RemoveActor(character->GetName());
	}
}

void Client::GameManager::ChangeEditorLevel()
{
	bool SysKey = _pInputComponent->IsKeyHold(DIK_LCONTROL);

	if (SysKey && _pInputComponent->IsKeyPress(DIK_TAB) && !IsSceneChangeTrigger)
	{
		_pOwnerWorld->GetCameraActor()->FadeOut(1.0f);
		IsSceneChangeTrigger = true;

		switch (currentStage)
		{
		case Client::Stage::Title:
			currentStage = Stage::Editor;
			break;
		case Client::Stage::Editor:
			currentStage = Stage::Title;
			break;
		default:
			break;
		}
	}
	else if (IsSceneChangeTrigger)
	{
		if (!_pOwnerWorld->GetCameraActor()->IsFadeOut())
		{
			if (currentStage == Stage::Editor)
			{
				if(_pBossManager)
				{
					_pBossManager->OnBossHPBar();
				}
				ChangeStage(Stage::Editor);
				_pOwnerWorld->GetCameraActor()->FadeIn(1.0f);
				IsSceneChangeTrigger = false;
			}
			else if (currentStage == Stage::Title)
			{
				if (_pBossManager)
				{
					_pBossManager->OffBossHPBar();
				}
				ChangeStage(Stage::Title);
				_pOwnerWorld->GetCameraActor()->FadeIn(1.0f);
				IsSceneChangeTrigger = false;
			}
		}
	}
}

void Client::GameManager::DebugChangeStage()
{
	bool SysKey = _pInputComponent->IsKeyHold(DIK_LCONTROL);

	if (SysKey && _pInputComponent->IsKeyPress(DIK_PGUP) && !IsSceneChangeTrigger)
	{
		if (currentStage == Stage::Stage4)
		{
			ChangeStage(Stage::Dialouge);
		}
		else
		{
			ChainLoadingScreen();
		}
	}
}

bool Client::GameManager::staticInputPress(_uint key)
{
	return _pInputComponent->IsKeyPress(key);
}

void Client::GameManager::Destroy()
{
	Super::Destroy();
}

void Client::GameManager::AddAllCharacterMaxHP(int value)
{
	for (auto character : _characterContainer)
	{
		character->MaxHP += value;
	}
}

std::pair<float, int> Client::GameManager::GetCurrSmashingData()
{
	int stage = -1;
	switch (currentStage)
	{
	case Client::Stage::Editor:
		stage = _pBGMManager->CurrentStage + 1;
		break;
	case Client::Stage::Stage1:
		stage = 1;
		break;
	case Client::Stage::Stage2:
		stage = 1;
		break;
	case Client::Stage::Stage3:
		stage = 2;
		break;
	case Client::Stage::Stage4:
	case Client::Stage::Stage4_2:
		stage = 3;
		break;
	default:
		break;
	}
	std::string section = std::string("Stage") + std::to_string(stage);
	std::pair smashingData = std::make_pair(std::stof(_smashingInI.GetValue(section, "LimitTime")), std::stoi(_smashingInI.GetValue(section, "TargetCount")));
	return smashingData;
}
