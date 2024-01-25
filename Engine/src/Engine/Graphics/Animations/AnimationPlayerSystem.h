#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Handles the time for all animations and key frame events triggering, 
	// decoupled from animation system to allow more options on how to run the animations
	class AnimationPlayerSystem : public iSystem
	{
	public:
		AnimationPlayerSystem() = default;
		virtual ~AnimationPlayerSystem() { };

		virtual std::string SystemName() { return "AnimationPlayerSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void End(Scene* pScene);

		virtual void Shutdown();

	private:
		void m_TriggerPosKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame);
		void m_TriggerRotKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame);
		void m_TriggerScaleKeyFrameEvent(int oldKeyFrame, int keyFrame, int nextKeyFrame);
	};
}
