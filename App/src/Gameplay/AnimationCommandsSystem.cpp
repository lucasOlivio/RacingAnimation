#include "pch.h"

#include "AnimationCommandsSystem.h"

#include "Core/Constants.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"
#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/ECS/Scene/SceneManagerLocator.h"
#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Events/InputEvent.h"
#include "Engine/Events/EventBusLocator.hpp"

namespace MyEngine
{
	void AnimationCommandsSystem::Init()
	{
		// Subscribe to keyboard event
		iEventBus<eInputEvents, KeyboardEvent>* pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

		pEventBus->Subscribe(eInputEvents::KEYBOARD, InputTriggered);
	}

	void AnimationCommandsSystem::Start(Scene* pScene)
	{
	}

	void AnimationCommandsSystem::Update(Scene* pScene, float deltaTime)
	{
	}

	void AnimationCommandsSystem::Render(Scene* pScene)
	{
	}

	void AnimationCommandsSystem::End(Scene* pScene)
	{
	}

	void AnimationCommandsSystem::Shutdown()
	{
		// Subscribe to keyboard event
		iEventBus<eInputEvents, KeyboardEvent>* pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

		pEventBus->Unsubscribe(eInputEvents::KEYBOARD, InputTriggered);
	}

	void AnimationCommandsSystem::InputTriggered(const KeyboardEvent& event)
	{
		if (event.keyData.action != eInputActions::KEY_PRESS)
		{
			return;
		}

		if (event.keyData.key == eKeyCodes::SPACE)
		{
			m_ToggleState();
			return;
		}

		if (event.keyData.key == eKeyCodes::RIGHT_ARROW)
		{
			m_NextScene(1);
			return;
		}

		if (event.keyData.key == eKeyCodes::LEFT_ARROW)
		{
			m_NextScene(-1);
			return;
		}

		if (event.keyData.key == eKeyCodes::DIGIT_1)
		{
			m_SetAnimSpeed(1.0f);
			return;
		}

		if (event.keyData.key == eKeyCodes::DIGIT_2)
		{
			m_SetAnimSpeed(2.0f);
			return;
		}

		if (event.keyData.key == eKeyCodes::DIGIT_3)
		{
			m_SetAnimSpeed(3.0f);
			return;
		}

		if (event.keyData.key == eKeyCodes::DIGIT_4)
		{
			m_SetAnimSpeed(4.0f);
			return;
		}

		if (event.keyData.key == eKeyCodes::DIGIT_5)
		{
			m_SetAnimSpeed(5.0f);
			return;
		}

		if (event.keyData.key == eKeyCodes::R)
		{
			m_SetAnimSpeed(-1.0f);
			return;
		}
	}

	void AnimationCommandsSystem::m_ToggleState()
	{
		GameStateComponent* pState = CoreLocator::GetGameState();

		if (pState->currState == eGameStates::STOPPED || pState->currState == eGameStates::STARTED)
		{
			pState->currState = eGameStates::RUNNING;
		}
		else if (pState->currState == eGameStates::RUNNING)
		{
			pState->currState = eGameStates::STOPPED;
		}
	}

	void AnimationCommandsSystem::m_NextScene(int step)
	{
		iSceneManager* pSceneManager = SceneManagerLocator::Get();
		std::string currScene = pSceneManager->GetCurrentSceneName();

		int sceneIndex = 0;
		for (; sceneIndex < SCENES_LIST.size(); sceneIndex++)
		{
			if (currScene == SCENES_LIST[sceneIndex])
			{
				break;
			}
		}

		sceneIndex += step;
		if (sceneIndex < 0)
		{
			sceneIndex = 0;
		}
		else if(sceneIndex >= SCENES_LIST.size())
		{
			sceneIndex = SCENES_LIST.size() - 1;
		}

		currScene = SCENES_LIST[sceneIndex];

		pSceneManager->ChangeScene(currScene, true);
	}

	void AnimationCommandsSystem::m_SetAnimSpeed(float speed)
	{
		AnimationControllerComponent* pAnimController = GraphicsLocator::GetAnimationController();

		pAnimController->speed = speed;

		if (speed < 0)
		{
			pAnimController->reset = true;
		}
	}
}
