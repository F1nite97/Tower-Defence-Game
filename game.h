// Library includes:
#include <vector>
// COMP710 GP Framework
#ifndef GAME_H
#define GAME_H
// Forward declarations:
class Renderer;
class Scene;
class InputSystem;

class Game
{
	// Member methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	bool Initialise();
	bool DoGameLoop();
	void Quit();
	void ToggleDebugWindow();
protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessFrameCounting(float deltaTime);
	void DebugDraw();
private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);

// Member data:
public:
protected:
	static Game* sm_pInstance;
	Renderer* m_pRenderer;
	InputSystem* m_pInputSystem;

	_int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElapsedSeconds;
	int m_iFrameCount;
	int m_iFPS;
	std::vector<Scene*> m_scenes;
	int m_iCurrentScene;
	bool m_bShowDebugWindow;
#ifdef USE_LAG
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG
	bool m_bLooping;
private:
};
#endif // GAME_H

