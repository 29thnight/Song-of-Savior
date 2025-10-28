#include <NoteManager.h>
#include <SceneComponent.h>
#include <SoundManager.h>
#include <InputComponent.h>
#include <TextRenderComponent.h>
#include <TimeManager.h>
#include <CsvLoader.h>
#include <SimpleIniLoader.h>
#include <BitmapComponent.h>
#include <CameraActor.h>
#include <CameraComponent.h>
#include <UtfConvert.h>
#include <World.h>
#include <Mathf.h>
#include <CoreManager.h>
#include <TimerComponent.h>

#include "Note.h"
#include "GameManager.h"
#include "Boss.h"
#include "BossHP.h"

using namespace Engine;
using namespace Mathf;

void Client::NoteManager::BeginPlay()
{
	Base::BeginPlay();

	_pNoteRateLineBitmap = AddComponent<BitmapComponent>("BitmapComponent");
	_pNoteRateLineBitmap->SetTextures(&_vecTextures);
	_pNoteRateLineBitmap->SetOrder(10);
	_pNoteRateLineBitmap->SetBitmapIndex(0);

	_pNoteDoubleLineBitmap = AddComponent<BitmapComponent>("BitmapComponent2");
	_pNoteDoubleLineBitmap->SetTextures(&_vecTextures);
	_pNoteDoubleLineBitmap->SetBitmapIndex(1);
	_pNoteDoubleLineBitmap->SetOrder(11);
	_pNoteDoubleLineBitmap->SetOpacity(0.f);

	InitializeNoteSpawnData();
	InitializeKeySetting();

	_pInputComponent = AddComponent<InputComponent>("InputComponent");

	_pTextRenderComponent = AddComponent<TextRenderComponent>("TextRenderComponent");
	_pTextRenderComponent->SetFont(L"Arial");
	_pTextRenderComponent->SetSize(Mathf::Vector2(100.f, 100.f));

	Mathf::Vector2 Position = _pRootComponent->GetRelativeLocation();
	for (int i = 0; i < _noteMaxCount; ++i)
	{
		std::string noteName = "note" + std::to_string(i);
		_pOwnerWorld->SpawnActor(LAYER::OBJECT,noteName.c_str(),Note::Create(),Position);
		Note* note = dynamic_cast<Note*>(_pOwnerWorld->FindActor(noteName.c_str()));
		_noteObjectPool.push_back(note);
	}

	AddSmashSuccessEvent(this, &NoteManager::SmashingFade);
	AddSmashFailEvent(this, &NoteManager::SmashingFade);

	_pTimerComponent = AddComponent<TimerComponent>("TimerComponent");
	_pTimerComponent->AddTimer(2);
	_pTimerComponent->GetTimer(0).onTimeOver.AddDynamic(Manager, &GameManager::UpdateStageClearConditions);
	_pTimerComponent->GetTimer(1).onTimeOver.AddDynamic(Manager, &GameManager::UpdateDefeatConditions);
}

