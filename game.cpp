// COMP710 GP Framework 2022 - Davor Georgiev
// This include:
#include "game.h"

// Scenes
#include "SceneCheckerboards.h"

// Imgui
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_opengl3.h"

// Input system 
#include "inputsystem.h"
#include "xboxcontroller.h"
#include <iostream>
// Library includes:
#include "renderer.h"
#include "logmanager.h"
// FMOD
#include "soundsystem.h"
// Static Members:
Game* Game::sm_pInstance = 0;
Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}
	return (*sm_pInstance);
}
void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game() : m_pRenderer(0), m_pInputSystem(0), m_bLooping(true)
{
}
Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = 0;

	delete m_pInputSystem;
	m_pInputSystem = 0;

	SoundSystem::DestroyInstance();
}
void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	int bbWidth = 1024;
	int bbHeight = 768;
	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}
	bbWidth = m_pRenderer->GetWidth();
	bbHeight = m_pRenderer->GetHeight();
	m_iLastTime = SDL_GetPerformanceCounter();
	m_pRenderer->SetClearColour(0, 255, 255);
	
	if (!SoundSystem::GetInstance().Initialise())
	{
		LogManager::GetInstance().Log("SoundSystem failed to initialise!");
		return false;
	}

	Scene* pScene = 0;
	pScene = new SceneCheckerboards();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);
	m_iCurrentScene = 0;

	// InputSystem setup
	m_pInputSystem = new InputSystem();
	if (!m_pInputSystem->Initialise())
	{
		LogManager::GetInstance().Log("InputSystem failed to initialise!");
		return false;
	}

	return true;
}

bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	// TODO: Process input here!
	m_pInputSystem->ProcessInput();
	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		m_iLastTime = current;
		m_fExecutionTime += deltaTime;
		Process(deltaTime);
#ifdef USE_LAG
		m_fLag += deltaTime;
		int innerLag = 0;
		while (m_fLag >= stepSize)
		{
			Process(stepSize);
			m_fLag -= stepSize;
			++m_iUpdateCount;
			++innerLag;
		}
#endif //USE_LAG
		Draw(*m_pRenderer);
	}
	return m_bLooping;
}
void Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);
	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);
	SoundSystem::GetInstance().Process();

	if (m_pInputSystem->GetNumberOfControllersAttached() > 0)
	{
		ButtonState xboxA = m_pInputSystem->GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_A);
		ButtonState xboxX = m_pInputSystem->GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_X);
		ButtonState xboxLeft = m_pInputSystem->GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		ButtonState xboxRight = m_pInputSystem->GetController(0)->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

		if (xboxA == BS_PRESSED)
		{
			std::cout << "XboxA Pressed\n";
		}
		if (xboxLeft == BS_PRESSED)
		{
			std::cout << "XboxLeft Pressed\n";
		}
	}
}

void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;
	renderer.Clear();
	m_scenes[m_iCurrentScene]->Draw(renderer);
	// TODO: Add game objects to draw here!
	DebugDraw();
	renderer.Present();
	
}
void
Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElapsedSeconds += deltaTime;
	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}

void Game::DebugDraw
()
{
	if (m_bShowDebugWindow)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("COMP710 GP Framework (%s)", "2024, S2");
		if (ImGui::Button("Quit"))
		{
			Quit();
		}
		ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, m_scenes.size() - 1, "%d");
		m_scenes[m_iCurrentScene]->DebugDraw();
		ImGui::End();
	}
}

void Game::ToggleDebugWindow
()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;
	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}


