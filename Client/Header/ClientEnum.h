#pragma once

namespace Client
{
	enum class Stage
	{
		Title,
		Editor,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Dialouge,
		Stage4_2,
		Credit,
		LoadingLevel,
		Setting,
		GameOver
	};

	enum class CharacterType
	{
		Warrior,
		Thief,
		Sorcerer,
		Bard,
		All
	};

	enum class Difficulty
	{
		Normal,
		Hard
	};

	enum class GameStatus
	{
		Playing,
		Pause,
		Resume
	};

	enum class GameKeySettingType
	{
		A,
		B,
		GamePad
	};
}

inline Client::Stage& operator++(Client::Stage& stage, int) //연산자 오버로드 type safety때문에 주의가 필요
{
	if (stage == Client::Stage::LoadingLevel)
	{
		return stage;
	}
	return stage = static_cast<Client::Stage>(static_cast<int>(stage) + 1);
}

inline Client::GameStatus& operator++(Client::GameStatus& status, int)
{
	status = static_cast<Client::GameStatus>((static_cast<int>(status) + 1) % 3);

	return status;
}