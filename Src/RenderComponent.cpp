#include <RenderComponent.h>
#include <Actor.h>
#include <CoreManager.h>
#include <World.h>
#include <Layer.h>
#include <Mathf.h>
#include <Actor.h>

void Engine::RenderComponent::AddRenderQueueInLayer(int layer)
{
	Management->AddRenderQueue(layer, this);
}

void Engine::RenderComponent::AddRenderQueueInLayer()
{
	int index = _owner->GetLayerIndex();
	Management->AddRenderQueue(index, this);
}

void Engine::RenderComponent::RemoveRenderQueueInLayer()
{
	int index = _owner->GetLayerIndex();
	Management->RemoveRenderQueue(index, this);
}

void Engine::RenderComponent::SetCameraMatrix(const Mathf::Matx3F& cameraMatrix)
{
	_cameraMatrix = cameraMatrix;
	D2D1InvertMatrix(&_cameraMatrix);
}
