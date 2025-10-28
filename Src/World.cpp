#include <Actor.h>
#include <CameraActor.h>
#include <GraphicsManager.h>
#include <CameraComponent.h>
#include <SceneComponent.h>
#include <CoreManager.h>
#include <Object.h>
#include <Level.h>
#include <Layer.h>
#include <World.h>
#include <Mathf.h>
#include <ACollision.h>
#include <JsonLevel.h>

bool Engine::World::InitializeWorld(int layerSize)
{
	bool result{};
    result = InitializeLayer(layerSize);
	if(!result)
	{
		return result;
	}

	return result;
}

bool Engine::World::BeginPlayEnd()
{
	for (auto iter = _vecLevels.begin(); iter != _vecLevels.end(); iter++)
	{
		(*iter)->SettingLevelCamera(_pCameraActor);
	}

	for (auto iter = _vecLayers.begin(); iter != _vecLayers.end(); iter++)
	{
		(*iter)->SettingLayerCamera(_pCameraActor);
	}

	return true;
}

void Engine::World::Destroy()
{
	SafeDelete(_pCameraActor);
	ClearLayer();
	ClearLevel();
}

void Engine::World::Tick(_duration deltaSeconds)
{
	if(_pCameraActor)
	{
		_pCameraActor->Tick(deltaSeconds);
	}

	_vecLevels[_currentLevelIndex]->Tick(deltaSeconds);
}

void Engine::World::Fixed()
{
	_vecLevels[_currentLevelIndex]->Fixed();
}

void Engine::World::Render(_RenderTarget pRenderTarget)
{
	for(auto iter = _vecLayers.begin(); iter != _vecLayers.end(); iter++)
	{
		//if (IsLevelEndPlay())
		//{
		//	return;
		//}

		(*iter)->Render(pRenderTarget);
	}
}

void Engine::World::EndPlay()
{
	_vecLevels[_currentLevelIndex]->EndPlay();
}

void Engine::World::DestroyPoint()
{
	if (_previousLevelIndex == _currentLevelIndex)
	{
		_vecLevels[_currentLevelIndex]->DestroyPoint();
	}
	else
	{
		_vecLevels[_previousLevelIndex]->DestroyPoint();
		_previousLevelIndex = _currentLevelIndex;
	}

}

Engine::Level* Engine::World::GetLevel(int levelIndex) const
{
	if (levelIndex < _vecLevels.size())
	{
		return _vecLevels[levelIndex];
	}

	return nullptr;
}

void Engine::World::InnerContainLevel(Level* pLevel)
{
	pLevel->SetWorld(this);
	_vecLevels.push_back(pLevel);
}

void Engine::World::CurrentLevelBeginPlay()
{
	_vecLevels[_currentLevelIndex]->BeginPlay();
}

void Engine::World::ChangeLevel(int levelIndex)
{
	_vecLevels[_currentLevelIndex]->SetEndPlay(true);
	_vecLevels[_currentLevelIndex]->EndPlay();
	_vecLevels[_currentLevelIndex]->DestroyPoint();

	_previousLevelIndex = _currentLevelIndex;
	_currentLevelIndex = levelIndex;

	_vecLevels[levelIndex]->SetBeginPlay(false);
	_vecLevels[levelIndex]->BeginPlay();

}

void Engine::World::RemoveLevel(int levelIndex)
{
	for (auto iter = _vecLevels.begin(); iter != _vecLevels.end(); iter++)
	{
		if (*iter == _vecLevels[levelIndex])
		{
			SafeDelete(*iter);
			_vecLevels.erase(iter);
			break;
		}
	}
}

void Engine::World::ClearLevel()
{
	for (auto iter = _vecLevels.begin(); iter != _vecLevels.end(); iter++)
	{
		SafeDelete(*iter);
	}

	_vecLevels.clear();
}

void Engine::World::SaveLevelToJson(int levelIndex, const wchar_t* _savePath)
{
	using namespace nlohmann;

	if (levelIndex < _vecLevels.size())
	{
		std::wstring savePath(_savePath);
		size_t findPos = savePath.rfind(L".json");
		if (findPos == std::wstring::npos)
		{
			savePath += L".json";
		}

		ordered_json levelJsonObj;
		GetLevel(levelIndex)->SerializeOut(levelJsonObj);

		if (!levelJsonObj.is_null())
		{
			std::ofstream ofs(savePath);
			ofs << levelJsonObj.dump(2);
			ofs.close();
		}	
	}
}

void Engine::World::LoadLevelToJson(const wchar_t* _loadPath, std::vector<Actor*>* actorListOutput)
{
	using namespace nlohmann;
	wchar_t path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	std::wstring loadPath(path);
	loadPath += L"\\Assets\\";
	loadPath += _loadPath;

	std::ifstream ifs(loadPath, std::ios::in);
	if (!ifs.is_open())
	{
		MessageBox(nullptr, L"jsonLoad, File not found", L"Error", MB_OK);
		return;
	}
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ordered_json object = ordered_json::parse(str);
	ifs.close();
	
	try
	{
		if(actorListOutput == nullptr)
			_vecLevels[_currentLevelIndex]->SerializeIn(object);
		else
			_vecLevels[_currentLevelIndex]->SerializeIn(object, actorListOutput);
	}
	catch (const std::exception& ex)
	{
		DebugPrintf("LoadLevelToJson Error, %s\n", ex.what());
		assert(!"Load Level To Json Error");
	}

}
bool Engine::World::IsLevelEndPlay() const
{
	return _vecLevels[_currentLevelIndex]->IsEndPlay();
}
bool Engine::World::InitializeLayer(int layerSize)
{
    _layerSize = layerSize;
	for (int i = 0; i < _layerSize; i++)
	{
		Layer* pLayer = Layer::InitializeLayer(i);
		_vecLayers.push_back(pLayer);
	}

	return true;
}

