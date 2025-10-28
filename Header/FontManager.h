#pragma once
#include <CoreDefine.h>
#include <Singleton.h>

namespace Engine
{
	class FontManager : public CoreBase, public Singleton<FontManager>
	{
	private:
		friend class Singleton;

	private:
		explicit FontManager() = default;
		virtual ~FontManager() = default;

	public:
		bool Initialize();
		bool LoadFonts();
		HRESULT LoadFontFile(_pwstring fontFilePath, _pwstring fontName, float fontSize);
		_pFontFormat GetFont(_pwstring fontName);
		IDWriteFactory5* GetDWriteFactory() { return _pDWriteFactory.get(); }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	private:
		void Destroy() override;

	private:
		HRESULT InitializeDWrite();
		HRESULT AddFont(_pwstring fontName, float fontSize,IDWriteFontCollection1* pFontCollection, _pFontFormat* ppTextFormat);

	private:
		SmartPtr<IDWriteFactory5>							 _pDWriteFactory{ nullptr };
		SmartPtr<IDWriteFontSetBuilder1>					 _pFontSetBuilder{ nullptr };
		std::unordered_map<std::wstring, SmartPtr<_FontFormat>> _fontMap;
	};
}

inline Engine::FontManager* FontMgr = Engine::FontManager::GetInstance();
inline std::function<void()> FontTerminate = []() { FontMgr->Terminate(); };