#ifdef _WINDOWS
#include <windows.h>
#endif

#include "cdhBase.h"
using namespace CDH;

#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glut.h>
#include <gl/glaux.h>		// Header File For The Glaux Library

#include <string> 

#include "assert.h"
#include <math.h>
#include "CGfxOpenGL.h"
#include "tree/Tree.h"
#include "tree/branch.h"
#include "GameEngine.h"
#include "peeps/peep.h"
#include "fence.h"
#include "graveyard.h"
#include <vector>
#include "quaternion.h"
#include "camera.h"
#include "credits.h"
#include "openGLUtils.h"
#include "resourceLibrary.h"
#include "vector3d.h"

const static float g_farplane = 1000.0f;
const static float g_nearplane = 1.0f;
const static float g_fieldOfViewY_degrees = 45.0f;

const CHUint g_numTextures = 15;
const GLuint g_peep1TextureFrame1MaskIdx = 0;
const GLuint g_peep1TextureFrame2MaskIdx = 8;
const GLuint g_peep1TextureFrame1Idx = 1;
const GLuint g_peep1TextureFrame2Idx = 9;
const GLuint g_dirtTextureIdx = 4;
const GLuint g_tree1TextureIdx = 5;
const GLuint g_tree2TextureIdx = 14;
const GLuint g_limbTextureIdx = 6;
const GLuint g_treeTextureMaskIdx = 7;
const GLuint g_peep2ArmsUpTextureMaskIdx = 10;
const GLuint g_peep2ArmsUpTextureIdx = 11;
const GLuint g_peep2ArmsOutTextureMaskIdx = 12;
const GLuint g_peep2ArmsOutTextureIdx = 13;
const GLuint g_peep2ArmsDownTextureMaskIdx = 2;
const GLuint g_peep2ArmsDownTextureIdx = 3;
GLuint g_texture[g_numTextures];
GLUquadricObj *quadratic;	

struct Light
{
   Light(float _x,float _y,float _z,float _r,float _g,float _b) : 
      x(_x), y(_y), z(_z), r(_r),g(_g),b(_b) {}
   float x,y,z, r,g,b;
};
Light g_overheadLight(0, 10, 40, 1, 1, 1);
Light g_spotLight(0, 30, 10, 11, 1, 1);

static void drawFloorPlane()
{
   float y = -0.0f;
   float minx = GraveyardBounds::leftWallX - 80;
   float maxx = GraveyardBounds::rightWallX + 80;
   float minz = GraveyardBounds::backWallZ - 80;
   float maxz = GraveyardBounds::frontWallZ + 80;

   glPushMatrix();
   {
      glEnable(GL_TEXTURE_2D);		
	   glEnable(GL_BLEND);								
	   glDisable(GL_DEPTH_TEST);	  

      GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };   
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
      glNormal3f( 0.0f, 1.0f, 0.0f );	

      glBindTexture(GL_TEXTURE_2D, g_texture[4]);		
      glBlendFunc(GL_ONE, GL_ONE);		

      float size = 3.0f;
      for( float x = minx;
           x < maxx; 
           x += size)
         for( float z = minz;
              z < maxz; 
              z += size)
         {         			
	         glBegin(GL_QUADS);						
		         glTexCoord2f( 0.0f,  0.0f);   glVertex3f(x,        y,  z - size);	// Bottom Left
		         glTexCoord2f( 1.0f,  0.0f);   glVertex3f(x + size, y,  z - size);	// Bottom Right
		         glTexCoord2f( 1.0f,  1.0f);   glVertex3f(x + size, y,  z);	// Top Right
		         glTexCoord2f( 0.0f,  1.0f);   glVertex3f(x,        y,  z);	// Top Left
	         glEnd();	
         }
         
	   glEnable(GL_DEPTH_TEST);						
	   glDisable(GL_BLEND);	   
      glDisable(GL_TEXTURE_2D);	
   }
   glPopMatrix();
}

enum DIRECTION
{
   DIR_UP = 0, DIR_DOWN, DIR_LEFT, DIR_RIGHT
};

