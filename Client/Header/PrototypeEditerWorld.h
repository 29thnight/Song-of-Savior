#pragma once
#include <World.h>

namespace Client
{
	class PrototypeEditorLevel;
	class PrototypeEditorWorld : public Engine::World
	{
	private:
		PrototypeEditorWorld() = default;
		virtual ~PrototypeEditorWorld() = default;

	public:
		bool BeginPlay() override;

	public:
		static PrototypeEditorWorld* Create();
		void Destroy() override;

	private:
		PrototypeEditorLevel* _pPrototypeEditorLevel{ nullptr };
	};
}