#include "Core/AppEngine.h"

#include "Engine/Core/EngineLocator.h"

int main()
{
	using namespace MyEngine;

	Application* pApp = new Application();
	EngineLocator::Set(pApp);

	pApp->LoadConfigurations();

	// TODO: This would be better comming from command args?
	const std::string INITIAL_SCENE = "scene01.json";
	pApp->Init();

	pApp->Run(INITIAL_SCENE, true);

	pApp->Shutdown();

	delete pApp;
}