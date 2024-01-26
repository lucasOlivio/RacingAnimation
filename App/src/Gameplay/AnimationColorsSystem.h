#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Handles the color changing for the animations
	class AnimationColorsSystem : public iSystem
	{
	public:
		AnimationColorsSystem() = default;
		virtual ~AnimationColorsSystem() { };

		virtual std::string SystemName() { return "AnimationColorsSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void End(Scene* pScene);

		virtual void Shutdown();
	};
}
