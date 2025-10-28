#include "NoteEditer.h"
#include <World.h>
#include <TextRenderComponent.h>
#include <InputComponent.h>
#include <cassert>
#include <WinUtility.h>
#include <SoundManager.h>
#include <CoreManager.h>
#include <UtfConvert.h>
#include <CsvLoader.h>
#include <BitmapComponent.h>
#include <TimeManager.h>
#include <optional>

#include "NoteManager.h"
#include "BGMManager.h"
#include "Note.h"
#include "TimelineManager.h"
#include "BossManager.h"
#include "GameManager.h"
#include "Lich.h"

void Client::NoteEditor::BeginPlay()
{
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
	 else
	 {		 
		 if (!_pBGMManager->_bgmList.empty())
		 {
			 _selectStage = 0;
			 if (!_pBGMManager->_bgmList[_selectStage].empty())
				 _selectPhase = 0;
		 }
	 }

	 GetWorld()->SpawnActor(LAYER::BACKGROUND, "TimelineManager", TimelineManager::Create());
	 _pTimelineManager = dynamic_cast<TimelineManager*>(GetWorld()->FindActor("TimelineManager"));

	 _pInputComponent = AddComponent<Engine::InputComponent>("InputComponent");

	 constexpr float editorDebugWidth = 1920;
	 constexpr float editorDebugHeight = 500;
	 _pEditorDebugText = AddComponent<Engine::TextRenderComponent>("EditorDebugText");
	 _pEditorDebugText->SetFont(L"MapoGoldenPier");
	 _pEditorDebugText->ChooseBrush("Default");
	 _pEditorDebugText->SetColor(Color::White);
	 _pEditorDebugText->SetSize(Mathf::Vector2(editorDebugWidth, editorDebugHeight));
	 _pEditorDebugText->SetRelativeLocation({ editorDebugWidth * 0.5f , editorDebugHeight * 0.5f });
	 _pEditorDebugText->AddRenderQueueInLayer();
	 _pEditorDebugText->SetFontSize(25.f);

	 constexpr float NoteListWidth = 200;
	 constexpr float NoteListHeight = 1080;
	 _pNoteListText = AddComponent<Engine::TextRenderComponent>("NoteListText");
	 _pNoteListText->SetFont(L"MapoGoldenPier");
	 _pNoteListText->ChooseBrush("Default");
	 _pNoteListText->SetColor(Color::White);
	 _pNoteListText->SetSize(Mathf::Vector2(NoteListWidth, NoteListHeight));
	 _pNoteListText->SetRelativeLocation({ Management->setting.width - NoteListWidth * 0.5f , NoteListHeight * 0.5f });
	 _pNoteListText->AddRenderQueueInLayer();
	
	 UpdateBGMManager();
	 LoadNoteCSV();
	 LoadBossPatternCSV();
}

