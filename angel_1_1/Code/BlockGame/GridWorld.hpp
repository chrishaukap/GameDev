#ifndef __GRIDWORLD_HPP__
#define __GRIDWORLD_HPP__

#include "Object.hpp"
#include <vector>
using namespace std;

#define MAX_WORLD_WIDTH		50
#define	MAX_WORLD_HEIGHT	50

class GridWorld
{
public:
	GridWorld(char *fileDir);
	~GridWorld();

	Object *FindObjectInList(char c);
	bool ICanMove(MoveDir dir, int x, int y, int boxDepth = 0);
	void PushInDir(MoveDir dir, int x, int y);
	bool IsAlive();
	void Update(MoveDir dir);
	void DebugDraw(void) const;
	
	Player *mpPlayerTile;
	Tile *mGrid[MAX_WORLD_WIDTH][MAX_WORLD_HEIGHT];
private:
	GraphicalAsset mBackground;
	bool mIsValid;

	vector<Object *> mWorldTiles;

	GridWorld(void); //undef
};

#endif //__GRIDWORLD_HPP__
