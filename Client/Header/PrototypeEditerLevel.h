#pragma once
#include <Level.h>

namespace Client
{
	class PrototypeEditorLevel : public Engine::Level
	{
	private:
		PrototypeEditorLevel() = default;
		virtual ~PrototypeEditorLevel() = default;

	public:
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static PrototypeEditorLevel* Create() { return new PrototypeEditorLevel; }
	};
};