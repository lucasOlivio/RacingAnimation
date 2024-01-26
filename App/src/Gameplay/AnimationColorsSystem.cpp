#include "pch.h"

#include "AnimationColorsSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

namespace MyEngine
{
	void AnimationColorsSystem::Init()
	{
	}

	void AnimationColorsSystem::Start(Scene* pScene)
	{
	}

	void AnimationColorsSystem::Update(Scene* pScene, float deltaTime)
	{
		for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent, ModelComponent>(*pScene))
		{
			TransformAnimationComponent* pAnimation = pScene->Get<TransformAnimationComponent>(entityId);
			ModelComponent* pModel = pScene->Get<ModelComponent>(entityId);

			if (pAnimation->currStartPosKF < 0)
			{
				continue;
			}

			const PositionKeyFrame& currPosKF = pAnimation->positionKeyFrames[pAnimation->currStartPosKF];
			if (currPosKF.easeType == eEasingType::Linear)
			{
				pModel->defaultColor = WHITE;
			}
			else if (currPosKF.easeType == eEasingType::SineEaseIn)
			{
				pModel->defaultColor = RED;
			}
			else if (currPosKF.easeType == eEasingType::SineEaseOut)
			{
				pModel->defaultColor = YELLOW;
			}
			else if (currPosKF.easeType == eEasingType::SineEaseInOut)
			{
				pModel->defaultColor = GREEN;
			}
		}
	}

	void AnimationColorsSystem::Render(Scene* pScene)
	{
	}

	void AnimationColorsSystem::End(Scene* pScene)
	{
	}

	void AnimationColorsSystem::Shutdown()
	{
	}
}
