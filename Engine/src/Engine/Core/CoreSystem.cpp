#include "pch.h"

#include "CoreSystem.h"

#include "Engine/Core/EngineLocator.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Events/EventBusLocator.hpp"

namespace MyEngine
{
    void CoreSystem::Init()
    {
        // Subscribe to enter collision event
        iEventBus<eGameStateEvents, GameStartedEvent>* pStartedBus = EventBusLocator<eGameStateEvents, GameStartedEvent>::Get();
        iEventBus<eGameStateEvents, GameRunningEvent>* pRunningBus = EventBusLocator<eGameStateEvents, GameRunningEvent>::Get();
        iEventBus<eGameStateEvents, GameStoppedEvent>* pStoppedBus = EventBusLocator<eGameStateEvents, GameStoppedEvent>::Get();
        iEventBus<eGameStateEvents, GameOverEvent>* pGameOverBus = EventBusLocator<eGameStateEvents, GameOverEvent>::Get();

        pStartedBus->Subscribe(eGameStateEvents::GAME_STARTED, OnStart);
        pRunningBus->Subscribe(eGameStateEvents::GAME_RUNNING, OnRunning);
        pStoppedBus->Subscribe(eGameStateEvents::GAME_STOPPED, OnStopped);
        pGameOverBus->Subscribe(eGameStateEvents::GAME_OVER, OnGameOver);

        // Main systems must start right away
        Engine* pEngine = EngineLocator::Get();
        GameStateComponent* pState = CoreLocator::GetGameState();
        for (std::string systemName : pState->mainSystems)
        {
            pEngine->AddSystem(systemName);
        }
    }

    void CoreSystem::Start(Scene* pScene)
    {
    }

    void CoreSystem::Update(Scene* pScene, float deltaTime)
    {
    }

    void CoreSystem::Render(Scene* pScene)
    {
    }

    void CoreSystem::End(Scene* pScene)
    {
    }

    void CoreSystem::Shutdown()
    {
        // Subscribe to enter collision event
        iEventBus<eGameStateEvents, GameStartedEvent>* pStartedBus = EventBusLocator<eGameStateEvents, GameStartedEvent>::Get();
        iEventBus<eGameStateEvents, GameRunningEvent>* pRunningBus = EventBusLocator<eGameStateEvents, GameRunningEvent>::Get();
        iEventBus<eGameStateEvents, GameStoppedEvent>* pStoppedBus = EventBusLocator<eGameStateEvents, GameStoppedEvent>::Get();
        iEventBus<eGameStateEvents, GameOverEvent>* pGameOverBus = EventBusLocator<eGameStateEvents, GameOverEvent>::Get();

        pStartedBus->Unsubscribe(eGameStateEvents::GAME_STARTED, OnStart);
        pRunningBus->Unsubscribe(eGameStateEvents::GAME_RUNNING, OnRunning);
        pStoppedBus->Unsubscribe(eGameStateEvents::GAME_STOPPED, OnStopped);
        pGameOverBus->Unsubscribe(eGameStateEvents::GAME_OVER, OnGameOver);
    }

    void CoreSystem::OnStart(const GameStartedEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::STARTED, event.pScene);
    }

    void CoreSystem::OnRunning(const GameRunningEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::RUNNING, event.pScene);
    }

    void CoreSystem::OnStopped(const GameStoppedEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::STOPPED, event.pScene);
    }

    void CoreSystem::OnGameOver(const GameOverEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::GAMEOVER, event.pScene);
    }

    void CoreSystem::ChangeStateSystems(eGameStates prevstate, eGameStates currstate, Scene* pScene)
    {
        using namesIt = std::vector<std::string>::iterator;

        Engine* pEngine = EngineLocator::Get();
        GameStateComponent* pState = CoreLocator::GetGameState();

        // Remove only systems that are not in the current state
        for (std::string systemName : pState->mapStateSystems[prevstate])
        {
            namesIt it = std::find(pState->mapStateSystems[currstate].begin(), pState->mapStateSystems[currstate].end(), systemName);

            if (it == pState->mapStateSystems[currstate].end())
            {
                pEngine->RemoveSystem(systemName, pScene);
            }
        }

        // Add only systems that are not in the previous state
        for (std::string systemName : pState->mapStateSystems[currstate])
        {
            pEngine->AddSystem(systemName, pScene);
        }
    }
}
