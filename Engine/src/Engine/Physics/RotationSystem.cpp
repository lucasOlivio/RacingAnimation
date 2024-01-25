#include "pch.h"

#include "RotationSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void RotationSystem::Init()
    {
    }

    void RotationSystem::Start(Scene* pScene)
    {
    }

    void RotationSystem::Update(Scene* pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : SceneView<TransformComponent, RotationComponent>(*pScene))
        {
            TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
            RotationComponent* pRotation = pScene->Get<RotationComponent>(entityId);

            glm::vec3 newVelocity = pRotation->velocity + (pRotation->acceleration * deltaTime);
            glm::vec3 dragForce = newVelocity * -(pRotation->drag * deltaTime);
            pRotation->velocity = newVelocity + dragForce;

            // Clip velocity between min and max
            if (pRotation->velocity.length() <= 0.5f || pRotation->maxSpeed == 0.0f)
            {
                pRotation->velocity = glm::vec3(0.0f);
            }
            else if (pRotation->velocity.length() > pRotation->maxSpeed)
            {
                pRotation->velocity = glm::normalize(pRotation->velocity) * pRotation->maxSpeed;
            }

            glm::vec3 deltaRotation = pRotation->velocity * deltaTime;

            pTransform->orientation = TransformUtils::AdjustOrientation(pTransform->orientation, deltaRotation);
        }
    }

    void RotationSystem::Render(Scene* pScene)
    {
    }

    void RotationSystem::End(Scene* pScene)
    {
    }

    void RotationSystem::Shutdown()
    {
    }
}
