// COMP710 GP Framework 2022
#ifndef SCENECHECKERBOARDS_H
#define SCENECHECKERBOARDS_H
// Local includes:
#include "scene.h"
#include "fmod.hpp"
// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class SceneCheckerboards : public Scene
{
	// Member methods:
public:
	SceneCheckerboards();
	virtual ~SceneCheckerboards();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
protected:
private:
	SceneCheckerboards(const SceneCheckerboards& sceneCheckerboards);
	SceneCheckerboards& operator=(const SceneCheckerboards& sceneCheckerboards);
	FMOD::Sound* m_pSwishSound;
	// Member data:
public:
protected:
	Sprite* m_pCorners[4];
	Sprite* m_pCentre;
	Sprite* m_pWelcomeText;
	float m_angle;
	float m_rotationSpeed;
private:
};
#endif // SCENECHECKERBOARDS_H