void Client::NoteEditor::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (_respawnBoss)
	{
		Manager->SpawnBoss((Stage)(selectStage + 3));
		_respawnBoss = false;
	}

	const bool isCtrl = _pInputComponent->IsKeyHold(DIK_LCONTROL);
	const bool isShift = _pInputComponent->IsKeyHold(DIK_LSHIFT);
	const bool isAlt = _pInputComponent->IsKeyHold(DIK_LALT);

	if (_pInputComponent->IsKeyPress(DIK_F1))
	{
		SelectStage();
		_pTimelineManager->UpdateCurrNote();
		GetWorld()->TerminateActor(Manager->GetCurrentBoss()->GetName());
		_respawnBoss = true;
		return;
	}

	if (_pInputComponent->IsKeyPress(DIK_F2))
	{
		SelectPhase();
		_pTimelineManager->UpdateCurrNote();
	}

	if (_pInputComponent->IsKeyPress(DIK_B))
	{
		SelectBgm();
	}

	if (_pInputComponent->IsKeyPress(DIK_F3))
	{
		ResizeStage();
		_pTimelineManager->UpdateCurrNote();
	}

	if (_pInputComponent->IsKeyPress(DIK_F4))
	{
		ResizePhase();
		_pTimelineManager->UpdateCurrNote();
	}

	if (_pInputComponent->IsKeyPress(DIK_F5))
	{
		UpdateNoteManager();
		UpdateBossManager();
		UpdateNoteListText();
		LoadNoteCSV();
		LoadBossPatternCSV();
		_pTimelineManager->UpdateCurrNote();
	}

	if (_pInputComponent->IsKeyPress(DIK_F8))
	{
		SelectMode(EMODE(int(_selectMode) + 1));
	}

	if (_pInputComponent->IsKeyPress(DIK_M))
	{
		MoveBgmPosition();
	}

	if (_pInputComponent->IsKeyPress(DIK_RETURN))
	{
		_pBGMManager->PauseBGM();
		isPause = !isPause;
	}

	if (_pInputComponent->IsKeyPress(DIK_INSERT) || _pInputComponent->IsKeyPress(DIK_HOME))
	{
		InsertNote();
		_pTimelineManager->UpdateCurrNote();
	}

	if (_pInputComponent->IsKeyPress(DIK_DELETE))
	{
		DeleteNote();
		_pTimelineManager->UpdateCurrNote();
	}
	
	if (isCtrl && _pInputComponent->IsKeyPress(DIK_S))
	{
		//return;
		try
		{
			SaveNoteToCSV();
			SaveBossPatternToCSV();
			MessageBox(nullptr, L"저장 완료!", L"안내", MB_OK);
		}
		catch (const std::exception& ex)
		{
			std::wstring error = L"오류 : " + UtfConvert::utf8_to_wstring(ex.what());
			MessageBox(nullptr, error.c_str(), L"저장 오류!", MB_OK);
		}
	}

	if (isCtrl && _pInputComponent->IsKeyPress(DIK_F))
	{
		Manager->SetFullHP();
	}

	if (isCtrl)
	{	
		float mWheel = _pInputComponent->GetMouseWheel();
		if (mWheel > 0.f)
		{
			_pTimelineManager->TimelineBlockCount -= 1;
		}
		else if (mWheel < 0.f)
		{
			_pTimelineManager->TimelineBlockCount += 1;
		}
	}

	constexpr float frameTime = 0.016f;
	if (isPause)
	{
		if (_pInputComponent->IsKeyPress(DIK_RIGHTARROW))
		{
			Sound->SetPosition(CHANNEL_BGM, Sound->GetPosition(CHANNEL_BGM) + frameTime * static_cast<long double>(1e9));
			//DebugPrintf("%0.3lF\n", Sound->GetPosition(CHANNEL_BGM));
			UpdateNoteManager();
			UpdateBossManager();
		}
		else if (_pInputComponent->IsKeyPress(DIK_LEFTARROW))
		{
			Sound->SetPosition(CHANNEL_BGM, Sound->GetPosition(CHANNEL_BGM) - frameTime * static_cast<long double>(1e9));
			//DebugPrintf("%0.3lF\n", Sound->GetPosition(CHANNEL_BGM));
			UpdateNoteManager();
			UpdateBossManager();
		}
	}

	if (_selectMode == EMODE::BOSS)
	{
		if (_pInputComponent->IsKeyPress(DIK_1))
		{
			_selectBossAttackType = 1;
		}
		else if (_pInputComponent->IsKeyPress(DIK_2))
		{
			_selectBossAttackType = 2;
		}
		else if (_pInputComponent->IsKeyPress(DIK_3))
		{
			_selectBossAttackType = 3;
		}
	}

	UpdateDebugText();
	UpdateNoteListText();
}

void Client::NoteEditor::Fixed()
{

}

void Client::NoteEditor::EndPlay()
{

}

void Client::NoteEditor::SetupInputComponent(Engine::InputComponent* inputComponent)
{
	inputComponent->BindKeyAll();
	inputComponent->BindKeys(DIM_SCROLL);
									  
	inputComponent->AttachInputManager();
}

#ifdef _REALRELEASE

#endif // _REALRELEASE


void Client::NoteEditor::SelectStage()
{
	int result = WinUtility::GetIntFromUser(L"Stage select", L"편집할 스테이지를 입력하세요.");
	if (0 < result)
	{
		if (result <= _pBGMManager->_bgmList.size())
		{
			selectStage = result - 1;
			selectStage = std::clamp(selectStage, 0, 2);
			DebugPrintf("Select stage : %d\n", result);
			if (selectPhase >= _pBGMManager->_bgmList[selectStage].size())
			{
				selectPhase = (int)_pBGMManager->_bgmList[selectStage].size() - 1;
			}
			else if (_pBGMManager->_bgmList[selectStage].empty())
			{
				selectPhase = -1;
			}
		}
		else
		{
			MessageBox(nullptr, L"존재하지 않는 스테이지입니다.", L"안내", MB_OK);
		}
	}
}

