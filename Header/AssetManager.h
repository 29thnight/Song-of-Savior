#pragma once
#include <CoreDefine.h>
#include <CoreBase.h>
#include <Singleton.h>

namespace Engine
{
	CORECLASS()
		class AssetManager : public CoreBase, public Singleton<AssetManager>
	{
	private:
		friend class Singleton;

	private:
		AssetManager() = default;
		virtual ~AssetManager() = default;

	public:
		using AssetRegister = std::function<Actor* ()>;
		static Actor* InitializeActor(const std::string& assetTypeName)
		{
			auto iter = GetAssetRegister().find(assetTypeName);
			if (iter != GetAssetRegister().end())
			{
				return iter->second();
			}
			else
			{
				return nullptr;
			}
		}

		static bool RegisterActor(const std::string& assetTypeName, AssetRegister _register)
		{
			GetAssetRegister()[assetTypeName] = _register;
			return true;
		}

		static std::unordered_map<std::string, AssetRegister>& GetAssetRegister()
		{
			static std::unordered_map<std::string, AssetRegister> assetRegister;
			return assetRegister;
		}
	};
}

template <typename AssetTypeName>
inline bool RegisterActor() {

	static_assert(std::is_base_of<Engine::Actor, AssetTypeName>::value, "AssetTypeName is not derived from Actor");

	std::function<Engine::Actor*()> initializer = AssetTypeName::Create;
	std::string name = typeid(AssetTypeName).name();

	return Engine::AssetManager::RegisterActor(name.substr(6), initializer);
}