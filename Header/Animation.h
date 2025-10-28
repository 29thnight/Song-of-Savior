#pragma once
#include <CoreBase.h>
#include <CoreDefine.h>
#include <rlottie.h>
#include <Mathf.h>

namespace Engine
{
	class Animation : public CoreBase
	{
	protected:
		explicit Animation() = default;
		virtual ~Animation() = default;

	public:
		ID2D1Bitmap* operator[](_uint index) { return _textures[index].get(); }

	public:
		bool LoadAnimation(_pwstring fileName);
		size_t size() const { return _textures.size(); }

	public: //rlottie ¹× metadata Á¤º¸
		Mathf::SizeF GetCanvasSize() const;
		size_t GetTotalFrame() const { return _totalFrame; }
		const double GetFrameRate() const { return _frameRate; }
		const double GetDuration() const { return _duration; }
		bool IsLoop() const { return _isLoop; }

	public:
		virtual void SerializeIn(nlohmann::ordered_json& object) {};
		virtual void SerializeOut(nlohmann::ordered_json& object) {};

	public:
		static Animation* Create();
		void Destroy() override;

	private:
		std::string							_path{};
		std::vector<SmartPtr<ID2D1Bitmap>>	_textures{};

	private:
		constexpr static size_t _performanceCount{ 4 };
		double _frameRate{ };
		double _duration{ };
		size_t _width{ };
		size_t _height{ };
		size_t _totalFrame{ };
		bool   _isLoop{ false };
	};
};