#include <CoreDefine.h>
#include <Texture.h>
#include <CoreManager.h>
#include <TextureManager.h>
#include <Animation.h>
#include <TimeManager.h>
#include <rlottie.h>
#include <UtfConvert.h>
#include <GraphicsManager.h>

namespace file = std::filesystem;

bool Engine::TextureManager::LoadTexture(LPCWSTR filePath)
{
    file::path rootPath(filePath);

    for (const auto& entry : file::directory_iterator(rootPath))
    {
        if (entry.is_directory())
        {
            if (entry.path().filename() == "." || entry.path().filename() == "..")
                continue;

            LoadTexture(entry.path().wstring().c_str());
        }
        else
        {
            file::path fullPath = entry.path();
            std::wstring extension = fullPath.extension().wstring();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            if (extension == L".png" || extension == L".jpg" || extension == L".jpeg" || extension == L".bmp")
            {
                std::wstring tag = fullPath.parent_path().wstring();
                tag = tag.substr(tag.find_last_of(L"/") + 1);
                std::replace(tag.begin(), tag.end(), L'\\', L'/');

                Texture* pTexture = _textures[tag].get();

                if (nullptr == pTexture)
                {
                    pTexture = Texture::Create();
                    pTexture->LoadTexture(fullPath.wstring().c_str());
                    _textures[tag] = pTexture;
                }
                else
                {
                    pTexture->LoadTexture(fullPath.wstring().c_str());
                }
            }
			else if (extension == L".aasset")
            {
                std::wstring tag = fullPath.parent_path().wstring();
                tag = tag.substr(tag.find_last_of(L"/") + 1);
                std::replace(tag.begin(), tag.end(), L'\\', L'/');

				Animation* pAnimation = _animations[tag].get();

				if (nullptr == pAnimation)
				{
					pAnimation = Animation::Create();
					pAnimation->LoadAnimation(fullPath.wstring().c_str());
					_animations[tag] = pAnimation;
				}
				else
				{
					pAnimation->LoadAnimation(fullPath.wstring().c_str());
				}
            }
        }
    }

    return true;
}

Engine::Texture* Engine::TextureManager::FindTexture(_pwstring fileTag)
{
    return _textures[fileTag].get();
}

Engine::Animation* Engine::TextureManager::FindAnimation(_pwstring fileTag)
{
	return _animations[fileTag].get();
}

void Engine::TextureManager::Destroy()
{
	_animations.clear();
	_textures.clear();
}