Engine::Layer* Engine::World::GetLayer(int layerIndex) const
{
	if (layerIndex < _vecLayers.size())
	{
		return _vecLayers[layerIndex];
	}

	return nullptr;
}

void Engine::World::AddLayer(int layerIndex, Layer* pLayer)
{
	_vecLayers.push_back(pLayer);
}

void Engine::World::RemoveLayer(int layerIndex)
{
	for(auto iter = _vecLayers.begin(); iter != _vecLayers.end(); iter++)
	{
		if (*iter == _vecLayers[layerIndex])
		{
			SafeDelete(*iter);
			_vecLayers.erase(iter);
			break;
		}
	}
}

void Engine::World::ClearLayer()
{
	for(auto iter = _vecLayers.begin(); iter != _vecLayers.end(); iter++)
	{
		SafeDelete(*iter);
	}

	_vecLayers.clear();
}

void Engine::World::SettingCamera(CameraActor* pCameraActor)
{
	if(!pCameraActor)
	{
		_pCameraActor = CameraActor::Create();
		_pCameraActor->SetWorld(this);
		_pCameraActor->SetName("MainCamera");
		return;
	}

	_pCameraActor = pCameraActor;
	_pCameraActor->SetWorld(this);
}

void Engine::World::SettingCameraPosition(Mathf::Vector2 position)
{
	_pCameraActor->GetRootComponent()->SetRelativeLocation(position);
}

void Engine::World::SettingTrackingCameraTarget(Actor* pTargetActor)
{
	_pCameraActor->TrackTarget(pTargetActor);
	_pCameraActor->SettingTracking(true);
}

void Engine::World::SetCameraLerpFactor(float lerpFactor)
{
	_pCameraActor->SetCameraLerpFactor(lerpFactor);
}

void Engine::World::SettingTrackingCamera(bool isTracking)
{
	_pCameraActor->SettingTracking(isTracking);
}

bool Engine::World::IsTrackingCamera() const
{
	return _pCameraActor->IsTracking();
}

void Engine::World::SettingCameraOffset(Mathf::Vector2 offset)
{
	_pCameraActor->GetCameraComponent()->SetRelativeLocation(offset);
}

Engine::Actor* Engine::World::FindActor(_pstring name)
{
	auto findIter = _actorMap.find(name);
	if (findIter != _actorMap.end())
		return findIter->second;
	else
		return nullptr;
}

bool Engine::World::SpawnActor(int layerIndex, _pstring name, Actor* pActor)
{
	pActor->SetName(name);
	pActor->SetRootComponent(pActor->AddComponent<SceneComponent>("RootComponent"));
	pActor->SetLayerIndex(layerIndex);
	pActor->SetWorld(this);

	_actorMap[name] = pActor;
	_vecLevels[_currentLevelIndex]->AddActor(pActor);

	return true;
}

bool Engine::World::SpawnActor(int layerIndex, _pstring name, Actor* pActor, Mathf::Vector2 location)
{
	pActor->SetName(name);
	pActor->SetRootComponent(pActor->AddComponent<SceneComponent>("RootComponent"));
	pActor->SetLayerIndex(layerIndex);
	pActor->SetWorld(this);
	pActor->GetRootComponent()->SetRelativeLocation(location);

	_actorMap[name] = pActor;
	_vecLevels[_currentLevelIndex]->AddActor(pActor);

	return true;
}

bool Engine::World::RemoveActor(_pstring name)
{
	_actorMap.find(name)->second->OnDestroyMark();

	return true;
}

bool Engine::World::TerminateActor(_pstring name)
{
	auto findIter = _actorMap.find(name);
	if (findIter == _actorMap.end())
	{
		return false;
	}
	else
	{
		Actor* pActor = (*findIter).second;
		pActor->OnCompleteDestroyMark();
		Management->ScheduleDestroyPoint();
		return true;
	}
}

bool Engine::World::ReviveActor(_pstring name)
{
	Actor* pActor = static_cast<Actor*>(CoreManager::GetInstance()->FindDestroyList(name));
	if (pActor)
	{
		pActor->OnRevive();
		_actorMap.insert(std::make_pair(name, pActor));
		_vecLevels[_currentLevelIndex]->InsertActor(pActor);
		pActor->ReviveInitialize();
		pActor->SetWorld(this);
		Management->UnRegisterDestroyList(pActor);

		return true;
	}

	return false;
}

void Engine::World::DebugDrawGrid(_RenderTarget pRenderTarget)
{
	Mathf::SizeF size = pRenderTarget->GetSize();

	for (float i = 0; i < size.width; i += 100.f)
	{
		pRenderTarget->DrawLine(Mathf::Point2F{ i, 0.f }, Mathf::Point2F{ i, (float)size.height }, Graphics->GetBrush("Debug"));
	}

	for (float i = 0; i < size.height; i += 100.f)
	{
		pRenderTarget->DrawLine(Mathf::Point2F{ 0.f, i }, Mathf::Point2F{ (float)size.width, i }, Graphics->GetBrush("Debug"));
	}
}