void Client::NoteEditor::SelectPhase()
{
	int reslut = WinUtility::GetIntFromUser(L"Stage phase", L"편집할 페이즈를 입력하세요.");
	if (0 < reslut)
	{
		if (selectStage != -1 && reslut <= _pBGMManager->_bgmList[selectStage].size())
		{
			selectPhase = reslut - 1;
			DebugPrintf("Select Phase : %d\n", reslut);
		}
		else
		{
			MessageBox(nullptr, L"존재하지 않는 페이즈입니다.", L"안내", MB_OK);
		}
	}
}

void Client::NoteEditor::SelectBgm()
{
	std::wstring bgmName = WinUtility::GetStringFromUser(L"사용할 BGM 선택", L"Assets Sound에 존재하는 파일명을 입력해주세요.");
	if (bgmName != L"")
	{
		if (Sound->FindSound(UtfConvert::wstring_to_utf8(bgmName)))
		{
			_pBGMManager->_bgmList[selectStage][selectPhase] = UtfConvert::wstring_to_utf8(bgmName);
			_pBGMManager->WriteBGMList("Assets/Sound/BGMList.json");
			UpdateBGMManager();
		}
		else
		{
			MessageBox(nullptr, L"존재하지 않는 리소스 입니다. A", L"안내", MB_OK);
		}
	}
}

void Client::NoteEditor::SelectMode(EMODE mode)
{
	if (mode == EMODE::END)
	{
		mode = EMODE::NOTE;
	}
	_selectMode = mode;
	_pTimelineManager->SelectMode = static_cast<int>(mode);
}

void Client::NoteEditor::ResizeStage()
{
	int reslut = WinUtility::GetIntFromUser(L"Resize stage", L"총 스테이지 개수를 입력해주세요.");
	if (0 < reslut)
	{
		_pBGMManager->CreateBGMList(reslut);
		DebugPrintf("Stage Size : %d\n", reslut);
		if (selectStage == -1)
		{
			selectStage = 0;
		}
		else if (selectStage >= _pBGMManager->_bgmList.size())
		{
			selectStage = (int)_pBGMManager->_bgmList.size() - 1;
		}

		for (auto& phase : _pBGMManager->_bgmList)
		{
			if (phase.empty())
			{
				phase.resize(1);
			}
		}
		UpdateBGMManager();
		_pBGMManager->WriteBGMList("Assets/Sound/BGMList.json");
	}
}

void Client::NoteEditor::ResizePhase()
{
	int reslut = WinUtility::GetIntFromUser(L"Resize phase", L"현재 스테이지의 페이즈 개수를 입력해주세요.");
	if (0 < reslut)
	{
		_pBGMManager->CreatePhaseSize(selectStage, reslut);
		DebugPrintf("Phase Size : %d\n", reslut);
		if (selectPhase == -1)
		{
			selectPhase = 0;
		}
		else if (selectPhase >= _pBGMManager->_bgmList[selectStage].size())
		{
			selectPhase = (int)_pBGMManager->_bgmList[selectStage].size() - 1;
		}
		_pBGMManager->WriteBGMList("Assets/Sound/BGMList.json");
	}
}

void Client::NoteEditor::MoveBgmPosition()
{
	float result = WinUtility::GetFloatFromUser(L"Select Time", L"이동할 시간을 입력하세요.");
	if (0 <= result)
	{
		DebugPrintf("Select Time : %f\n", result);
		Sound->SetPosition(CHANNEL_BGM, result * 1e9);
		DebugPrintf("%0.3lF\n", Sound->GetPosition(CHANNEL_BGM));
		UpdateNoteManager();
		UpdateBossManager();
		if (_selectMode == EMODE::NOTE)
		{
			_debugIndex = _pNoteManager->_spawnCount;
		}
		else if (_selectMode == EMODE::BOSS)
		{
			_debugIndex = _pBossManager->_noteIndexAttack;
		}

		if (Manager->GetCurrentStage() == Stage::Stage4)
		{
			dynamic_cast<Lich*>(Manager->GetCurrentBoss())->SetPhase1();
		}
	}
}

