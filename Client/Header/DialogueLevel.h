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
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;
	public:
		static DialogueLevel* Create() { return new DialogueLevel; }

	private:
		Dialogue* _pDialogue{ nullptr };
	};
}