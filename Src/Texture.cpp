#include <CoreManager.h>
#include <Texture.h>

ID2D1Bitmap* Engine::Texture::LoadTexture(_pwstring fileName)
{
	SmartPtr<ID2D1Bitmap>			pBitmap{ nullptr };
	SmartPtr<IWICBitmapDecoder>		pDecoder{ nullptr };
	SmartPtr<IWICFormatConverter>	pConverter{ nullptr };
	SmartPtr<IWICBitmapFrameDecode>	pFrame{ nullptr };
	HRESULT hresult{ S_FALSE };
	
	hresult = Management->WICFactory->CreateDecoderFromFilename(
		fileName,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if(SUCCEEDED(hresult))
	{
		hresult = pDecoder->GetFrame(0, &pFrame);
	}

	if(SUCCEEDED(hresult))
	{
		hresult = Management->WICFactory->CreateFormatConverter(&pConverter);
	}

	if(SUCCEEDED(hresult))
	{
		hresult = pConverter->Initialize(
			pFrame.get(), GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
	}

	if(SUCCEEDED(hresult))
	{
		hresult = Management->renderTarget->CreateBitmapFromWicBitmap(
			pConverter.get(), nullptr, &pBitmap);
	}

	if(SUCCEEDED(hresult))
	{
		_textures.push_back(pBitmap);
		return pBitmap.get();
	}

	return nullptr;
}

void Engine::Texture::Destroy()
{
	_textures.clear();
}

Engine::Texture* Engine::Texture::Create()
{
	return new Texture;
}