void Client::NoteEditor::UpdateNoteManager()
{
	if (_selectStage >= _pBGMManager->_bgmList.size() || _selectPhase >= _pBGMManager->_bgmList[_selectStage].size())
	{
		return;
	}

	if (_pNoteManager)
	{
		long double BGMposition = _pBGMManager->GetPosition;
		for (auto& noteData : _pNoteManager->_noteSpawnData)
		{
			if(noteData._noteTime + _pNoteManager->_noteSpawnOffset >= BGMposition)
			{
				_pNoteManager->_spawnCount = noteData._spawnIndex;
				break;
			}
		}
		while (!_pNoteManager->_noteSpawnQueue.empty())
		{
			_pNoteManager->_noteSpawnQueue.pop_front();
		}
		for (auto note : _pNoteManager->_noteObjectPool)
		{
			note->SetVisible(false);;
		}
	}
}

void Client::NoteEditor::InsertNote()
{
	switch (_selectMode)
	{
	case Client::NoteEditor::EMODE::NOTE:
	{
		auto& noteSpawnData = _pNoteManager->_noteSpawnData;
		NoteManager::NoteSpawnTable newNoteData;
		newNoteData._noteTime = _pBGMManager->GetPosition;
		if (newNoteData._noteTime <= _pNoteManager->_noteSpawnOffset)
		{
			MessageBox(nullptr, L"노트 생성 시간이 노래의 시작부분보다 이전입니다.", L"에러", MB_OK);
			return;
		}

		int index = 0;
		for (auto insertIter = noteSpawnData.begin(); insertIter != noteSpawnData.end(); ++insertIter)
		{
			if (insertIter->_noteTime > newNoteData._noteTime)
			{
				newNoteData._spawnIndex = index;
				_pNoteManager->_noteSpawnData.insert(insertIter, newNoteData);
				for (int i = index + 1; i < _pNoteManager->_noteSpawnData.size(); i++)
				{
					_pNoteManager->_noteSpawnData[i]._spawnIndex++;
				}
				UpdateNoteManager();
				return;
			}
			else if (insertIter->_noteTime == newNoteData._noteTime)
			{
				return;
			}
			index++;
		}

		newNoteData._spawnIndex = (int)noteSpawnData.size();
		_pNoteManager->_noteSpawnData.push_back(newNoteData);
		UpdateNoteManager();
	}
	break;

	case Client::NoteEditor::EMODE::BOSS:
	{
		auto& noteSpawnData = _pBossManager->_noteSpawnData;
		BossManager::NoteTable newNoteData;
		newNoteData._noteTime = _pBGMManager->GetPosition;
		if (_selectBossAttackType == 1)
		{
			newNoteData._attack = true;
		}
		if (_selectBossAttackType == 2)
		{
			newNoteData._skill1 = true;
		}
		if (_selectBossAttackType == 3)
		{
			newNoteData._skill2 = true;
		}

		int index = 0;
		for (auto insertIter = noteSpawnData.begin(); insertIter != noteSpawnData.end(); ++insertIter)
		{
			if (insertIter->_noteTime > newNoteData._noteTime)
			{
				newNoteData._noteIndex = index;
				_pBossManager->_noteSpawnData.insert(insertIter, newNoteData);
				for (int i = index + 1; i < _pBossManager->_noteSpawnData.size(); i++)
				{
					_pBossManager->_noteSpawnData[i]._noteIndex++;
				}
				UpdateBossManager();
				return;
			}
			else if (insertIter->_noteTime == newNoteData._noteTime)
			{
				if (_selectBossAttackType == 1)
				{
					insertIter->_attack = true;
				}
				if (_selectBossAttackType == 2)
				{
					insertIter->_skill1 = true;
				}
				if (_selectBossAttackType == 3)
				{
					insertIter->_skill2 = true;
				}		
				UpdateBossManager();
				return;
			}
			index++;
		}

		newNoteData._noteIndex = (int)noteSpawnData.size();
		_pBossManager->_noteSpawnData.push_back(newNoteData);
		UpdateBossManager();
	}
	break;



	default:
		break;
	}
}

