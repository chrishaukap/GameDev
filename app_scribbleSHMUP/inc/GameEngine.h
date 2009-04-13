#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "cdhBase.h"
#include "defines.h"
#include <vector>

namespace CDH
{

enum ProjectileType
{
	Throwing = 0,
	Rocket,
	Pulse
};

struct Camera;

class MainCharacter;
class Background;
class KeyInput;
class Vector3;
class Animation;
class ObjectScroller;

class LevelObjects;
class GameEngine
{
public:
   GameEngine(const KeyInput& input);
   ~GameEngine();

   void initializePreMenu();
   void initializePostMenu();
   void process();
   void destroy();
   
   void render() const;

   // camera accessor
   Camera* camera() {return m_camera;}
   const Camera* camera() const {return m_camera;}

	const MainCharacter* player() const {return m_mainCharacter;}
   

	Status createProjectile( ProjectileType type, 
									const Vector3& position, 
									const Vector3& orientation,
                           CHUint team);
   CHint getEndBossHitPoints() const;
   bool bossKilled() const;
private:

   // processing
   void processCamera();   
	void resolveCollisions();
   void objectCleanup();
   Camera* m_camera;
   
   // key input
   const KeyInput& m_input;

	LevelObjects* m_levelObjects;

	MainCharacter* m_mainCharacter;
	Background* m_background;
	
	Animation* m_projectileAnimation; 
	Animation* m_mainCharacterAnimation;

   ObjectScroller* m_objectScroller;

	float m_boundaryLeft, m_boundaryRight;

   GameEngine(const GameEngine& C);
   const GameEngine& operator=(const GameEngine& C);
};

}

#endif // GAMESTATE_H

