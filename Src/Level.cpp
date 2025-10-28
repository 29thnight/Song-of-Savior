#include <Level.h>
#include <Actor.h>
#include <Object.h>
#include <World.h>
#include <CoreManager.h>
#include <SceneComponent.h>
#include <CameraActor.h>
#include <Mathf.h>
#include <AssetManager.h>
#include <UtfConvert.h>

bool Engine::Level::BeginPlay()
{
	if (_actors.empty())
		return false;

	SetBeginPlay(true);

	return true;
}

void Engine::Level::Tick(_duration deltaSeconds)
{
	if(_actors.empty())
		return;

	for(auto& actor : _actors)
	{
		if (_actors.empty() || 0 == _actors.size())
			return;

		if(actor->IsDestroyMarked())
			continue;

		actor->Tick(deltaSeconds);
	}
}

void Engine::Level::Fixed()
{
	if(_actors.empty())
		return;

	for(auto& actor : _actors)
	{

		if(actor->IsDestroyMarked())
			continue;

		actor->Fixed();
	}
}

void Engine::Level::EndPlay()
{
}

void Engine::Level::DestroyPoint()
{
	if (_actors.empty())
	return;

	for (auto it = _actors.begin(); it != _actors.end();)
	{
		try
		{
			if (*it == nullptr)
			{
				it = _actors.erase(it);
				continue;
			}

			if (!(*it)->IsDestroyMarked())
			{
				++it;
				continue;
			}

			(*it)->EndPlay();
			Management->AddDestroyList(*it);
			Management->EraseActorMap(*it);
			it = _actors.erase(it); // 요소를 삭제하고 반복자를 다음 요소로 이동
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error while processing actor: " << e.what() << std::endl;
			it = _actors.erase(it); // 예외가 발생한 경우에도 해당 요소를 제거하고 반복자를 다음으로 이동
		}
	}
}

void Engine::Level::SerializeIn(nlohmann::ordered_json& object)
{
	using namespace nlohmann;

	for (auto iter = object.begin(); iter != object.end(); ++iter)
	{
		std::string ActorName = iter.key();	
		Actor* newActor = AssetManager::InitializeActor((*iter)["Actor Class Type"]);
		newActor->SetTextureName(UtfConvert::utf8_to_wstring((*iter)["textureName"]).c_str());

		SpawnActor((*iter)["layerIndex"], ActorName.c_str(), newActor);
		newActor->SerializeIn(*iter);
	}
}

void Engine::Level::SerializeIn(nlohmann::ordered_json& object, std::vector<Actor*>* actorListOutput)
{
	using namespace nlohmann;

	bool isOuput = actorListOutput != nullptr;
	for (auto iter = object.begin(); iter != object.end(); ++iter)
	{
		std::string ActorName = iter.key();
		Actor* newActor = AssetManager::InitializeActor((*iter)["Actor Class Type"]);
		newActor->SetTextureName(UtfConvert::utf8_to_wstring((*iter)["textureName"]).c_str());

		SpawnActor((*iter)["layerIndex"], ActorName.c_str(), newActor);
		newActor->SerializeIn(*iter);
		if (isOuput)
			actorListOutput->push_back(newActor);
	}
}

void Engine::Level::SerializeOut(nlohmann::ordered_json& object)
{
	using namespace nlohmann;

	for (auto& actor : _actors)
	{
		const char* name = actor->GetName();
		ordered_json actorJsonObj;
		actor->SerializeOut(actorJsonObj);

		if(!actorJsonObj.is_null())
			object[name] = actorJsonObj;
	}
}

bool Engine::Level::InsertActor(Actor* pActor)
{
	_actors.push_back(pActor);

    return true;
}

bool Engine::Level::AddActor(Actor* pActor)
{
    _actors.push_back(pActor);
	pActor->BeginPlay();

    return true;
}

Engine::Actor* Engine::Level::GetActor(_pstring name)
{
    for (auto iter = _actors.begin(); iter != _actors.end(); iter++)
	{
		if (!strcmp((*iter)->GetName(), name))
			return *iter;
	}

	return nullptr;
}

bool Engine::Level::spawnActor(int layerIndex, _pstring name, Actor* pActor)
{
	pActor->SetName(name);
	pActor->SetRootComponent(pActor->AddComponent<SceneComponent>("RootComponent"));
	pActor->SetLayerIndex(layerIndex);
	pActor->SetWorld(_pWorld);

	_pWorld->GetActorMap()[name] = pActor;
	AddActor(pActor);

	return true;	
}

bool Engine::Level::spawnActor(int layerIndex, _pstring name, Actor* pActor, Mathf::Vector2 location)
{
	pActor->SetName(name);
	pActor->SetRootComponent(pActor->AddComponent<SceneComponent>("RootComponent"));
	pActor->SetLayerIndex(layerIndex);
	pActor->SetWorld(_pWorld);
	pActor->GetRootComponent()->SetRelativeLocation(location);

	_pWorld->GetActorMap()[name] = pActor;
	AddActor(pActor);

	return true;
}

bool Engine::Level::RemoveActor(_pstring name)
{
	_pWorld->GetActorMap().find(name)->second->OnDestroyMark();

	return true;
}

bool Engine::Level::TerminateActor(_pstring name)
{
	auto iter = _pWorld->GetActorMap().find(name);
	if (iter == _pWorld->GetActorMap().end())
	{
		return false;
	}

	Actor* pActor = iter->second;
	pActor->OnCompleteDestroyMark();

	return true;
}

bool Engine::Level::ReviveActor(_pstring name)
{
	Actor* pActor = dynamic_cast<Actor*>(Management->FindDestroyList(name));
	if (pActor)
	{
		pActor->OnRevive();
		_pWorld->GetActorMap()[name] = pActor;/*.insert(std::make_pair(name, pActor));*/
		InsertActor(pActor);
		pActor->ReviveInitialize();
		pActor->SetWorld(_pWorld);
		Management->UnRegisterDestroyList(pActor);

		return true;
	}

	return false;
}


void Engine::Level::Destroy()
{
	for (auto iter = _actors.begin(); iter != _actors.end(); iter++)
	{
		SafeDelete(*iter);
	}

	_actors.clear();
}