void Client::NoteEditor::DeleteNote()
{
	switch (_selectMode)
	{
	case Client::NoteEditor::EMODE::NOTE:
	{
		if (_pNoteManager)
		{
			if (!_pNoteManager->_noteSpawnQueue.empty())
			{
				auto& spawnQueue = _pNoteManager->_noteSpawnQueue;
				auto& spawnData = _pNoteManager->_noteSpawnData;
				auto& objectPool = _pNoteManager->_noteObjectPool;
				long double nextTime = -1.f;
				long double prevTime = -1.f;
				int nextIndex = 0;
				int prevIndex = (int)spawnQueue.size() - 1;
				for (auto iter = spawnQueue.begin(); iter != spawnQueue.end(); ++iter)
				{
					if (iter->_noteTime >= _pBGMManager->GetPosition)
					{
						nextTime = iter->_noteTime - _pBGMManager->GetPosition;
						break;
					}
					nextIndex++;
				}
				for (auto iter = spawnQueue.rbegin(); iter != spawnQueue.rend(); ++iter)
				{
					if (iter->_noteTime <= _pBGMManager->GetPosition)
					{
						prevTime = _pBGMManager->GetPosition - iter->_noteTime;
						break;
					}
					prevIndex--;
				}
				NoteManager::NoteSpawnTable queueData{};
				if (nextTime < 0 && prevTime < 0)
				{
					return;
				}
				else
				{
					if (nextTime < 0)
					{
						queueData = spawnQueue[prevIndex];
					}
					else if (prevTime < 0)
					{
						queueData = spawnQueue[nextIndex];
					}
					else
					{
						queueData = (nextTime < prevTime) ? spawnQueue[nextIndex] : spawnQueue[prevIndex];
					}
				}
				spawnData.erase(spawnData.begin() + queueData._spawnIndex);
				std::cout << queueData._spawnIndex << "번 노트 삭제" << std::endl;
				for (size_t i = queueData._spawnIndex; i < spawnData.size(); i++)
				{
					spawnData[i]._spawnIndex--;
				}
				UpdateNoteManager();
			}
		}
	}
	break;


	case Client::NoteEditor::EMODE::BOSS:
	{
		if (_pBossManager)
		{
			auto& spawnData = _pBossManager->_noteSpawnData;

			if (spawnData.empty())
				return;

			long double nextTime = -1.f;
			long double prevTime = -1.f;

			std::optional<std::vector<BossManager::NoteTable>::iterator> nextIter = std::nullopt;
			std::optional<std::vector<BossManager::NoteTable>::iterator> prevIter = std::nullopt;

			std::vector<BossManager::NoteTable>::iterator deleteDataIter;
			for (auto iter = spawnData.begin(); ; ++iter)
			{
				if (iter->_noteTime >= _pBGMManager->GetPosition)
				{
					nextTime = iter->_noteTime - _pBGMManager->GetPosition;
					nextIter = std::optional{iter};
					break;
				}
				if (iter == std::prev(spawnData.end()))
				{
					nextIter = std::nullopt;
					break;
				}
			}
			for (auto iter = std::prev(spawnData.end()); ; --iter)
			{
				if (iter->_noteTime <= _pBGMManager->GetPosition)
				{
					prevTime = _pBGMManager->GetPosition - iter->_noteTime;
					prevIter = std::optional{iter};
					break;
				}
				if (iter == spawnData.begin())
				{
					prevIter = std::nullopt;
					break;
				}
			}
			BossManager::NoteTable noteData{};
			if ((nextTime < 0) && (prevTime < 0))
			{
				return;
			}
			else
			{
				if (nextTime < 0)
				{
					if (prevIter != std::nullopt)
					{
						noteData = *prevIter.value();
						deleteDataIter = prevIter.value();
					}		
				}
				else if (prevTime < 0)
				{
					if (nextIter != std::nullopt)
					{
						noteData = *nextIter.value();
						deleteDataIter = nextIter.value();
					}	
				}
				else
				{
					if (nextTime < prevTime)
					{
						if (nextIter.has_value())  // 변경된 부분 -> 값이 있는지 사전검사
						{
							noteData = *nextIter.value();
							deleteDataIter = nextIter.value();
						}
					}
					else
					{
						if (prevIter.has_value())  // 변경된 부분 -> 값이 있는지 사전검사
						{
							noteData = *prevIter.value();
							deleteDataIter = prevIter.value();
						}
					}
				}
			}
			spawnData.erase(deleteDataIter);
			std::cout << noteData._noteIndex << "번 노트 삭제" << std::endl;				 
			for (size_t i = noteData._noteIndex; i < spawnData.size(); i++)
			{
				spawnData[i]._noteIndex--;
			}
			UpdateBossManager();
		}
	}
	break;

	default:
		break;
	}
	

}

