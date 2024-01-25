#include "pch.h"

#include "AnimationSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Utils/AnimationUtils.h"

namespace MyEngine
{
    void AnimationSystem::Init()
    {
    }

    void AnimationSystem::Start(Scene* pScene)
    {
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformAnimationComponent* pAnimation = pScene->Get<TransformAnimationComponent>(entityId);

            pAnimation->time = 0.0f;
        }
    }

    void AnimationSystem::Update(Scene* pScene, float deltaTime)
    {
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
            TransformAnimationComponent* pAnimation = pScene->Get<TransformAnimationComponent>(entityId);

            float currTime = pAnimation->time;
            int startIndex = 0;
            int endIndex = 0;

            // Position
            AnimationUtils::GetKeyFrames<PositionKeyFrame>(currTime, pAnimation->positionKeyFrames,
								                           startIndex, endIndex);

            AnimationUtils::InterpolateAndApply<PositionKeyFrame, glm::vec3>(pAnimation->positionKeyFrames, 
                                                      currTime, startIndex, endIndex, pTransform->position);

            // Rotation
            AnimationUtils::GetKeyFrames<RotationKeyFrame>(currTime, pAnimation->rotationKeyFrames,
								                           startIndex, endIndex);

            AnimationUtils::InterpolateAndApply<RotationKeyFrame, glm::quat>(pAnimation->rotationKeyFrames, 
                                                      currTime, startIndex, endIndex, pTransform->orientation);

            // Scale
            AnimationUtils::GetKeyFrames<ScaleKeyFrame>(currTime, pAnimation->scaleKeyFrames,
								                           startIndex, endIndex);

            AnimationUtils::InterpolateAndApply<ScaleKeyFrame, float>(pAnimation->scaleKeyFrames, 
                                                      currTime, startIndex, endIndex, pTransform->scale);
        }
    }

    void AnimationSystem::Render(Scene* pScene)
    {
    }

    void AnimationSystem::End(Scene* pScene)
    {
    }

    void AnimationSystem::Shutdown()
    {
    }
}
