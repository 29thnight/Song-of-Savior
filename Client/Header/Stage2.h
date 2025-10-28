#pragma once
#include <Level.h>

namespace Client
{
	class Stage2Level : public Engine::Level
	{
	private:
		Stage2Level() = default;
		virtual ~Stage2Level() = default;

	public:
		// Level��(��) ���� ��ӵ�
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static Stage2Level* Create() { return new Stage2Level; }

	private:

	};
};