void Client::NoteEditor::UpdateDebugText()
{
	_pEditorDebugText->Text = L"Select Stage : F1, Select Phase : F2, F3 : Resize stage, F4 : Resize Phase, F8 : Mode Change\n";
	_pEditorDebugText->Text += _selectMode == EMODE::NOTE ? L"Mode : Player Note\n" : L"Mode : Boss Patten Note\n";
	if (selectStage != -1)
	{
		_pEditorDebugText->Text += (std::wstring(L"stage count : ") + std::to_wstring(_pBGMManager->_bgmList.size())).c_str();
		_pEditorDebugText->Text += (std::wstring(L"\nPhase count : ") + std::to_wstring(_pBGMManager->_bgmList[_selectStage].size())).c_str();
	
		size_t noteCount{};
		switch (_selectMode)
		{
		case Client::NoteEditor::EMODE::NOTE:
			noteCount = _pNoteManager->_noteSpawnData.size();
			break;
		case Client::NoteEditor::EMODE::BOSS:
			_pEditorDebugText->Text += (std::string("   AttackType : ") + std::to_string(_selectBossAttackType)).c_str();
			noteCount = _pBossManager->_noteSpawnData.size();
			break;
		default:
			break;
		}
		_pEditorDebugText->Text += (std::wstring(L"\nSelect stage : ") + std::to_wstring(_selectStage + 1) + L"   Note count : " + std::to_wstring(noteCount)).c_str();
	}	
	else
	{
		_pEditorDebugText->Text += L"stage count : NULL";
		_pEditorDebugText->Text += L"\nPhase count : NULL";
		_pEditorDebugText->Text += L"\nSelect stage : NULL";
	}
	if (selectPhase != -1)
	{
		std::wstring timePosition = std::to_wstring(_pBGMManager->GetPosition / (long double)1e9);
		_pEditorDebugText->Text += (std::wstring(L"\nSelect Phase : ") + std::to_wstring(_selectPhase + 1) +
			std::wstring(L"   bgm : ") + UtfConvert::utf8_to_wstring((_pBGMManager->_bgmList[_selectStage][_selectPhase])) +
			std::wstring(L"   Time position : ") + timePosition.substr(0, timePosition.rfind(L".") + 5)
			).c_str();
	}	
	else
	{
		_pEditorDebugText->Text += L"\nSelect Phase : NULL";
	}

	static std::wstring noteRating(L"");
	if (_pNoteManager->IsPerfect)
	{
		noteRating = L"Perfect";
	}
	if (_pNoteManager->IsGreat)
	{
		noteRating = L"Great";
	}
	if (_pNoteManager->IsGood)
	{
		noteRating = L"Good";
	}
	if (_pNoteManager->IsMiss)
	{
		noteRating = L"Miss";
	}
	_pEditorDebugText->Text += (std::wstring(L"\n판정 : ") + noteRating).c_str();
	_pEditorDebugText->Text += (std::wstring(L"   FPS : ") + std::to_wstring(Time->GetFPS())).c_str();


}

void Client::NoteEditor::UpdateNoteListText()
{
	if (Sound->GetPause(CHANNEL_BGM) == false)
	{
		switch (_selectMode)
		{
		case Client::NoteEditor::EMODE::NOTE:
			if (_pNoteManager->_noteSpawnQueue.empty())
			{
				_debugIndex = _pNoteManager->_spawnCount;
			}
			else
			{
				_debugIndex = _pNoteManager->_noteSpawnQueue.front()._spawnIndex;
			}
			break;
		case Client::NoteEditor::EMODE::BOSS:
			_debugIndex = _pBossManager->_noteIndexAttack;
			break;
		default:
			break;
		}	
	}
	else
	{
		if (!_pInputComponent->IsKeyHold(DIK_LCONTROL))
		{
			float mWheel = _pInputComponent->GetMouseWheel();
			int lastIndex;
			if (_selectMode == Client::NoteEditor::EMODE::NOTE)
				lastIndex = (int)_pNoteManager->_noteSpawnData.size() - 1;
			else
				lastIndex = (int)_pBossManager->_noteSpawnData.size() - 1;

			if (lastIndex < 0)
				lastIndex = 0;

			if (mWheel > 0.f)
			{
				//std::cout << "wheel : " << mWheel << std::endl;
				_debugIndex = std::clamp(--_debugIndex, 0, lastIndex);
			}
			else if (mWheel < 0.f)
			{
				//std::cout << "wheel : " << mWheel << std::endl;
				_debugIndex = std::clamp(++_debugIndex, 0, lastIndex);
			}
		}
	}

	_pNoteListText->Text = "";
	
	switch (_selectMode)
	{
	case Client::NoteEditor::EMODE::NOTE:
		if (_pNoteManager)
		{
			for (auto& data : _pNoteManager->_noteSpawnData)
			{
				if (data._spawnIndex < _debugIndex)
				{
					continue;
				}

				_pNoteListText->Text += (std::string("index : ") + std::to_string(data._spawnIndex) +
					"\ntime : " + std::to_string(data._noteTime / (long double)1e9) +
					"\n\n").c_str();
			}
		}
		break;
	case Client::NoteEditor::EMODE::BOSS:
		if (_pBossManager)
		{
			for (auto& data : _pBossManager->_noteSpawnData)
			{
				if (data._noteIndex < _debugIndex)
				{
					continue;
				}
				_pNoteListText->Text += (std::string("index : ") + std::to_string(data._noteIndex) +
					"\ntime : " + std::to_string(data._noteTime / (long double)1e9) +
					"\nattack : " + std::to_string(data._attack) +
					"\nskill1 : " + std::to_string(data._skill1) +
					"\nskill2 : " + std::to_string(data._skill2) +
					"\n\n").c_str();
			}
		}
		break;
	default:
		break;
	}

}