static void ShowMessage(const Vector3& p,
                        const char *msg,
                        const myglColor& c,
                        float size) // not length of message, but rendering size
{   
   glPushMatrix();
   {
      glDisable(GL_TEXTURE_2D);	

      GLfloat color[] = { c.r, c.g, c.b, 1 };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  
      glNormal3f( 0.0f, 0.0f, 1.0f );	

      glTranslatef(p.x, p.y, p.z);
      glScalef(size,size,size);
      while (*msg) 
      {
         //glutStrokeCharacter(GLUT_STROKE_ROMAN, *msg);
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
      glDisable(GL_LIGHTING);
      glTranslatef(p.x, p.y, p.z);
      glScalef(0.004f,0.004f,0.004f);
      while (*msg) 
      {
         //glutStrokeCharacter(GLUT_STROKE_ROMAN, *msg);
         msg++;
      }
      glEnable(GL_LIGHTING);
   }
   glPopMatrix();
}

// HAUKAP - needs to go in image handling lib
static 
AUX_RGBImageRec* LoadBMP(char *filename)                
{
   FILE *file = NULL;         
   if (!filename)  
      return NULL;

   file = fopen( filename, "r" );
   assert( file != NULL );
   if (file)                        
   {
      fclose(file);                
      return auxDIBImageLoad(filename);    
   }
	else
	{
		std::string msg = "Could not find the file: ";
		msg += filename;
		// HAUKAP - unix/windos messaging/error handling system
		MessageBox( NULL, msg.c_str(), "Required file missing", MB_OK );
   return NULL;                      
}
}
CGfxOpenGL::CGfxOpenGL(const GameEngine& game) : 
   m_gameEngine(game) 
{ }
CGfxOpenGL::~CGfxOpenGL()
{ }

bool CGfxOpenGL::Shutdown()
{
	Status stat = ResourceLibrary::terminate();
	assert(stat == StatusSuccess);

	gluDeleteQuadric(quadratic);						// Delete The Quadratic To Free System Resources
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
#if 1
   m_gameEngine.render();
#else	
	GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector3 p1(0.0f, 7.0f, 0.0f);
	Vector3 p2(5.0f, 12.0f, 0.0f);
	Vector3 p3(5.0f, 11.0f, 8.0f);
	float dist1 = utils3D::distanceP1P2(p1, p2 );
	float dist2 = utils3D::distanceP1P2(p2, p3 );

	Vector3 vecP12P2( utils3D::getVectorP1ToP2(p1, p2) );
	vecP12P2.normalise();
	Vector3 vecP22P3( utils3D::getVectorP1ToP2(p2, p3) );
	vecP22P3.normalise();

	Vector3 zAxis(0.0f, 0.0f, 1.0f);

	float angle1 = acos(vecP12P2.scalarProduct(zAxis));
	Vector3 axis1 = zAxis.vectorProduct(vecP12P2);	
	float angle2 = acos(vecP22P3.scalarProduct(zAxis));
	Vector3 axis2 = zAxis.vectorProduct(vecP22P3);	

	float startrad1 = 2.0f, startrad2 = 1.0f;
	float endrad1 = 1.0f, endrad2 = 0.0f;

	glPushMatrix();
	{     		
		glTranslatef(p1.x, p1.y, p1.z);
		glRotatef(RAD2DEG(angle1), axis1.x, axis1.y, axis1.z);
		gluCylinder(quadratic, startrad1, endrad1, dist1, 32, 32);
	}
	glPopMatrix();
	glPushMatrix();
	{     		
		glTranslatef(p2.x, p2.y, p2.z);
		glRotatef(RAD2DEG(angle2), axis2.x, axis2.y, axis2.z);
		gluCylinder(quadratic, startrad2, endrad2, dist2, 32, 32);
	}
	glPopMatrix();

   glBegin(GL_LINES);
   {
      glVertex3f( p1.x, p1.y, p1.z); 
      glVertex3f( p2.x, p2.y, p2.z ); 
   }
   glEnd();   
   glBegin(GL_LINES);
   {
      glVertex3f( p2.x, p2.y, p2.z ); 
      glVertex3f( p2.x, p2.y+10, p2.z ); 
   }
   glEnd();   
   glBegin(GL_LINES);
   {
      glVertex3f(  p1.x, p1.y, p1.z ); 
      glVertex3f(  p1.x, p1.y+10, p1.z); 
   }
   glEnd();   
#endif
}

bool CGfxOpenGL::Init()
{     
   {
		Status stat = ResourceLibrary::initialize();
		assert(stat == StatusSuccess);

		RES_HANDLE h1 = ResourceLibrary::LoadResource("img/peep1_frame1_mask.bmp");
		RES_HANDLE h2 = ResourceLibrary::LoadResource("img/peep1_frame1.bmp");
		AUX_RGBImageRec* r1 = ResourceLibrary::getRGBImage(h1);
		AUX_RGBImageRec* r2 = ResourceLibrary::getRGBImage(h2);

      AUX_RGBImageRec* textures[g_numTextures];         
      memset(textures, 0, sizeof(void *)*g_numTextures);
      
      textures[g_peep1TextureFrame1MaskIdx] = LoadBMP("img/peep1_frame1_mask.bmp");
      assert(textures[g_peep1TextureFrame1MaskIdx] != NULL);
      textures[g_peep1TextureFrame1Idx] = LoadBMP("img/peep1_frame1.bmp");
      assert(textures[g_peep1TextureFrame1Idx] != NULL);
      textures[g_peep1TextureFrame2MaskIdx] = LoadBMP("img/peep1_frame2_mask.bmp");
      assert(textures[g_peep1TextureFrame2MaskIdx] != NULL);
      textures[g_peep1TextureFrame2Idx] = LoadBMP("img/peep1_frame2.bmp");
      assert(textures[g_peep1TextureFrame2Idx] != NULL);

      textures[g_dirtTextureIdx] = LoadBMP("img/dirt15.bmp");
      assert(textures[g_dirtTextureIdx] != NULL);

      textures[g_tree1TextureIdx] = LoadBMP("img/tree1.bmp");   //g_treeTextureIdx
      assert(textures[g_tree1TextureIdx] != NULL);
      textures[g_tree2TextureIdx] = LoadBMP("img/tree2.bmp");   //g_treeTextureIdx
      assert(textures[g_tree2TextureIdx] != NULL);
      textures[g_limbTextureIdx] = LoadBMP("img/dirt.bmp"); //g_limbTextureIdx
      assert(textures[g_limbTextureIdx] != NULL);
      textures[g_treeTextureMaskIdx] = LoadBMP("img/tree_mask.bmp");   //g_treeTextureIdx
      assert(textures[g_treeTextureMaskIdx] != NULL);  
      
      textures[g_peep2ArmsUpTextureMaskIdx] = LoadBMP("img/peep2_armsup_mask.bmp");
      assert(textures[g_peep2ArmsUpTextureMaskIdx] != NULL);
      textures[g_peep2ArmsUpTextureIdx] = LoadBMP("img/peep2_armsup.bmp");
      assert(textures[g_peep2ArmsUpTextureIdx] != NULL);

      textures[g_peep2ArmsOutTextureMaskIdx] = LoadBMP("img/peep2_armsout_mask.bmp");
      assert(textures[g_peep2ArmsOutTextureMaskIdx] != NULL);
      textures[g_peep2ArmsOutTextureIdx] = LoadBMP("img/peep2_armsout.bmp");
      assert(textures[g_peep2ArmsOutTextureIdx] != NULL);

      textures[g_peep2ArmsDownTextureMaskIdx] = LoadBMP("img/peep2_armsdown_mask.bmp");
      assert(textures[g_peep2ArmsDownTextureMaskIdx] != NULL);
      textures[g_peep2ArmsDownTextureIdx] = LoadBMP("img/peep2_armsdown.bmp");
      assert(textures[g_peep2ArmsDownTextureIdx] != NULL);


      glGenTextures(g_numTextures, &g_texture[0]); 

      for (CHUint i=0; i < g_numTextures; ++i)
      {
         glBindTexture(GL_TEXTURE_2D, g_texture[i]);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,  
                      textures[i]->sizeX, 
                      textures[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
                      textures[i]->data);
         
      }

      for (CHUint i=0; i < g_numTextures; ++i)						
      {
         if (textures[i])							
         {
            if (textures[i]->data)	
               free(textures[i]->data);
            free( textures[i] );				
         }
      }
   }

   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); /* or GL_REPLACE */
	glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
   glEnable(GL_LIGHTING);
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glEnable(GL_TEXTURE_2D);							// Enable 2D Texture Mapping

   // HAUKAP - FOG screws up the transparencies of the peep!!!! figure this out
   //glEnable(GL_FOG);
   //glFogf(GL_FOG_DENSITY, 0.0075f);
   //glFogf(GL_FOG_MODE, GL_EXP2);
   //float fogcolor[] = {0.2f,0.0f,0.0f};
   //glFogfv(GL_FOG_COLOR, fogcolor);

   //float globalambient[] = {1.0, 1.0, 1.0, 1.0f};
   //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);
	
	quadratic=gluNewQuadric();							// Create A Pointer To The Quadric Object (Return 0 If No Memory) (NEW)
	gluQuadricNormals(quadratic, GLU_SMOOTH);			// Create Smooth Normals (NEW)
	gluQuadricTexture(quadratic, GL_TRUE);				// Create Texture Coords (NEW)


   return true;
}
void CGfxOpenGL::render() const
{      
   glClearColor(0, 0, 0, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
   glLoadIdentity();	

   // camera stuff
   {
      Vector3 refP(0.0f, 10.0f, 0.0f);
      gluLookAt(m_gameEngine.camera()->x,
                m_gameEngine.camera()->y,
                m_gameEngine.camera()->z,
                refP.x, refP.y, refP.z,
                0.0f, 1.0f, 0.0f);
   }   

   { // LIGHTING
      {
         // OVERHEAD LIGHT
         GLfloat overheadLightcolor[] = { g_overheadLight.r, g_overheadLight.g, g_overheadLight.b, 1.0f };
         GLfloat overheadLightPos[] = { g_overheadLight.x, g_overheadLight.y, g_overheadLight.z, 1.0f };
         Vector3 vec = 
            utils3D::getUnitVectorP1ToP2(Vector3(g_overheadLight.x, g_overheadLight.y, g_overheadLight.z), 
                                         Vector3(0, g_overheadLight.y, 0) );
         float overheadLightDirection[] = {vec.x, vec.y, vec.z};

         glEnable(GL_LIGHT0);
         glLightfv(GL_LIGHT0, GL_DIFFUSE, overheadLightcolor);
         glLightfv(GL_LIGHT0, GL_POSITION, overheadLightPos);
         glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, overheadLightDirection); 
         glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90);         
      }

      {
         // SPOT LIGHT
         static float angle = 0.0f;
         angle += 4;
         if(angle >= 360)
            angle = 0.0f;
         g_spotLight.x = cos(DEG2RAD(angle)) * 10;
         g_spotLight.z = sin(DEG2RAD(angle)) * 10;
         GLfloat spotLightColor[] = { g_spotLight.r, g_spotLight.g, g_spotLight.b, 1.0f };
         GLfloat spotLightPos[] = { g_spotLight.x, g_spotLight.y, g_spotLight.z, 1.0f };
         Vector3 vec = utils3D::getUnitVectorP1ToP2(Vector3(g_spotLight.x, g_spotLight.y, g_spotLight.z), Vector3(0,0,0) );
         float spotLightDirection[] = {vec.x, vec.y, vec.z};

         glEnable(GL_LIGHT1);
         glLightfv(GL_LIGHT1, GL_DIFFUSE, spotLightColor);
         glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);
         glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotLightDirection);
         glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);             
      }
   }

   // floor plane
   drawFloorPlane();

   glPushMatrix();
   {
      // render the scene
      renderScene();
   }
   glPopMatrix();   
   
