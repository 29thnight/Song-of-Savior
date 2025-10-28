#include <Animation.h>
#include <CoreManager.h>
#include "../ThirdParty/nlohmann/json.hpp"

bool Engine::Animation::LoadAnimation(_pwstring fileName)
{
    HRESULT hresult{ S_FALSE };
    string  convert = fileName;

    _path = convert;

	std::string metaFilePath = _path.substr(0, _path.find_last_of('.')) + ".metadata";
    std::ifstream inputFile(metaFilePath);
	if (inputFile.is_open())
	{
		nlohmann::json json;
		inputFile >> json;
		inputFile.close();

		if (json.contains("loop"))
		{
			_isLoop = json["loop"].get<bool>();
		}
	}

    std::shared_ptr<rlottie::Animation>	_animation = rlottie::Animation::loadFromFile(_path);
    if (!_animation) return false;

    _animation->size(_width, _height);
	_duration = _animation->duration();
    _totalFrame = _animation->totalFrame();
    _frameRate = _performanceCount / _animation->frameRate();

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t frameNumber = 0; frameNumber < _totalFrame; frameNumber += _performanceCount) 
    {
        std::vector<uint32_t> frameBuffer(_width * _height);
        rlottie::Surface surface(frameBuffer.data(), _width, _height, _width * 4);
        _animation->renderSync(frameNumber, surface);

        SmartPtr<ID2D1Bitmap> pBitmap{ nullptr };

		D2D1_BITMAP_PROPERTIES bitmapProperties{ };
        bitmapProperties.pixelFormat = Management->renderTarget->GetPixelFormat();
        bitmapProperties.dpiX = bitmapProperties.dpiY = 96.0f;

        Management->renderTarget->CreateBitmap(
            D2D1::SizeU((UINT32)_width, (UINT32)_height),
            frameBuffer.data(),
            (UINT32)_width * 4,
            &bitmapProperties,
            &pBitmap
        );
		//DebugPrintf("Frame %llu loaded\n", static_cast<unsigned long long>(frameNumber));
        _textures.push_back(pBitmap);
    }
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	DebugPrintf("Animation Load Time: %fs\n", duration.count());

    return true;
}

Mathf::SizeF Engine::Animation::GetCanvasSize() const
{
	return Mathf::SizeF{ (float)_width, (float)_height };
}

Engine::Animation* Engine::Animation::Create()
{
	return new Animation;
}

void Engine::Animation::Destroy()
{
}