void Client::NoteEditor::UpdateBGMManager()
{
	if (_selectStage >= _pBGMManager->_bgmList.size() || _selectPhase >= _pBGMManager->_bgmList[_selectStage].size())
	{
		return;
	}
	_pBGMManager->_currentStage = _selectStage;
	_pBGMManager->_currentPhase = _selectPhase;	
}

void Client::NoteEditor::SaveNoteToCSV()
{
	std::wstring basePath = L"Assets/NoteCsv/" + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1);
	CreateDirectory(basePath.c_str(), nullptr);

	std::wstring writePath = std::wstring(L"Assets/NoteCsv/") + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1) + L"/"
		+ std::wstring(L"phase") + std::to_wstring(_selectPhase + 1) + std::wstring(L".csv");
	CSVWriter<int, long double> writer(writePath);
	for (auto& data : _pNoteManager->_noteSpawnData)
	{
		writer.addRow(std::make_tuple(data._spawnIndex, data._noteTime));
	}
	writer.writeFile({ L"노트 인덱스", L"음악 포지션" });
}

void Client::NoteEditor::LoadNoteCSV()
{
	if (_selectStage >= _pBGMManager->_bgmList.size() || _selectPhase >= _pBGMManager->_bgmList[_selectStage].size())
	{
		return;
	}
	
	std::wstring basePath = L"Assets/NoteCsv/" + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1);
	CreateDirectory(basePath.c_str(), nullptr);

	std::wstring writePath = std::wstring(L"Assets/NoteCsv/") + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1) + L"/"
		+ std::wstring(L"phase") + std::to_wstring(_selectPhase + 1) + std::wstring(L".csv");
	try
	{
		_pNoteManager->SetNoteSpawnData(UtfConvert::wstring_to_utf8(writePath));
	}
	catch (const std::exception&)
	{
		NewFileNoteCSV(writePath);
		_pNoteManager->SetNoteSpawnData(UtfConvert::wstring_to_utf8(writePath));
	}
}

void Client::NoteEditor::NewFileNoteCSV(const std::wstring& path)
{
	CSVWriter<int, long double> writer(path);
	writer.writeFile({ L"노트 인덱스", L"음악 포지션" });
}

//void Client::NoteEditor::InsertBossPattern(int pattenNum)
//{
//	auto& noteSpawnData = _pBossManager->_noteSpawnData;
//	BossManager::NoteTable newNoteData;
//	
//	newNoteData._noteTime = _pBGMManager->GetPosition;
//	if (newNoteData._noteTime <= _pBossManager->GetPattenOffset(pattenNum))
//	{
//		MessageBox(nullptr, L"노트 생성 시간이 노래의 시작부분보다 이전입니다.", L"에러", MB_OK);
//		return;
//	}
//
//	newNoteData._noteType = pattenNum;
//	int index = 0;
//	for (auto insertIter = noteSpawnData.begin(); insertIter != noteSpawnData.end(); ++insertIter)
//	{
//		if (insertIter->_noteTime > newNoteData._noteTime)
//		{
//			newNoteData._noteIndex = index;
//			_pBossManager->_noteSpawnData.insert(insertIter, newNoteData);
//			for (int i = index + 1; i < _pBossManager->_noteSpawnData.size(); i++)
//			{
//				_pBossManager->_noteSpawnData[i]._noteIndex++;
//			}
//			UpdateBossManager();
//			return;
//		}
//		else if (insertIter->_noteTime == newNoteData._noteTime)
//		{
//			return;
//		}
//		index++;
//	}
//
//	newNoteData._noteIndex = (int)noteSpawnData.size();
//	_pBossManager->_noteSpawnData.push_back(newNoteData);
//	UpdateBossManager();
//	return;
//}

