#include "pch.h"

#include "AnimationPlayerSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Events/EventBusLocator.hpp"
#include "Engine/Events/AnimationEvent.h"

#include "Engine/Utils/AnimationUtils.h"

namespace MyEngine
{
    void AnimationPlayerSystem::Init()
    {
    }

    void AnimationPlayerSystem::Start(Scene* pScene)
    {
    }

    void AnimationPlayerSystem::Update(Scene* pScene, float deltaTime)
    {
        for (Entity entityId : SceneView<TransformComponent, TransformAnimationComponent>(*pScene))
        {
            TransformAnimationComponent* pAnimation = pScene->Get<TransformAnimationComponent>(entityId);
            
            int startPos1 = 0;
            int endPos1 = 0;
            int startScale1 = 0;
            int endScale1 = 0;
            int startRot1 = 0;
            int endRot1 = 0;
            int startPos2 = 0;
            int endPos2 = 0;
            int startScale2 = 0;
            int endScale2 = 0;
            int startRot2 = 0;
            int endRot2 = 0;

            // Get keyframes before time increase
            AnimationUtils::GetKeyFrames<PositionKeyFrame>(pAnimation->time, pAnimation->positionKeyFrames,
								                           startPos1, endPos1);
            AnimationUtils::GetKeyFrames<ScaleKeyFrame>(pAnimation->time, pAnimation->scaleKeyFrames,
								                           startScale1, endScale1);
            AnimationUtils::GetKeyFrames<RotationKeyFrame>(pAnimation->time, pAnimation->rotationKeyFrames,
								                           startRot1, endRot1);

            pAnimation->time += deltaTime;

            // Get keyframes fater time increase
            AnimationUtils::GetKeyFrames<PositionKeyFrame>(pAnimation->time, pAnimation->positionKeyFrames,
								                           startPos2, endPos2);
            AnimationUtils::GetKeyFrames<ScaleKeyFrame>(pAnimation->time, pAnimation->scaleKeyFrames,
								                           startScale2, endScale2);
            AnimationUtils::GetKeyFrames<RotationKeyFrame>(pAnimation->time, pAnimation->rotationKeyFrames,
								                           startRot2, endRot2);

            // Check if passed frame and if is event frame
            if (endPos1 > -1 && pAnimation->positionKeyFrames[endPos1].isKeyEvent)
            {
                if (startPos1 != endPos2 && endPos1 == startPos2)
                {
                    m_TriggerPosKeyFrameEvent(startPos1, endPos1, endPos2);
                }
            }

            if (endScale1 > -1 && pAnimation->scaleKeyFrames[endScale1].isKeyEvent)
            {
                if (startScale1 != endScale2 && endScale1 == startScale2)
                {
                    m_TriggerScaleKeyFrameEvent(startScale1, endScale1, endScale2);
                }
            }

            if (endRot1 > -1 && pAnimation->rotationKeyFrames[endRot1].isKeyEvent)
            {
                if (startRot1 != endRot2 && endRot1 == startRot2)
                {
                    m_TriggerRotKeyFrameEvent(startRot1, endRot1, endRot2);
                }
            }
        }
    }

    void AnimationPlayerSystem::Render(Scene* pScene)
    {
    }

    void AnimationPlayerSystem::End(Scene* pScene)
    {
    }

    void AnimationPlayerSystem::Shutdown()
    {
    }

    void AnimationPlayerSystem::m_TriggerPosKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame)
    {
        iEventBus<eAnimationEvents, PositionKeyFrameEvent>* pEventBus = EventBusLocator<eAnimationEvents, PositionKeyFrameEvent>::Get();

        PositionKeyFrameEvent keyEvent = PositionKeyFrameEvent();
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }

    void AnimationPlayerSystem::m_TriggerRotKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame)
    {
        iEventBus<eAnimationEvents, RotationKeyFrameEvent>* pEventBus = EventBusLocator<eAnimationEvents, RotationKeyFrameEvent>::Get();

        RotationKeyFrameEvent keyEvent = RotationKeyFrameEvent();
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }

    void AnimationPlayerSystem::m_TriggerScaleKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame)
    {
        iEventBus<eAnimationEvents, ScaleKeyFrameEvent>* pEventBus = EventBusLocator<eAnimationEvents, ScaleKeyFrameEvent>::Get();

        ScaleKeyFrameEvent keyEvent = ScaleKeyFrameEvent();
        keyEvent.oldKeyFrame = oldKeyFrame;
        keyEvent.keyFrame = keyFrame;
        keyEvent.nextKeyFrame = nextKeyFrame;
        pEventBus->Publish(keyEvent);
    }
}
