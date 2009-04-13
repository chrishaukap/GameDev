#ifndef LEVELOBJECTS_H
#define LEVELOBJECTS_H

#include "cdhBase.h"
#include <vector>

namespace CDH
{

class GameObject;
typedef std::vector<GameObject*> GameObjects;

class GameEngine;
class LevelObjects
{
public:
   LevelObjects(GameEngine& engine,
                float portalCenterX, float portalCenterY, 
					 float portalWidth, float portalHeight);
   ~LevelObjects();

	// load a level
	Status load();
	Status process();
	
	void restartLevel();
	void startLevel();
	void endLevel();

	// get all objects that lie within the portal (specified at construction)
	GameObjects getObjectsInPortal();

	// get all objects
	GameObjects getAllObjects() const;

private: 
   GameEngine& m_engine;
	float m_portalCenterX;
	float m_portalCenterY;
	float m_portalWidth;
	float m_portalHeight;

	struct Data;
	Data* m_data;
	
	void removeStaleObjects();
	void sort();
	bool isObjectWithinPortal(const GameObject& object) const;

   LevelObjects();
   LevelObjects(const LevelObjects& C);
   const LevelObjects& operator=(const LevelObjects& C);
};

}

#endif // GAMESTATE_H