void Client::NoteManager::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);

	if (_isSmashingMode)
	{
		ChangeSmashingMode();
		Smashing(); // Smashing Mode tick
	}
	else if (_isNormalMode)
	{
		ChangeNormalMode();
	}

	currentStageNoteDataLoad();

	if (_spawnCount < _noteSpawnData.size())
	{
		_ldouble StartPosition = _noteSpawnData[_spawnCount]._noteTime - _noteSpawnOffset;
		_ldouble HitPosition = _noteSpawnData[_spawnCount]._noteTime;
		if (StartPosition <= Sound->GetPosition(CHANNEL_BGM) && 0 != Sound->GetPosition(CHANNEL_BGM))
		{
			_noteObjectPool[_noteIndex]->SetVisible(true);
			_noteObjectPool[_noteIndex]->_ldStartPosition = StartPosition;
			_noteObjectPool[_noteIndex]->_ldTargetPosition = HitPosition;
			_noteObjectPool[_noteIndex]->_ldMissOffset = _noteSpawnOffset;
			_noteObjectPool[_noteIndex]->Tick(_duration(0));
			_noteSpawnData[_spawnCount]._noteIndex = _noteIndex;
			_noteSpawnQueue.push_back(_noteSpawnData[_spawnCount]);

			_noteIndex++;
			_spawnCount++;

			if (_noteIndex >= _noteMaxCount)
			{
				_noteIndex = 0;
			}
		}
	}
	else if(_spawnCount == _noteSpawnData.size())
	{
		long double currPosition = Sound->GetPosition(CHANNEL_BGM);
		if (_SmashingPositionIndex < _SmashingPosition.size())
		{
			//보스가 죽었을때만 나와야함. 에디터는?..?
			long double smashingPosition = _SmashingPosition[_SmashingPositionIndex];
			if (!_isSmashingMode &&
				Manager->GetCurrentBoss()->HP <= 0 &&
				currPosition >= smashingPosition
				)
			{
				dynamic_cast<BossHP*>(GetWorld()->FindActor("BossHP"))->UpdateHpScale();
				std::pair currSmashingData(Manager->GetCurrSmashingData());
				SetSmashingMode(currSmashingData.first, currSmashingData.second);
				_SmashingPositionIndex++;
			}
		}
	}

	if (_pInputComponent->IsKeyPress(_keyboardHit)	&& !_noteSpawnQueue.empty() && Time->GetTimeScale() != 0.f)
	{
		_ldouble frontHitEnd = _noteSpawnQueue.front()._noteTime;
		constexpr _ldouble calibrateFactor = 19604167;

		if (Sound->GetPosition(CHANNEL_BGM) - calibrateFactor < frontHitEnd - _noteGoodOffset)
			return;

		DebugPrintf("frontHitEnd : %lf\n", frontHitEnd);
		DebugPrintf("key press time : %lf\n", Sound->GetPosition(CHANNEL_BGM));

		Perfect(frontHitEnd);
		Great(frontHitEnd);
		Good(frontHitEnd);
	}

	if(_pInputComponent->IsKeyRelease(_keyboardHit)	&& !_noteSpawnQueue.empty())
	{
		_isPerfect = false;
		_isGreat = false;
		_isGood = false;
		_isMiss = false;
	}

	if (!_noteSpawnQueue.empty())
	{
		_ldouble frontHitEnd = _noteSpawnQueue.front()._noteTime + _noteGoodOffset;
		if (frontHitEnd < Sound->GetPosition(CHANNEL_BGM))
		{
			_isMiss = true;
			DebugPrintf("Miss\n");
			_noteSpawnQueue.pop_front();
			_pNoteDoubleLineBitmap->SetBitmapIndex(3);
			_pNoteDoubleLineBitmap->StartConceal(0.5f);
		}
	}

	if(_noteSpawnQueue.empty())
	{
		_ldouble currentPosition = Sound->GetPosition(CHANNEL_BGM);
		_ldouble endPosition = Sound->GetLength(CHANNEL_BGM);

		_isPerfect = false;
		_isGreat = false;
		_isGood = false;
		_isMiss = false;

		if (0 == endPosition && !_isEnd)
		{
			_pTimerComponent->SetTime(1, 2.f);
			for (auto& note : _noteObjectPool)
			{
				note->SetVisible(false);
			}

			_noteSpawnData.clear();
			_isEnd = true;
		}
	}

	_pInputComponent->InvokeVibration(deltaSeconds);

}

void Client::NoteManager::Fixed()
{
}

void Client::NoteManager::EndPlay()
{
	for (auto& note : _noteObjectPool)
	{
		note->OnCompleteDestroyMark();
	}
}

void Client::NoteManager::InitializeKeySetting() //기획 변경으로 인한 불필요 함수
{
}

