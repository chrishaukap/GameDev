#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

struct GraphicalAsset
{
	char *mFilename;
};

// Properties
#define IS_PLAYER	1<<0
#define IS_MOVABLE	1<<1
#define IS_FRIENDLY	1<<2
#define IS_COVER	1<<3	// as in cover to hide behind
#define IS_PORTAL	1<<4
#define IS_LIGHT	1<<5
#define IS_NULL		1<<6

class Object
{
public:
	Object(char c, char *assetDir);
	~Object();

	void SetProperty(long prop)	{ mProps |= prop; }
	void RemoveProperty(long prop) { mProps &= ~prop; }

	bool IsPlayer(void)		
	{ 
		return (mProps & IS_PLAYER) != 0; 
	}
	bool IsMovable(void)	
	{ 
		return (mProps & IS_MOVABLE) != 0; 
	}
	bool IsFriendly(void)	
	{ 
		return (mProps & IS_FRIENDLY) != 0; 
	}
	bool IsCover(void)		
	{ 
		return (mProps & IS_COVER) != 0; 
	}	
	bool IsLight(void)		
	{ 
		return (mProps & IS_LIGHT) != 0; 
	}
	bool IsNull(void)		
	{ 
		return (mProps & IS_NULL) != 0; 
	}
	bool IsPortal(void)		
	{ 
		return (mProps & IS_PORTAL) != 0; 
	}	

	void Render(void);
	
	GraphicalAsset mAsset;
	char mIcon;
private:
	long mProps;	// max flags is 32

	Object(void); //undef
};

class Tile
{
public:
	Tile() { }
	~Tile()
	{
		delete mpGround;
		delete mpThing;
	}

	bool IsMovable(void)	{ return (mpGround->IsNull() && mpThing->IsMovable()); }
	bool IsNull(void)	{ return (mpGround->IsNull() && (mpThing->IsNull())); }

	Object *mpGround;	// Null, light, immovable
	Object *mpThing;	// Box, player, portals, innocents
};

enum MoveDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class Player : public Object
{
public:
	Player(char c, char *assetDir):Object(c,assetDir),xCoord(-1),yCoord(-1) { }
	~Player();

	void Move(MoveDir dir)
	{
		switch (dir)
		{
		case UP:
			yCoord--;
			break;
		case DOWN:
			yCoord++;
			break;
		case LEFT:
			xCoord--;
			break;
		case RIGHT:
			xCoord++;
			break;
		}
	}
	int xCoord;
	int yCoord;
private:
	Player();	// undef
};

#endif //__OBJECT_HPP__
