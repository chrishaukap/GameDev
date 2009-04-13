#ifdef _WINDOWS
#include <windows.h>
#endif

#include "cdhBase.h"
using namespace CDH;

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <gl/glaux.h>		// Header File For The Glaux Library

#include <string> 

#include "assert.h"
#include <math.h>
#include "CGfxOpenGL.h"
#include "GameEngine.h"
#include <vector>
#include "math/quaternion.h"
#include "camera/camera.h"
#include "credits.h"
#include "openGLUtils.h"
#include "resource/resourceLibrary.h"
#include "math/vector3d.h"
#include "resource/imageBase.h"
#include "mainCharacter.h"

const static float g_farplane = 1000.0f;
const static float g_nearplane = 1.0f;
const static float g_fieldOfViewY_degrees = 45.0f;
  
struct Light
{
   Light(float _x,float _y,float _z,float _r,float _g,float _b) : 
      x(_x), y(_y), z(_z), r(_r),g(_g),b(_b) {}
   float x,y,z, r,g,b;
};

static void ShowMessage(const Vector3& p,
                        const char *msg,
                        const myglColor& c,
                        float size) // not length of message, but rendering size
{   
   glPushMatrix();
   {
      glDisable(GL_TEXTURE_2D);	

      glNormal3f( 0.0f, 0.0f, 1.0f );	

      glTranslatef(p.x, p.y, p.z);
      glScalef(size,size,size);
      while (*msg) 
      {
         glutStrokeCharacter(GLUT_STROKE_ROMAN, *msg);
         msg++;
      }

      glEnable(GL_TEXTURE_2D);	
   }
   glPopMatrix();
}

static void ShowDebugMessage(const Vector3& p,
                             const char* msg)
{   
   glPushMatrix();
   {
      glTranslatef(p.x, p.y, p.z);
      glColor3f( 1.0f, 0.0f, 0.0f);
      glScalef(0.004f,0.004f,0.004f);
      while (*msg) 
      {
         glutStrokeCharacter(GLUT_STROKE_ROMAN, *msg);
         msg++;
      }
      glColor3f( 1.0f, 1.0f, 1.0f);
   }
   glPopMatrix();
}

CGfxOpenGL::CGfxOpenGL(const GameEngine& game) : 
   m_gameEngine(game) 
{ }

CGfxOpenGL::~CGfxOpenGL()
{ }

bool CGfxOpenGL::Shutdown()
{
   return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
   if (height == 0)                    // don't want a divide by zero
   {
      height = 1;                 
   }

   glViewport(0, 0, width, height);        // reset the viewport to new dimensions
   glMatrixMode(GL_PROJECTION);            // set projection matrix current matrix
   glLoadIdentity();                       // reset projection matrix

   // calculate aspect ratio of window
   gluPerspective(g_fieldOfViewY_degrees,1.0f, g_nearplane,g_farplane);
	
   glMatrixMode(GL_MODELVIEW);             // set modelview matrix
   glLoadIdentity();                       // reset modelview matrix

   m_windowWidth = width;
   m_windowHeight = height;
}

void CGfxOpenGL::renderScene() const
{
   m_gameEngine.render();
}