void Client::NoteManager::InitializeNoteSpawnData()
{
	std::string absolutePath = "Assets/NoteSpawnData.ini";
	SimpleIniFile iniFile;
	try
	{
		iniFile.Load(absolutePath.c_str());
	}
	catch (const std::runtime_error& e)
	{
        std::cerr << "Error loading INI file: " << e.what() << std::endl;
        return;
	}

	_noteSpawnOffset = static_cast<_ldouble>(std::stof(
		iniFile.GetValue("NoteSpawnData", "NoteSpawnOffset", "0.0"))) * 1e9;

	_notePerfectOffset = static_cast<_ldouble>(std::stof(
		iniFile.GetValue("NoteSpawnData", "NotePerfectOffset", "0.0"))) * 1e9;

	_noteGreatOffset = static_cast<_ldouble>(std::stof(
			iniFile.GetValue("NoteSpawnData", "NoteGreatOffset", "0.0"))) * 1e9;

	_noteGoodOffset = static_cast<_ldouble>(
		std::stof(iniFile.GetValue("NoteSpawnData", "NoteGoodOffset", "0.0"))) * 1e9;

	_chargePerfectMana = static_cast<int>(
		std::stoi(iniFile.GetValue("NoteChargeData", "ChargePerfectMana", "2")));

	_chargeGreatMana = static_cast<int>(
		std::stoi(iniFile.GetValue("NoteChargeData", "ChargeGreatMana", "1")));

	_chargeGoodMana = static_cast<int>(
		std::stoi(iniFile.GetValue("NoteChargeData", "ChargeGoodMana", "1")));

}


long double Client::NoteManager::GetCurrNotePosition()
{
	if (!_noteSpawnData.empty())
	{
		if (_spawnCount < _noteSpawnData.size())
		{
			return _noteSpawnData[_spawnCount]._noteTime;
		}
	}

	return 0;
}

void Client::NoteManager::SetSmashingMode(float LimitTime, int TargetCount)
{
	Sound->SetPause(CHANNEL_BGM, true);
	_isSmashingMode = true;
	_isNormalMode = false;
	_elapsedPosition = 0;
	_startPosition = GetRootComponent()->GetRelativeLocation();
	_smashingLimitTime = LimitTime;
	_SmashingDemention = TargetCount;
}

void Client::NoteManager::SetNormalMode()
{
	_isSmashingMode = false;
	_isNormalMode = true;
	_elapsedPosition = 0;
	_startPosition = GetRootComponent()->GetRelativeLocation();
}

void Client::NoteManager::ChangeSmashingMode()
{
	constexpr long double deltaTimeOffset = 1.5f * Mathf::SecToNano;
	constexpr float startScale = 1.0f;
	constexpr float targetScale = 3.2f;
	Mathf::Vector2 targetPosition{ Management->setting.width * 0.5f, Management->setting.height * 0.5f };
	if (targetScale == GetRootComponent()->GetRelativeScale().x)
	{
		return;
	}

	if (_elapsedPosition <= deltaTimeOffset)
	{
		long double deltaTime = static_cast<long double>(Time->GetNonScaleDeltaSeconds().count());
		_elapsedPosition += deltaTime;
		Mathf::Vector2 lerpPosition = Mathf::Lerp(_startPosition, targetPosition, _elapsedPosition / deltaTimeOffset);
		float lerpScale = Mathf::Lerp(startScale, targetScale, _elapsedPosition / deltaTimeOffset);

		GetRootComponent()->SetRelativeScale({ lerpScale, lerpScale });
		GetRootComponent()->SetRelativeLocation(lerpPosition);

		if(_elapsedPosition > deltaTimeOffset)
		{
			GetRootComponent()->SetRelativeScale({ targetScale, targetScale });
			GetRootComponent()->SetRelativeLocation(targetPosition);
			_isSmashingMode = true;
			_isNormalMode = false;
			_isSmashing = true;
		}
	}
}

