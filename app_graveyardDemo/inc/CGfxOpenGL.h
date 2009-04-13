#ifndef __GL_COMPONENT
#define __GL_COMPONENT
#include "defines.h"

class GameEngine;
class Vector3;
class CGfxOpenGL
{
public:
private:
	int m_windowWidth;
	int m_windowHeight;
   const GameEngine& m_gameEngine;

public:
	CGfxOpenGL(const GameEngine& game);
	virtual ~CGfxOpenGL();

	bool Init();
	bool Shutdown();

	void SetupProjection(int width, int height);

	void render() const;
   void renderScene() const;
   void renderMenu() const;
   void renderCreditScreen() const;
};

#endif