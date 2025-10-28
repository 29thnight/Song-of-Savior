#include <Actor.h>
#include <SceneComponent.h>
#include <TextureManager.h>
#include <ActorComponent.h>
#include <ObjectPtr.h>			 
#include <UtfConvert.h>
#include <World.h>

void Engine::Actor::BeginPlay()
{
	if (_textureName.empty())
	{
		Object::CreateObjectDirectory();
		string convertName = (string)"Assets/" + this->_name.c_str();
		_vecTextures.push_back(TextureMgr->FindTexture(convertName));

		return;
	}

	string convertName = (string)"Assets/" + _textureName.c_str();
	_vecTextures.push_back(TextureMgr->FindTexture(convertName));
}

void Engine::Actor::Tick(_duration deltaSeconds)
{
	for (auto& component : _vecComponents)
	{
		component->TickComponent(deltaSeconds);
	}
}

void Engine::Actor::Fixed()
{
	
}

void Engine::Actor::EndPlay()
{
}

void Engine::Actor::SetName(_pstring name)
{
	std::string prevName{ _name };
	if (_pOwnerWorld)
	{
		auto& _actorMap = _pOwnerWorld->GetActorMap();
		auto findIter = _actorMap.find(prevName);
		if (findIter != _actorMap.end())
		{
			_actorMap.erase(findIter);		
		}
		_actorMap[std::string{name}] = this;
	}
	_name = name;
}

void Engine::Actor::SerializeIn(nlohmann::ordered_json& object)
{
	_layerIndex = object["layerIndex"];

	for (auto& component : _vecComponents)
	{
		for (auto iter = object["Components"].begin(); iter != object["Components"].end(); ++iter)
		{
			if (component->GetName() != iter.key())
				continue;
			else
			{
				component->SerializeIn(*iter);
				break;
			}	
		}
	}
}

void Engine::Actor::SerializeOut(nlohmann::ordered_json& object)
{
	std::string classType = typeid(*this).name();
	size_t namePos = classType.find("class") + 6;
	classType = classType.substr(namePos);
	object["Actor Class Type"] = classType;

	object["textureName"] = UtfConvert::wstring_to_utf8(_textureName);
	object["layerIndex"] = _layerIndex;
	for (auto& component : _vecComponents)
	{
		component->SerializeOut(object["Components"]);
	}
}

bool Engine::Actor::SetRootComponent(SceneComponent* pRootComponent)
{
	_pRootComponent = pRootComponent;

	return true;
}

void Engine::Actor::AttachToActor(Actor* pParent)
{
	_pParent = pParent;
	_pRootComponent->AttachToComponent(_pParent->GetRootComponent());
}

void Engine::Actor::DetachFromActor()
{
	_pRootComponent->AttachToComponent(nullptr);
	_pParent = nullptr;
}

void Engine::Actor::PushBackComponent(ActorComponent* pComponent)
{
	_vecComponents.push_back(MakeObjectPtr<ActorComponent>(pComponent));
}

void Engine::Actor::Destroy()
{
	_vecComponents.clear();
	_vecTextures.clear();
	_pRootComponent = nullptr;
	_pParent = nullptr;
}

Mathf::Vector2 Engine::Actor::GetBitmapSize(int index) const
{
	if (!_vecTextures.empty())
	{
		if (_vecTextures[0] == nullptr)
			return {0.f, 0.f};

		D2D1_SIZE_F size = (*_vecTextures[0])[index]->GetSize();
		return{ size.width, size.height };
	}
	return { 0.f,0.f };
}