void Client::NoteManager::ChangeNormalMode()
{
	constexpr long double deltaTimeOffset = 1.5f * Mathf::SecToNano;
	constexpr float startScale = 3.2f;
	constexpr float targetScale = 1.0f;
	Mathf::Vector2 targetPosition{ 300.f, 300.f };

	GetRootComponent()->SetRelativeScale({ targetScale, targetScale });
	GetRootComponent()->SetRelativeLocation(targetPosition);
	_isSmashingMode = false;
	_isNormalMode = false;
	_isSmashing = false;
}

void Client::NoteManager::Smashing()
{
	if (_isSmashing)
	{
		if (_pInputComponent->IsKeyPress(_keyboardHit))
		{
			_isPerfect = true;
			_SmashingCount++;
			DebugPrintf("SmashingCount : %d\n", _SmashingCount);
			int randomIndex = _random.Generate();
			int randomSmashingSound = _randomSmashingSound.Generate();
			_pNoteDoubleLineBitmap->SetBitmapIndex(randomIndex);
			if (randomSmashingSound == 0)
			{
				Sound->PlaySound("Smashing_01", CHANNEL_UI, NOT_LOOP);
			}
			else if (randomSmashingSound == 1)
			{
				Sound->PlaySound("Smashing_02", CHANNEL_UI, NOT_LOOP);
			}
			else if (randomSmashingSound == 2)
			{
				Sound->PlaySound("Smashing_03", CHANNEL_UI, NOT_LOOP);
			}
			else if (randomSmashingSound == 3)
			{
				Sound->PlaySound("Smashing_04", CHANNEL_UI, NOT_LOOP);
			}
			_pNoteDoubleLineBitmap->StartConceal(0.5f);
			_pInputComponent->SetVibration(0.f, 1.f, 0.5f);
		}

		_smashingElapsedTime += Time->DurationToFloat(Time->GetDeltaSeconds());
		if (_SmashingCount >= _SmashingDemention)
		{
			_isSmashingMode = false;
			SetNormalMode();
			_SmashingCount = 0;
			DebugPrintf("연타 성공! \n");
			_smashingElapsedTime = 0.f;
			_smashSuccessEvent.Broadcast();
		}
		else if (_smashingLimitTime <= _smashingElapsedTime)
		{
			_isSmashingMode = false;
			SetNormalMode();
			_SmashingCount = 0;
			DebugPrintf("연타 실패! \n");
			_smashingElapsedTime = 0.f;
			_smashFailureEvent.Broadcast();
		}
	}
}

void Client::NoteManager::Perfect(_ldouble hitPoint)
{
	_ldouble frontHitStart = hitPoint - _notePerfectOffset;
	_ldouble frontHitEnd = hitPoint + _notePerfectOffset;
	if(frontHitStart < Sound->GetPosition(CHANNEL_BGM)
   		&& frontHitEnd >= Sound->GetPosition(CHANNEL_BGM))
	{
		_noteObjectPool[_noteSpawnQueue.front()._noteIndex]->SetVisible(false);
		_noteSpawnQueue.pop_front();
		_isPerfect = true;
		Manager->ChargeMana(CharacterType::All, _chargePerfectMana);
		DebugPrintf("Perfect\n");
		_pNoteDoubleLineBitmap->SetBitmapIndex(1);
		_pNoteDoubleLineBitmap->StartConceal(0.5f);
		_pInputComponent->SetVibration(0.f, 1.f, 0.5f);
	}
}

