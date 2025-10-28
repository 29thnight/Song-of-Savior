#pragma once
#include <Level.h>

namespace Client
{
	class Stage1Level : public Engine::Level
	{
	private:
		Stage1Level() = default;
		virtual ~Stage1Level() = default;

	public:
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static Stage1Level* Create() { return new Stage1Level; }

	private:

	};
};