#include "pch.h"

#include "StateSystem.h"

#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Events/GameStateEvent.h"
#include "Engine/Events/EventBusLocator.hpp"

namespace MyEngine
{
	void StateSystem::Init()
	{
	}

	void StateSystem::Start(Scene* pScene)
	{
	}

	void StateSystem::Update(Scene* pScene, float deltaTime)
	{
		GameStateComponent* pState = CoreLocator::GetGameState();
		if (pState->currState == pState->prevState)
		{
			return;
		}

		m_TriggerStateChange(pScene, pState->prevState, pState->currState);
		pState->prevState = pState->currState;
	}

	void StateSystem::Render(Scene* pScene)
	{
	}

	void StateSystem::End(Scene* pScene)
	{
	}

	void StateSystem::Shutdown()
	{
	}

	void StateSystem::m_TriggerStateChange(Scene* pScene, const eGameStates& oldState, const eGameStates& newState)
	{
		if (newState == eGameStates::STARTED)
		{
			iEventBus<eGameStateEvents, GameStartedEvent>* pEventBus = EventBusLocator<eGameStateEvents, GameStartedEvent>::Get();

			GameStartedEvent stateEvent = GameStartedEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			pEventBus->Publish(stateEvent);

			return;
		}
		else if (newState == eGameStates::STOPPED)
		{
			iEventBus<eGameStateEvents, GameStoppedEvent>* pEventBus = EventBusLocator<eGameStateEvents, GameStoppedEvent>::Get();

			GameStoppedEvent stateEvent = GameStoppedEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			pEventBus->Publish(stateEvent);

			return;
		}
		else if (newState == eGameStates::RUNNING)
		{
			iEventBus<eGameStateEvents, GameRunningEvent>* pEventBus = EventBusLocator<eGameStateEvents, GameRunningEvent>::Get();

			GameRunningEvent stateEvent = GameRunningEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			pEventBus->Publish(stateEvent);

			return;
		}
		else if (newState == eGameStates::GAMEOVER)
		{
			iEventBus<eGameStateEvents, GameOverEvent>* pEventBus = EventBusLocator<eGameStateEvents, GameOverEvent>::Get();

			GameOverEvent stateEvent = GameOverEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			pEventBus->Publish(stateEvent);

			return;
		}
	}
}
