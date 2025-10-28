#pragma once
#include <CoreManager.h>
#include <CoreDefine.h>
#include <CoreBase.h>

namespace Engine
{
	class System : public CoreBase
	{
	private:
		explicit System() = default;
		~System() = default;

	public:
		System(const System&) = delete;
		System(System&&) = delete;
		System& operator=(System&&) = delete;
		System& operator=(const System&) = delete;

	public:
		bool MainLoop();

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	private:
		bool Initialize(GameSettings&& gameSettings);
		void Destroy() override;

	public:
		static System* Create(GameSettings&& gameSettings);

	private:
		CoreManager*		_pCoreManager{ nullptr };
	};
}