#include <CsvLoader.h>
#include <SoundManager.h>
#include <AnimationComponent.h>
#include "Level.h"
#include "World.h"
#include "Boss.h"
#include "Dragon.h"
#include "Wolf.h"
#include "Lich.h"
#include "BossManager.h"
#include "BossHP.h"

#include <CoreManager.h>

void Client::BossManager::BeginPlay()
{
	Base::BeginPlay();
}

void Client::BossManager::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);
	long double BGMPosition = Sound->GetPosition(CHANNEL_BGM);
	if(_pBoss->IndicateCount > 0 && -1 != _pBoss->CharginCancel)
	{
		if (_pBossHP)
		{
			_pBossHP->SetVisibleIndicator(true);
			_pBossHP->SetIndicatorText(_pBoss->IndicateCount);
		}
	}
	else
	{
		_pBossHP->SetVisibleIndicator(false);
	}

	if (_noteIndexAttack < _noteSpawnData.size())
	{
		_ldouble PattenPosition = _noteSpawnData[_noteIndexAttack]._noteTime - _pBoss->_ldAttackOffset;
		if (PattenPosition <= BGMPosition)
		{
			if (!_pBoss->IsDead)
			{
				if (_noteSpawnData[_noteIndexAttack]._attack)
				{
					_pBoss->StartPattern(1);
					if (!_noteSpawnData[_noteIndexAttack]._skill1)
					{
						_noteIndexSkill1 = _noteIndexAttack + 1;
					}
					if (!_noteSpawnData[_noteIndexAttack]._skill2)
					{
						_noteIndexSkill2 = _noteIndexAttack + 1;
					}
				}
			}
			_noteIndexAttack++;
		}		
	}
	if (_noteIndexSkill1 < _noteSpawnData.size())
	{
		_ldouble PattenPosition = _noteSpawnData[_noteIndexSkill1]._noteTime - _pBoss->_ldSkillOffset - _pBoss->_ldSkillCharging;
		if (PattenPosition <= BGMPosition) 
		{
			if (!_pBoss->IsDead)
			{
				if (_noteSpawnData[_noteIndexSkill1]._skill1)
				{
					_pBoss->StartPattern(2);
					if (!_noteSpawnData[_noteIndexSkill1]._attack)
					{
						_noteIndexAttack = _noteIndexSkill1 + 1;
					}
					if (!_noteSpawnData[_noteIndexSkill1]._skill2)
					{
						_noteIndexSkill2 = _noteIndexSkill1 + 1;
					}
				}		
			}
			_noteIndexSkill1++;		
		}
	}
	if (_noteIndexSkill2 < _noteSpawnData.size())
	{
		_ldouble PattenPosition = _noteSpawnData[_noteIndexSkill2]._noteTime - _pBoss->_ldBossSkillOffset - _pBoss->_ldBossSkillCharging;
		if (PattenPosition <= BGMPosition)
		{
			if (!_pBoss->IsDead)
			{
				if (_noteSpawnData[_noteIndexSkill2]._skill2)
				{
					_pBoss->StartPattern(3);
					if (!_noteSpawnData[_noteIndexSkill2]._attack)
					{
						_noteIndexAttack = _noteIndexSkill2 + 1;
					}
					if (!_noteSpawnData[_noteIndexSkill2]._skill1)
					{
						_noteIndexSkill1 = _noteIndexSkill2 + 1;
					}
				}				
			}
			_noteIndexSkill2++;
		}
	}
}

void Client::BossManager::Fixed()
{
}

void Client::BossManager::EndPlay()
{
}

void Client::BossManager::ReviveInitialize()
{
	_noteIndexAttack = 0;
	_noteIndexSkill1 = 0;
	_noteIndexSkill2 = 0;
}


Client::Boss* Client::BossManager::SpawnBoss(Stage currentStage)
{
	_currentStage = currentStage;
	std::string bossName = "";

	if (_pBossHP)
	{
		_pBossHP->OnCompleteDestroyMark();
		Management->ScheduleDestroyPoint();
		_pBossHP = nullptr;
	}

	switch (_currentStage)
	{
		case Client::Stage::Editor: //에디터 스테이지 초기값
		case Client::Stage::Stage2:
			bossName = "Wolf";
			GetWorld()->SpawnActor(LAYER::OBJECT, bossName.c_str(), Wolf::Create(), Mathf::Vector2{ 1500.f, 540.f });
			break;
		case Client::Stage::Stage3:
			bossName = "Dragon";
			GetWorld()->SpawnActor(LAYER::OBJECT, bossName.c_str(), Dragon::Create(), Mathf::Vector2{ 1500.f, 540.f });
			break;
		case Client::Stage::Stage4:
		case Client::Stage::Stage4_2:
			bossName = "Lich";
			GetWorld()->SpawnActor(LAYER::OBJECT, bossName.c_str(), Lich::Create(), Mathf::Vector2{ 1500.f, 550.f });
			break;
		case Client::Stage::LoadingLevel:
		case Client::Stage::Title:
		default:
			break;
	}

	_pBoss = static_cast<Boss*>(GetWorld()->FindActor(bossName.c_str()));
	if (_currentStage == Client::Stage::Stage4_2)
	{
		Lich* lich = dynamic_cast<Lich*>(_pBoss);
		if(nullptr != lich)
		{
			lich->SetPhase2();
		}
	}

	if (_pBossHP == nullptr)
	{
		_pBossHP = BossHP::Create();
		_pBossHP->_pBossManager = this;
		GetWorld()->SpawnActor(LAYER::UI, "BossHP", _pBossHP);
		_pBossHP->SetHeadBitmap();
	}

	return _pBoss;
}

void Client::BossManager::SetNoteSpawnData(const std::string& fileName)
{
	_noteSpawnData.clear();

	CSVReader<int, long double, int, int, int> reader(fileName);
	reader.forEach([&](int noteIndex, long double noteTime, int _attack, int _skill1, int _skill2)
		{
			NoteTable table;
			table._noteIndex = noteIndex;
			table._noteTime = noteTime;
			table._attack = _attack;
			table._skill1 = _skill1;
			table._skill2 = _skill2;
			
			_noteSpawnData.push_back(table);
		}
	);
}

int Client::BossManager::GetCurrBossHp()
{
	if (_pBoss)
	{
		return _pBoss->HP;
	}
	return -1;
}

int Client::BossManager::GetCurrBossMaxHp()
{
	if (_pBoss)
	{
		return _pBoss->MaxHP;
	}
	return -1;
}

void Client::BossManager::OnBossHPBar()
{
	if (_pBossHP)
	{
		_pBossHP->_pBossHeadBitmap->Visible = true;
		_pBossHP->_pCurrHPBitmap->Visible = true;
		_pBossHP->_pFullHPBitmap->Visible = true;
	}
}

void Client::BossManager::OffBossHPBar()
{
	if (_pBossHP)
	{
		_pBossHP->_pBossHeadBitmap->Visible = false;
		_pBossHP->_pCurrHPBitmap->Visible = false;
		_pBossHP->_pFullHPBitmap->Visible = false;
	}
}

long double Client::BossManager::GetPattenOffset(int pattenType)
{
	return _pBoss->GetPattenOffset(pattenType);
}
