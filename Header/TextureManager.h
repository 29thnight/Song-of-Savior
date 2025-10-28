#pragma once
#include <CoreDefine.h>
#include <Singleton.h>
#include <ObjectPtr.h>
#include <Animation.h>
#include <ThreadPool.h>

namespace file = std::filesystem;
namespace Engine
{
	CORECLASS()
	class TextureManager : public CoreBase, public Singleton<TextureManager>
	{
	private:
		friend class Singleton;

	private:
		using Textures = std::unordered_map<std::wstring, ObjectPtr<Texture>>;
		using Animations = std::unordered_map<std::wstring, ObjectPtr<Animation>>;

	private:
		TextureManager() = default;
		~TextureManager() = default;

	public:
		bool LoadTexture(LPCWSTR filePath);
		Texture* FindTexture(_pwstring fileTag);
		Animation* FindAnimation(_pwstring fileTag);
		Textures& GetTextures() { return _textures; }
		Animations& GetAnimations() { return _animations; }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		void Destroy() override;

	private:
		Textures			 _textures;
		Animations			 _animations;
	};
}

inline Engine::TextureManager* TextureMgr = Engine::TextureManager::GetInstance();
inline std::function<void()> TextureTerminate = []() { TextureMgr->Terminate(); };