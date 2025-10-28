#include "TimelineManager.h"
#include <World.h>
#include <CoreManager.h>
#include <SceneComponent.h>
#include <TextureManager.h>
#include <cassert>

#include "NoteEditer.h"
#include "TimelineBlock.h"
#include "CurrTimeBlock.h"
#include "BGMManager.h"
#include "NoteManager.h"
#include "TimelineCurrNote.h"
#include "BossManager.h"


void Client::TimelineManager::BeginPlay()
{
	const int screenWidth = Management->setting.width;
	const int screenHeight = Management->setting.height;

	SpawnTimeline();

	_pBGMManager = dynamic_cast<BGMManager*>(GetWorld()->FindActor("BGMManager"));
	if (!_pBGMManager)
	{
		assert(!"BGMManager가 없습니다.");
	}

	_pNoteManager = dynamic_cast<NoteManager*>(GetWorld()->FindActor("NoteManager"));
	if (!_pNoteManager)
	{
		assert(!"NoteManager가 없습니다.");
	}

	_pBossManager = dynamic_cast<BossManager*>(GetWorld()->FindActor("BossManager"));
	if (!_pBossManager)
	{
		assert(!"BossManager가 없습니다.");
	}

	GetWorld()->SpawnActor(LAYER::BACKOBJECT, "CurrTimeBlock", CurrTimeBlock::Create());
	_pCurrTimeBlock = dynamic_cast<CurrTimeBlock*>(GetWorld()->FindActor("CurrTimeBlock"));
	UpdateCurrTimeBlock();

}

void Client::TimelineManager::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);
	UpdateCurrTimeBlock();
	if (_isDestroyTimelineBlock)
	{
		if (GetWorld()->FindActor("TimelineBlock_0") == nullptr)
		{
			SpawnTimeline();
			_isDestroyTimelineBlock = false;
		}
	}

	SpawnCurrNote();
}

void Client::TimelineManager::Fixed()
{
}

void Client::TimelineManager::EndPlay()
{
	DestroyTimeline();
}

void Client::TimelineManager::Destroy()
{
	Actor::Destroy();
}

void Client::TimelineManager::DestroyTimeline()
{
	for (int i = 0; i < _timelineBlockCount; ++i)
	{
		std::string name = std::string("TimelineBlock_") + std::to_string(i);
		Engine::Actor* block = GetWorld()->FindActor(name.c_str());
		if (block)
		{
			GetWorld()->TerminateActor(block->GetName());
		}
		else
		{
			break;
		}
	}
	//GetWorld()->TerminateActor("CurrTimeBlock");
}

void Client::TimelineManager::SpawnTimeline()
{
	for (int i = 0; i < _timelineBlockCount; ++i)
	{
		std::string name = std::string("TimelineBlock_") + std::to_string(i);
		GetWorld()->SpawnActor(LAYER::BACKGROUND, name.c_str(), TimelineBlock::Create());
		TimelineBlock* timelineBlock = dynamic_cast<TimelineBlock*>(GetWorld()->FindActor(name.c_str()));

		long double currBGMPosition = (long double)i * (long double)0.016 * (long double)1e9;
		long double lineTimeSize = _timelineBlockCount * (long double)0.016 * (long double)1e9;
		currBGMPosition = fmodl(currBGMPosition, lineTimeSize);

		float screenHeight = (float)Management->setting.height;
		Mathf::Vector2 size = timelineBlock->GetBitmapSize(0);
		_startPosition = size.x * 0.5f;
		_endPosition = Management->setting.width - size.x * 0.5f;

		long double ldPositionX = (long double)_startPosition + (currBGMPosition / lineTimeSize) * (long double)(_endPosition - _startPosition);
		float positionX = static_cast<float>(ldPositionX);
		timelineBlock->GetRootComponent()->SetRelativeLocation({ positionX, screenHeight - size.y * 0.5f });
	}
}

void Client::TimelineManager::UpdateCurrTimeBlock()
{
	if (_pCurrTimeBlock)
	{
		long double currBGMPosition = _pBGMManager->GetPosition;
		long double lineTimeSize = _timelineBlockCount * (long double)0.016 * (long double)1e9;
		currBGMPosition = fmodl(currBGMPosition ,lineTimeSize);

		float screenHeight = (float)Management->setting.height;
		Mathf::Vector2 size = _pCurrTimeBlock->GetBitmapSize(0);

		long double ldPositionX = (long double)_startPosition + (currBGMPosition / lineTimeSize) * (long double)(_endPosition - _startPosition);
		float positionX = static_cast<float>(ldPositionX);
		_pCurrTimeBlock->GetRootComponent()->SetRelativeLocation({ positionX, screenHeight - size.y * 0.5f });
	}
}