void Client::NoteManager::Great(_ldouble hitPoint)
{
	_ldouble frontHitStart = hitPoint - _noteGreatOffset;
	_ldouble frontHitEnd = hitPoint + _noteGreatOffset;
	if(frontHitStart < Sound->GetPosition(CHANNEL_BGM)
		&& frontHitEnd >= Sound->GetPosition(CHANNEL_BGM)
		&& !_isPerfect)
	{
		_noteObjectPool[_noteSpawnQueue.front()._noteIndex]->SetVisible(false);
		_noteSpawnQueue.pop_front();
		Manager->ChargeMana(CharacterType::All, _chargeGreatMana);
		_isGreat = true;
		DebugPrintf("Great\n");
		_pNoteDoubleLineBitmap->SetBitmapIndex(2);
		_pNoteDoubleLineBitmap->StartConceal(0.5f);
	}
}

void Client::NoteManager::Good(_ldouble hitPoint)
{
	_ldouble frontHitStart = hitPoint - _noteGoodOffset;
	_ldouble frontHitEnd = hitPoint + _noteGoodOffset;
	if(frontHitStart < Sound->GetPosition(CHANNEL_BGM)
		&& frontHitEnd >= Sound->GetPosition(CHANNEL_BGM)
		&& !_isPerfect && !_isGreat)
	{
		_noteObjectPool[_noteSpawnQueue.front()._noteIndex]->SetVisible(false);
		_noteSpawnQueue.pop_front();
		Manager->ChargeMana(CharacterType::All, _chargeGoodMana);
		_isGood = true;
		DebugPrintf("Good\n");
		_pNoteDoubleLineBitmap->SetBitmapIndex(2);
		_pNoteDoubleLineBitmap->StartConceal(0.5f);
	}
}

void Client::NoteManager::ChangeKeySetting(_uint hitKey)
{
	_keyboardHit = hitKey;
}

void Client::NoteManager::currentStageNoteDataLoad()
{
	_ldouble currentPosition = Sound->GetPosition(CHANNEL_BGM);
	_ldouble endPosition = Sound->GetLength(CHANNEL_BGM);

	if (0 == endPosition || _isEnd)
	{
		return;
	}

	if (0 == _noteSpawnData.size() || _noteSpawnData.empty())
	{
		Stage currentStage = Manager->GetCurrentStage();
		switch (currentStage)
		{
		case Client::Stage::Stage2:
			SetNoteSpawnData("Assets/NoteCsv/Stage2/phase1.csv");
			break;
		case Client::Stage::Stage3:
			SetNoteSpawnData("Assets/NoteCsv/Stage3/phase1.csv");
			break;
		case Client::Stage::Stage4:
			SetNoteSpawnData("Assets/NoteCsv/Stage4/phase1.csv");
			break;
		case Client::Stage::Stage4_2:
			SetNoteSpawnData("Assets/NoteCsv/Stage4/phase2.csv");
			break;
		default:
			break;
		}
	}
}

void Client::NoteManager::DongDong(ENoteRate rating)
{
}

void Client::NoteManager::SetNoteSpawnData(const std::string& fileName)
{
	_noteSpawnData.clear();

	CSVReader<int, long double> reader(fileName);
	reader.forEach([&](int spawnIndex, long double noteTime)
		{
			NoteSpawnTable table;
			table._spawnIndex = spawnIndex;
			table._noteTime = noteTime;
			_noteSpawnData.push_back(table);
		});
}

void Client::NoteManager::SetSmashingPosition(std::vector<long double> smashingPosition)
{
	_SmashingPosition = smashingPosition;
	_SmashingPositionIndex = 0;
}

void Client::NoteManager::SmashingFade()
{
	GetWorld()->GetCameraActor()->FadeOutIn(1.0f, Color::White);
	Manager->GetCurrentBoss()->Dead();
	_pTimerComponent->SetTime(0, 3.f);
}


void Client::NoteManager::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeys(DIK_SPACE, DIK_RETURN, DIP_B, DIP_A);
	inputComponent->AttachInputManager();
}

void Client::NoteManager::Destroy()
{
	Base::Destroy();
	for (auto& note : _noteObjectPool)
	{
		note->IsCompleteDestroyMarked();
	}
}
