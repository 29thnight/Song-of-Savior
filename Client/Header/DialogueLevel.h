#pragma once
#include <Level.h>

namespace Client
{
	class Dialogue;
	class DialogueLevel : public Engine::Level
	{
	private:
		DialogueLevel() = default;
		virtual ~DialogueLevel() = default;
	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void EndPlay() override;
	public:
		static DialogueLevel* Create() { return new DialogueLevel; }

	private:
		Dialogue* _pDialogue{ nullptr };
	};
}