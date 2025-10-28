#include <BGMManager.h>
#include <SoundManager.h>
#include <TimeManager.h>
#include <GameManager.h>
#include "../../ThirdParty/nlohmann/json.hpp"

void Client::BGMManager::BeginPlay()
{
	Base::BeginPlay();
	//BGM ����Ʈ�� �ҷ��´�.
	nlohmann::json json;
	std::ifstream bgmListFiles("Assets/Sound/BGMList.json");
	if (!bgmListFiles.is_open())
	{
		assert(!"BGMList.json ������ �����ϴ�.");
	}

	bgmListFiles >> json;
	if (json.contains("StageNumber"))
	{
		json["StageNumber"].get_to(_stageNumber);
	}

	for (int i = 0; i < _stageNumber; ++i)
	{
		std::string stage = "Stage" + std::to_string(i+1);
		if (json.contains(stage))
		{
			std::vector<std::string> bgmList;
			json[stage].get_to(bgmList);
			_bgmList.push_back(bgmList);
		}
		else
		{
			assert(!"BGMList.json ������ �߸��Ǿ����ϴ�.");
		}
	}
}

void Client::BGMManager::Tick(_duration deltaSeconds)
{
	Base::Tick(deltaSeconds);

	if (_isDelayedPlayBGM)
	{
		_delayTime -= Time->DurationToFloat(deltaSeconds);
		if (_delayTime <= 0)
		{
			PauseBGM();
			_isDelayedPlayBGM = false;
		}
	}
}

void Client::BGMManager::Fixed()
{
}

void Client::BGMManager::EndPlay()
{
	PauseBGM();
}

void Client::BGMManager::ReviveInitialize()
{
}

void Client::BGMManager::CreateBGMList(int stageCount)
{
	_bgmList.resize(stageCount);
	_stageNumber = stageCount;
}
void Client::BGMManager::CreatePhaseSize(int stageIndex, int phaseSize)
{
	if (stageIndex < _bgmList.size())
	{
		_bgmList[stageIndex].resize(phaseSize);
	}
	else
	{
		DebugPrintf("�������� �ʴ� �������� �Դϴ�.\n");
	}
}

void Client::BGMManager::ReadBGMList(std::string filePath)
{
	nlohmann::json json;
	std::ifstream bgmListFiles(filePath);
	if (!bgmListFiles.is_open())
	{
		assert(!"BGMList.json ������ �����ϴ�.");
	}

	bgmListFiles >> json;
	if (json.contains("StageNumber"))
	{
		json["StageNumber"].get_to(_stageNumber);
	}

	for (int i = 0; i < _stageNumber; ++i)
	{
		std::string stage = "Stage" + std::to_string(i+1);
		if (json.contains(stage))
		{
			std::vector<std::string> bgmList;
			json[stage].get_to(bgmList);
			_bgmList.push_back(bgmList);
		}
		else
		{
			assert(!"BGMList.json ������ �߸��Ǿ����ϴ�.");
		}
	}
}

void Client::BGMManager::SetBGMList(int stageNumber, int phaseNumber, std::string filePath)
{
	_bgmList[stageNumber][phaseNumber] = filePath;
}

void Client::BGMManager::WriteBGMList(std::string filePath) const
{
	nlohmann::json json;
	json["StageNumber"] = _stageNumber;

	for (int i = 0; i < _stageNumber; ++i)
	{
		std::string stage = "Stage" + std::to_string(i + 1);
		json[stage] = _bgmList[i];
	}

	std::ofstream bgmListFiles(filePath);
	if (!bgmListFiles.is_open())
	{
		assert(!"BGMList.json ������ ������ �� �����ϴ�.");
	}

	bgmListFiles << json.dump(4);
}

void Client::BGMManager::DelayedPlayBGM(float delayTime)
{
	_delayTime = delayTime;
	_isDelayedPlayBGM = true;
	PlayBGM();
	PauseBGM();
}

void Client::BGMManager::PlayBGM()
{
	if (_isPlayBGM)
		StopBGM(); //�뷡 �ߺ���� ����

	Sound->PlaySound(_bgmList[_currentStage][_currentPhase].c_str(), CHANNEL_BGM, NOT_LOOP);
	_isPlayBGM = true;
}

bool Client::BGMManager::PauseBGM()
{
	if (_isPlayBGM)
	{
		bool isPause = Sound->GetPause(CHANNEL_BGM);
		Sound->SetPause(CHANNEL_BGM, !isPause);
		return !isPause;
	}
}

void Client::BGMManager::StopBGM()
{
	if (_isPlayBGM)
	{
		Sound->StopSound(CHANNEL_BGM);
		_isPlayBGM = false;
	}
}