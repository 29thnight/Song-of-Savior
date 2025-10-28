#pragma once
#include <Level.h>

namespace Client
{
	class Stage3Level : public Engine::Level
	{
	private:
		Stage3Level() = default;
		virtual ~Stage3Level() = default;

	public:
		// Level을(를) 통해 상속됨
		bool BeginPlay() override;
		void EndPlay() override;

	public:
		static Stage3Level* Create() { return new Stage3Level; }

	private:

	};

};