/*
#if _DEBUG
   char buf[256] = {0};
   sprintf_s(buf, "SPROUTS: [%d], PEEPPARTS: [%d]", m_gameEngine.numSpouts(), m_gameEngine.numPeepParts() );
   ShowDebugMessage(Vector3(-15.0f, 18.0f, 0.0f), buf);
   sprintf_s(buf, "DROPLETS: [%d]", m_gameEngine.numDroplets() );
   ShowDebugMessage(Vector3(-15.0f, 16.0f, 0.0f), buf);
#endif*/
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
   static GLfloat creditsLightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
   {
      float lightMove = 0.3f;
      float lightMaxZ = 38.0f;
      float lightMinZ = 24.0f;
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
   ShowMessage(Vector3(-10.0f, 8.0f, 0.0f), Credits::line1, textColor, 0.004f);
   ShowMessage(Vector3(-10.0f, 6.0f, 0.0f), Credits::line2, textColor, 0.004f);
   ShowMessage(Vector3(-10.0f, 4.0f, 0.0f), Credits::line3, textColor, 0.004f);
   ShowMessage(Vector3(-10.0f, 2.0f, 0.0f), Credits::line4, textColor, 0.004f);
   ShowMessage(Vector3(-10.0f, 0.0f, 0.0f), Credits::line5, textColor, 0.004f);
   ShowMessage(Vector3(2.5f, 0.0f, 0.0f), EMAIL_ADDRESS, highlightColor, 0.005f);

   ShowMessage(Vector3(-10.0f, -2.0f, 0.0f), "Please visit ", textColor, 0.004f);
   ShowMessage(Vector3(-6.5f, -2.0f, 0.0f), WEB_ADDRESS, highlightColor, 0.005f);
   ShowMessage(Vector3(3.0f, -2.0f, 0.0f), "for updates to this game!", textColor, 0.004f);
}