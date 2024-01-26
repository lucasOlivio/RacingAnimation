#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Events/InputEvent.h"

namespace MyEngine
{
	// Handles all controls for animations
	class AnimationCommandsSystem : public iSystem
	{
	public:
		AnimationCommandsSystem() = default;
		virtual ~AnimationCommandsSystem() { };

		virtual std::string SystemName() { return "AnimationCommandsSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void End(Scene* pScene);

		virtual void Shutdown();

		static void InputTriggered(const KeyboardEvent& event);

	private:
		static void m_ToggleState();

		static void m_NextScene(int step);

		static void m_SetAnimSpeed(float speed);
	};
}