//void Client::NoteEditor::DeleteBossPattern()
//{
//	if (_pBossManager)
//	{
//		if (!_pBossManager->_noteSpawnData.empty())
//		{
//			auto& spawnData = _pBossManager->_noteSpawnData;
//			//auto& objectPool = _pBossManager->_noteObjectPool;
//			spawnData.erase(spawnData.begin() + _pBossManager->_noteIndex);
//			std::cout << _pBossManager->_noteIndex << "번 노트 삭제" << std::endl;
//			for (size_t i = _pBossManager->_noteIndex; i < spawnData.size(); i++)
//			{
//				spawnData[i]._noteIndex--;
//			}
//			UpdateBossManager();
//		}
//	}
//
//}

void Client::NoteEditor::UpdateBossManager()
{
	if (_selectStage >= _pBGMManager->_bgmList.size() || _selectPhase >= _pBGMManager->_bgmList[_selectStage].size())
	{
		return;
	}

	if (_pBossManager)
	{
		long double BGMposition = _pBGMManager->GetPosition;
		for (auto& noteData : _pBossManager->_noteSpawnData)
		{
			long double pattenOffset{0};
			if (noteData._attack)
			{
				pattenOffset = _pBossManager->GetPattenOffset(1);
			}
			if (noteData._skill1)
			{
				pattenOffset = _pBossManager->GetPattenOffset(2);
		    }		
			if (noteData._skill2)
			{
				pattenOffset = _pBossManager->GetPattenOffset(3);
			}
			
			if (noteData._noteTime - pattenOffset >= BGMposition)
			{
				_pBossManager->_noteIndexAttack = noteData._noteIndex;
				_pBossManager->_noteIndexSkill1 = noteData._noteIndex;
				_pBossManager->_noteIndexSkill2 = noteData._noteIndex;
				break;
			}
		}
	}
}

void Client::NoteEditor::SaveBossPatternToCSV()
{
	std::wstring basePath = L"Assets/BossPattenCsv/" + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1);
	CreateDirectory(basePath.c_str(), nullptr);

	std::wstring writePath = std::wstring(L"Assets/BossPattenCsv/") + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1) + L"/"
		+ std::wstring(L"phase") + std::to_wstring(_selectPhase + 1) + std::wstring(L".csv");
	CSVWriter<int, long double, bool, bool, bool> writer(writePath);
	for (auto& data : _pBossManager->_noteSpawnData)
	{
		writer.addRow(std::make_tuple(data._noteIndex, data._noteTime,data._attack, data._skill1, data._skill2));
	}
	writer.writeFile({ L"노트 인덱스", L"음악 포지션" ,L"공격", L"스킬1", L"스킬2"});
}

void Client::NoteEditor::LoadBossPatternCSV()
{
	if (_selectStage >= _pBGMManager->_bgmList.size() || _selectPhase >= _pBGMManager->_bgmList[_selectStage].size())
	{
		return;
	}

	std::wstring basePath = L"Assets/BossPattenCsv/" + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1);
	CreateDirectory(basePath.c_str(), nullptr);

	std::wstring writePath = std::wstring(L"Assets/BossPattenCsv/") + std::wstring(L"Stage") + std::to_wstring(_selectStage + 1) + L"/"
		+ std::wstring(L"phase") + std::to_wstring(_selectPhase + 1) + std::wstring(L".csv");
	try
	{
		_pBossManager->SetNoteSpawnData(UtfConvert::wstring_to_utf8(writePath));
	}
	catch (const std::exception&)
	{
		NewFileBossPatternCSV(writePath);
		_pBossManager->SetNoteSpawnData(UtfConvert::wstring_to_utf8(writePath));
	}
}

void Client::NoteEditor::NewFileBossPatternCSV(const std::wstring& path)
{
	CSVWriter<int, long double,bool,bool,bool> writer(path);
	writer.writeFile({ L"노트 인덱스", L"음악 포지션" ,L"공격", L"스킬1", L"스킬2"});
}