bool CGfxOpenGL::Init()
{     
   glEnable(GL_LIGHTING);
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	
   return true;
}
void CGfxOpenGL::render() const
{    
   // HAUKAP - <sigh> disable lighting until i figure it out.  
   // it causes all kinds of transparency issues!   
   glDisable(GL_LIGHTING); 

   glClearColor(1, 1, 0, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
   glLoadIdentity();	

   // camera stuff
   {
      Vector3 refP(0.0f, 0.0f, 0.0f);
      gluLookAt(m_gameEngine.camera()->x,
                m_gameEngine.camera()->y,
                m_gameEngine.camera()->z,
                refP.x, refP.y, refP.z,
                0.0f, 1.0f, 0.0f);
   }   

	renderScene();
		
   char buf[1024];
	sprintf( buf, "Health: [%d] NumLives: [%d]" , 
				m_gameEngine.player()->hitpoints(),
				m_gameEngine.player()->numLives());
   ShowDebugMessage(Vector3(-14.0f, 12.0f, 0.0f), buf);

   CHint endBossHP = m_gameEngine.getEndBossHitPoints();
   if(endBossHP >= 0)
   {
      sprintf( buf, "EndBoss Hitpoints: [%d]" , 
				   m_gameEngine.getEndBossHitPoints());
      ShowDebugMessage(Vector3(7.0f, 12.0f, 0.0f), buf);
   }
   if(m_gameEngine.bossKilled())
   {
      sprintf( buf, "You Won!  Press escape to quit!");
      ShowDebugMessage(Vector3(0.0f, 0.0f, 0.0f), buf);     
   }


   glEnable(GL_LIGHTING);
}



void CGfxOpenGL::renderMenu() const
{
   
   glClearColor(0, 0, 0, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
   glLoadIdentity();

   static float zCamera = 26.0f;
   gluLookAt(0.0f, 0.0f, zCamera,
             0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f);

   // setup lighting for credit screen
   static GLfloat creditsLightPos[] = { -11.0f, 0.0f, 10.0f, 1.0f };
   static bool creditsLightMoveRight = true;
   static float rate = 0.05f;
   if( creditsLightMoveRight )
   {
      if( (creditsLightPos[0] += rate) > -7 )
         creditsLightMoveRight = false;
   }
   else
   {
      if( (creditsLightPos[0] -= rate) < -11 )
         creditsLightMoveRight = true;   
   }

   GLfloat creditsLightColor[] = {0.5f,0.25f,0.25f};
   Vector3 vec = 
      utils3D::getUnitVectorP1ToP2(Vector3(creditsLightPos[0], creditsLightPos[1], creditsLightPos[2]), 
                                   Vector3(creditsLightPos[0], creditsLightPos[1], creditsLightPos[2] - 1) );
   float creditsLightDirection[] = {vec.x, vec.y, vec.z};

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, creditsLightColor);
   glLightfv(GL_LIGHT0, GL_POSITION, creditsLightPos);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, creditsLightDirection);
   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0f);      
   
   myglColor textColor(0.9, 0.9, 0.9);
   char buf[4] = {0};
   static float yval = 0.0f;
   ++yval;
   for(CHUint i=0; i<350; ++i)
   {
      itoa(i, buf, 10);
      ShowMessage(Vector3(-10.0f, -yval + i, 0.0f), buf, textColor, 0.004f);
   }
   ShowMessage(Vector3(-9.0f, 0.0f, 0.0f), "Press Enter!", textColor, 0.004f);

}

void CGfxOpenGL::renderCreditScreen() const
{   
   glClearColor(0, 0, 0, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
   glLoadIdentity();

   static float zCamera = 26.0f;
   gluLookAt(0.0f, 0.0f, zCamera,
             0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f);

   // setup lighting for credit screen
   static GLfloat creditsLightPos[] = { 0.0f, 0.0f, 15.0f, 1.0f };
   {
      float lightMove = 0.4f;
      float lightMaxZ = 38.0f;
      float lightMinZ = 26.0f;
      static bool addIt = true;

      creditsLightPos[2] += addIt ? lightMove : -lightMove;
      if(creditsLightPos[2] >= lightMaxZ) addIt = false;
      if(creditsLightPos[2] <= lightMinZ) addIt = true;
   }

   GLfloat creditsLightColor[] = {0.5f,0.25f,0.25f};
   Vector3 vec = 
      utils3D::getUnitVectorP1ToP2(Vector3(creditsLightPos[0], creditsLightPos[1], creditsLightPos[2]), 
                                   Vector3(creditsLightPos[0], creditsLightPos[1], creditsLightPos[2] - 1) );
   float creditsLightDirection[] = {vec.x, vec.y, vec.z};

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, creditsLightColor);
   glLightfv(GL_LIGHT0, GL_POSITION, creditsLightPos);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, creditsLightDirection);
   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0f);         
   
   myglColor textColor(0.9, 0.9, 0.9);
   myglColor highlightColor(1.0f, 0.0f, 0.0f);

   ShowMessage(Vector3(-7.0f, 5.0f, 0.0f), "Art:", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, 5.0f, 0.0f), "Magiciandude", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, 4.0f, 0.0f), "McWonderBeast", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, 3.0f, 0.0f), "Chris Haukap", textColor, 0.004f);

   ShowMessage(Vector3(-7.0f, 1.5f, 0.0f), "Programming:", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, 1.5f, 0.0f), "Chris Haukap - www.evilmordekai.com", textColor, 0.004f);

   ShowMessage(Vector3(-7.0f, 0.0f, 0.0f), "Music:", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, 0.0f, 0.0f), "Moldy_Tomato", textColor, 0.004f);

   ShowMessage(Vector3(-7.0f, -1.5f, 0.0f), "Sound Effects: ", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, -1.5f, 0.0f), "Roy42", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, -2.5f, 0.0f), "McWonderBeast", textColor, 0.004f);

   
   ShowMessage(Vector3(-7.0f, -4.0f, 0.0f), "Concept Art & Ideas", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, -4.0f, 0.0f), "Michael G.", textColor, 0.004f);
   ShowMessage(Vector3(0.0f, -5.0f, 0.0f), "McWonderBeast", textColor, 0.004f);



   
}