#include <BitmapComponent.h>
#include <BoxComponent.h>
#include <AssetManager.h>
#include <UI.h>

bool RegUI = RegisterActor<Client::UI>();

void Client::UI::BeginPlay()
{
	SetLayerIndex(LAYER::UI);
	Super::BeginPlay();
	//Engine::AssetManager::GetAssetRegister();
	_pPrimaryBitmap = AddComponent<Engine::BitmapComponent>("PrimaryBitmap");
	_pPrimaryBitmap->SetTextures(&_vecTextures);

	_pSecondaryBitmap = AddComponent<Engine::BitmapComponent>("SecondaryBitmap");
	_pSecondaryBitmap->SetTextures(&_vecTextures);
	_pSecondaryBitmap->SetVisible(false);

	_pSecondaryBitmap->SetOpacity(0.f);
}

void Client::UI::Tick(_duration deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (_isFocus && _pSecondaryBitmap->GetOpacity() == 1.f && !_pSecondaryBitmap->Visible)
	{
		_pSecondaryBitmap->SetVisible(true);
		_pPrimaryBitmap->SetVisible(false);
	}
	else if(!_isFocus && !_pPrimaryBitmap->Visible)
	{
		_pPrimaryBitmap->SetVisible(true);
		_pSecondaryBitmap->SetVisible(false);
	}

	//DebugPrintf("%f, %f\n", GetRootComponent()->GetRelativeLocation().x, GetRootComponent()->GetRelativeLocation().y);
}

void Client::UI::Fixed()
{
}

void Client::UI::EndPlay()
{
}

void Client::UI::Destroy()
{
	Super::Destroy();
}

void Client::UI::ReloadTextures()
{
	if (_pPrimaryBitmap || _pSecondaryBitmap)
	{
		_vecTextures.clear();
		Super::BeginPlay();
	}
}

void Client::UI::UnbindEnterEvent()
{
	_onEnterEvent.Unbind();
}

Client::UI& Client::UI::operator=(Client::UI&& rhs) noexcept
{
	_pRootComponent = rhs._pRootComponent;
	_textureName = rhs._textureName;

	for (int i = 0; i < _vecComponents.size(); ++i)
	{
		_vecComponents[i] = std::move(rhs._vecComponents[i]);
		_vecComponents[i]->SetOwner(this);
	}
	_pPrimaryBitmap = rhs._pPrimaryBitmap;
	_pPrimaryBitmap->SetTextures(&_vecTextures);	
	_pPrimaryBitmap->AddRenderQueueInLayer();

	_pSecondaryBitmap = rhs._pSecondaryBitmap;
	_pSecondaryBitmap->SetTextures(&_vecTextures);
	_pSecondaryBitmap->AddRenderQueueInLayer();

	return *this;
}

void Client::UI::CallEnterEvent()
{
	_onEnterEvent.Broadcast();
}