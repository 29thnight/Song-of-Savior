#include <Layer.h>
#include <CoreManager.h>
#include <World.h>
#include <CameraActor.h>
#include <CameraComponent.h>
#include <RenderComponent.h>

void Engine::Layer::Render(_RenderTarget pRenderTarget)
{
	if(_renderQueue.empty())
	return;

	World* pWorld = Management->setting.pWorld;

	if(pWorld->IsCustomRenderSort())
	{
		pWorld->CustomRenderSort();
	}
	else
	{
		if(pWorld->IsLevelEndPlay())
		{
			return;
		}

		if(pWorld->IsSortGreater())
		{
			std::sort(_renderQueue.begin(), _renderQueue.end(),
				[](RenderComponent* lhs, RenderComponent* rhs)
				{
					return (*lhs) > (*rhs);
				});
		}
		else
		{
			std::sort(_renderQueue.begin(), _renderQueue.end(),
				[](RenderComponent* lhs, RenderComponent* rhs)
				{
					return (*lhs) < (*rhs);
				});
		}

	}

	if(_layerIndex == LAYER::UI)
	{
		const Mathf::Matx3F& matrixCamera = Matx::Identity;
		pRenderTarget->SetTransform(Matx::Identity);
	
		for (auto& renderComponent : _renderQueue)
		{
			if (renderComponent->GetOwner()->IsDestroyMarked())
			{
				continue;
			}

			renderComponent->SetCameraMatrix(matrixCamera);
			renderComponent->Render(pRenderTarget);
		}
	}
	else
	{
		const Mathf::Matx3F& matrixCamera = _pCameraActor->GetCameraComponent()->GetWorldTransform();

		for (auto& renderComponent : _renderQueue)
		{
			if (renderComponent->GetOwner()->IsDestroyMarked())
			{
				continue;
			}

			renderComponent->SetCameraMatrix(matrixCamera);
			renderComponent->Render(pRenderTarget);
		}
	}
}

void Engine::Layer::AddRenderQueue(RenderComponent* pRenderComponent)
{
	if (!pRenderComponent)
	return;

	_renderQueue.push_back(pRenderComponent);
}

void Engine::Layer::RemoveRenderQueue(RenderComponent* pRenderComponent)
{
	if (!pRenderComponent)
	return;

	_renderQueue.erase(
		std::find(_renderQueue.begin(), _renderQueue.end(), pRenderComponent));
	_renderQueue.shrink_to_fit();
}

Engine::Layer* Engine::Layer::InitializeLayer(int index)
{
	Layer* pLayer = new Layer;
	pLayer->_layerIndex = index;

    return pLayer;
}

void Engine::Layer::Destroy()
{
}
