#include "EditorEngine.h"

#include "Engine/ECS/System/SystemBuilder.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

// Custom editor systems
#include "Gameplay/FlyingCameraSystem.h"

#include "UI/MenuSystem.h"
#include "UI/EntityBarSystem.h"
#include "UI/GuizmoSystem.h"

// Here we use to include all we need for this specific app details to work
namespace MyEngine
{
	// TODO: For now duplicated from App we just need editor to create the scenes,
	// but later this should all come separated from config and better system management
	void Editor::Init()
	{
		Engine::Init();

		// Register custom app systems
		SystemBuilder::RegisterSystem("MenuSystem", []() { return new MenuSystem; });
		SystemBuilder::RegisterSystem("EntityBarSystem", []() { return new EntityBarSystem; });
		SystemBuilder::RegisterSystem("FlyingCameraSystem", []() { return new FlyingCameraSystem; });
		SystemBuilder::RegisterSystem("GuizmoSystem", []() { return new GuizmoSystem; });

		GameStateComponent* pStates = CoreLocator::GetGameState();

		pStates->mainSystems.push_back("WindowFrameSystem");
		pStates->mainSystems.push_back("TransformParentSystem");
		pStates->mainSystems.push_back("WindowSystem");
		pStates->mainSystems.push_back("InputSystem");
		pStates->mainSystems.push_back("BaseUISystem"); // Has to come after inputsystem, to init imgui after we register out input callback
		// Graphics
		pStates->mainSystems.push_back("ShaderSystem");
		pStates->mainSystems.push_back("CameraSystem");
		pStates->mainSystems.push_back("RenderSystem");
		pStates->mainSystems.push_back("LightSystem");
		pStates->mainSystems.push_back("AnimationSystem");

		pStates->mainSystems.push_back("MenuSystem");
		pStates->mainSystems.push_back("EntityBarSystem");
		pStates->mainSystems.push_back("FlyingCameraSystem");
		pStates->mainSystems.push_back("GuizmoSystem");

		// TODO: This could come from a config file
		// TODO: Could this be categorized to avoid having to put all in the config?
		pStates->mapStateSystems[eGameStates::NOT_STARTED] = {};

		pStates->mapStateSystems[eGameStates::STARTED] = {};

		pStates->mapStateSystems[eGameStates::RUNNING] = {
			// Graphics
			"AnimationPlayerSystem",
			// Physics
			"MovementSystem",
			"RotationSystem",
			"GravitySystem",
			"GridBroadPhaseSystem",
			"CollisionSystem",
			#ifdef DEBUG
			// Debug
			"DebugSystem",
			"DrawGridSystem",
			"DrawCollisionSystem"
			#endif
		};

		pStates->mapStateSystems[eGameStates::STOPPED] = {};

		pStates->mapStateSystems[eGameStates::GAMEOVER] = {};
	}
}
