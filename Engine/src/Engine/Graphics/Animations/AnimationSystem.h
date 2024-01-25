#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Handles all the transform movements based on the time and the key frames for the animation
	class AnimationSystem : public iSystem
	{
	public:
		AnimationSystem() = default;
		virtual ~AnimationSystem() { };

		virtual std::string SystemName() { return "AnimationSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void End(Scene* pScene);

		virtual void Shutdown();
	};
}
