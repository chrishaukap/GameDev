#if !defined (SPRITE_H)
#define SPRITE_H

#include <windows.h>
#include <assert.h>

#include <vector>

class Bitmap;
class BitmapCanvas;
class Sprite
{
public:
   struct LoadFrame
   {
      char* m_imgFile;
      char* m_imgMaskFile;
      LoadFrame(char* imgFile, char* imgMaskFile);
      ~LoadFrame();   
   };
   struct Frame
   {
      Bitmap* m_img;
      Bitmap* m_imgMask;
      Frame(Bitmap* img, Bitmap* mask);
      ~Frame();   
   };
   typedef std::vector<int> ANIMATION;
   typedef std::vector<ANIMATION> ANIMATIONS;
   typedef std::vector<const LoadFrame*> LOADFRAMES;

   Sprite(HINSTANCE hInst);
   virtual ~Sprite();
   
   // call this first
   void loadFrames(LOADFRAMES& imageFrames);
   // then call this
   void loadAnimation(ANIMATION animation);
   void setAnimation(int animIndex);
   void moveByX(int x);
   void moveByY(int y);
   void moveUp();
   void moveDown();
   void moveLeft();
   void moveRight();

   void setX(int x){m_x = x;}
   void setY(int y){m_y = y;}
   void setSpeedFactor(int n){m_speed = n;}
   int getSpeedFactor(){return m_speed;}
   int getWidth();
   int getHeight();
   void animate();
   void render(BitmapCanvas& canvas);
   int getX();
   int getY();
   void setAnimSpeed(int speed);
   void setVisiblity(bool visible){m_visible = visible;}
private:
   std::vector<Frame*> m_frames;
   int m_currentFrame;


   ANIMATIONS m_animations;
   int m_currentAnimation;
   int m_animSpeed;
   int m_animClock;

   int m_x;
   int m_y;
   int m_speed;

   bool m_visible;
   HINSTANCE m_hInst;
};


#endif