void Client::TimelineManager::SpawnCurrNote()
{
	static long double prevLine = 0;
	long double lineTimeSize = _timelineBlockCount * (long double)0.016 * (long double)1e9;
	long double currBGMPosition = _pBGMManager->GetPosition;
	long double currLine = int(currBGMPosition / lineTimeSize);
	if (_isSpawnCurrNote)
	{
		long double currStart = currLine * lineTimeSize;
		long double currEnd = (currLine + (long double)1.0) * lineTimeSize;
		
		NoteEditor::EMODE eMode = static_cast<NoteEditor::EMODE>(_selectMode);
		switch (eMode)
		{
		case Client::NoteEditor::EMODE::NOTE:
		{
			std::vector<NoteManager::NoteSpawnTable> noteSpawnData(_pNoteManager->_noteSpawnData);
			if (!noteSpawnData.empty())
			{
				auto iter = std::remove_if(noteSpawnData.begin(), noteSpawnData.end(),
					[&](NoteManager::NoteSpawnTable data)->bool
					{
						return (data._noteTime < currStart);
					});
				if (iter != noteSpawnData.end())
				{
					noteSpawnData.erase(iter, noteSpawnData.end());
				}

				if (!noteSpawnData.empty())
				{
					iter = std::remove_if(noteSpawnData.begin(), noteSpawnData.end(),
						[&](NoteManager::NoteSpawnTable data)->bool
						{
							return (data._noteTime > currEnd);
						});
					if (iter != noteSpawnData.end())
					{
						noteSpawnData.erase(iter, noteSpawnData.end());
					}
				}
			}

			int index = 0;
			for (auto& data : noteSpawnData)
			{
				timelineCurrNoteArray.push_back(TimelineCurrNote::Create());
				std::string name = std::string("TimelineCurrNote_") + std::to_string(index);
				GetWorld()->SpawnActor(LAYER::OBJECT, name.c_str(), timelineCurrNoteArray.back());

				Mathf::Vector2 size = timelineCurrNoteArray.back()->GetBitmapSize(0);

				long double ldPositionX = (data._noteTime - currStart) / (currEnd - currStart) * (long double)(_endPosition - _startPosition);
				timelineCurrNoteArray.back()->GetRootComponent()->AddRelativeLocation({ (float)ldPositionX , Management->setting.height - size.y * 0.5f });
				index++;
			}
			_isSpawnCurrNote = false;
		}
			break;
		case Client::NoteEditor::EMODE::BOSS:
		{
			std::vector<BossManager::NoteTable> noteSpawnData(_pBossManager->_noteSpawnData);
			if (!noteSpawnData.empty())
			{
				auto iter = std::remove_if(noteSpawnData.begin(), noteSpawnData.end(),
					[&](BossManager::NoteTable data)->bool
					{
						return (data._noteTime < currStart);
					});
				if (iter != noteSpawnData.end())
				{
					noteSpawnData.erase(iter, noteSpawnData.end());
				}

				if (!noteSpawnData.empty())
				{
					iter = std::remove_if(noteSpawnData.begin(), noteSpawnData.end(),
						[&](BossManager::NoteTable data)->bool
						{
							return (data._noteTime > currEnd);
						});
					if (iter != noteSpawnData.end())
					{
						noteSpawnData.erase(iter, noteSpawnData.end());
					}
				}
			}

			int index = 0;
			for (auto& data : noteSpawnData)
			{
				timelineCurrNoteArray.push_back(TimelineCurrNote::Create());
				std::string name = std::string("TimelineCurrNote_") + std::to_string(index);
				GetWorld()->SpawnActor(LAYER::OBJECT, name.c_str(), timelineCurrNoteArray.back());

				Mathf::Vector2 size = timelineCurrNoteArray.back()->GetBitmapSize(0);

				long double ldPositionX = (data._noteTime - currStart) / (currEnd - currStart) * (long double)(_endPosition - _startPosition);
				timelineCurrNoteArray.back()->GetRootComponent()->AddRelativeLocation({ (float)ldPositionX , Management->setting.height - size.y * 0.5f });
				index++;
			}
			_isSpawnCurrNote = false;
		}
			break;
		default:
			break;
		}
	}	
	if (prevLine != currLine || _updateNote)
	{
		_isSpawnCurrNote = true;
		for (auto& currNote : timelineCurrNoteArray)
		{
			GetWorld()->TerminateActor(currNote->GetName());
		}
		timelineCurrNoteArray.clear();
		_updateNote = false;
	}
	prevLine = currLine;
}
