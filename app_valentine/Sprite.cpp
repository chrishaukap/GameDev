#include "inc/Sprite.h"
#include "inc/Bitmap.h"
#include "inc/Canvas.h"
#include <assert.h>

// m_imgFile cannot be NULL,
// m_imgMaskFile can be NULL
Sprite::Frame::Frame(Bitmap* img, Bitmap* mask) :
   m_img(img), m_imgMask(mask)
{
   assert(m_img !=NULL);
}
Sprite::Frame::~Frame()
{
   delete m_imgMask;
   delete m_img;
}

// m_imgFile cannot be NULL,
// m_imgMaskFile can be NULL
Sprite::LoadFrame::LoadFrame(char* imgFile, char* imgMaskFile) :
   m_imgFile(imgFile), m_imgMaskFile(imgMaskFile)
{
   assert(m_imgFile !=NULL);
}
Sprite::LoadFrame::~LoadFrame()
{
}


Sprite::Sprite(HINSTANCE hInst) :
   m_frames(),
   m_currentFrame(0),
   m_animations(),
   m_currentAnimation(0),
   m_animSpeed(0),
   m_animClock(0),
   m_x(0),
   m_y(0),
   m_speed(0),
   m_visible(true),
   m_hInst(hInst)
{
}
Sprite::~Sprite()
{
   for( unsigned int i=0; i< m_frames.size(); ++i )
   {
      delete(m_frames[i]);
   }
   m_frames.clear();   
}


// BUG - loading the same mask file over and over
void 
Sprite::loadFrames(LOADFRAMES& imageFrames)
{     
   for( unsigned int i=0; i< imageFrames.size(); ++i)
   {
      Bitmap* mask = NULL;
      Bitmap* img = new Bitmap;
      img->LoadFromFile(m_hInst, imageFrames[i]->m_imgFile);
      if(imageFrames[i]->m_imgMaskFile)
      {
         mask = new Bitmap;
         mask->LoadFromFile(m_hInst, imageFrames[i]->m_imgMaskFile);
      }

      Frame* frame = new Frame(img, mask);
      m_frames.push_back(frame);
   }
}

static void
ValidateAnimation(Sprite::ANIMATION animation, int minRange, int maxRange)
{
   for( unsigned int i=0; i< animation.size(); ++i)
   {
      assert( animation[i] >= minRange );
      assert( animation[i] <= maxRange );
   }
}

void 
Sprite::loadAnimation(ANIMATION animation)
{
   ValidateAnimation(animation, 0, m_frames.size() -1);
   m_animations.push_back(animation);
}
void 
Sprite::setAnimation(int animIndex)
{
   assert(animIndex >=0);
   assert(animIndex <= (int)m_animations.size()-1);
   m_currentAnimation = animIndex; 
   m_currentFrame = 0;
}

int 
Sprite::getWidth()
{
   // grabs from the first frame, we assume all frames are equal
   int width=0, height=0;
   m_frames[0]->m_img->GetSize(width,height);
   return width;
}
int
Sprite::getHeight()
{
   // grabs from the first frame, we assume all frames are equal
   int width=0, height=0;
   m_frames[0]->m_img->GetSize(width,height);
   return height;
}
void
Sprite::setAnimSpeed(int speed)
{
   m_animSpeed = speed;
}
void 
Sprite::moveByX(int x)
{  
   m_x += x;
}
void 
Sprite::moveByY(int y)
{
   m_y += y;
}

void 
Sprite::moveUp()
{   
   m_y -= m_speed;
}

void 
Sprite::moveDown()
{
   m_y += m_speed;
}

void 
Sprite::moveLeft()
{
   m_x -= m_speed;
}

void
Sprite:: moveRight()
{
   m_x += m_speed;
}


void 
Sprite::animate()
{
   if(++m_animClock > m_animSpeed)
   {
      m_animClock = 0;
      m_currentFrame++;
      if(m_currentFrame > (int)m_animations[m_currentAnimation].size() -1 )
         m_currentFrame = 0;
   }
}
void 
Sprite::render(BitmapCanvas& canvas)
{
   // render the current frame at m_x,m_y
   if(m_visible)
   {      
      int frame = m_animations[m_currentAnimation][m_currentFrame];
      if( m_frames[frame]->m_imgMask != NULL )
      {
         // we have a mask that we need to render first
         Blitter bltMask ( *(m_frames[frame]->m_imgMask) );
         bltMask.SetMode (SRCPAINT);
         bltMask.SetDest (m_x, m_y);
         bltMask.BlitTo (canvas);

         Blitter blitFrame( *(m_frames[frame]->m_img) );
         blitFrame.SetMode (SRCAND);
         blitFrame.SetDest (m_x, m_y);
         blitFrame.BlitTo(canvas);
      }
      else
      {
         // no mask, just render the frame
         Blitter bltMask ( *(m_frames[frame]->m_img) );
         bltMask.SetMode (SRCPAINT);
         bltMask.SetDest (m_x, m_y);
         bltMask.BlitTo (canvas);      
      }
      
   }
}
int 
Sprite::getX()
{
   return m_x;
}
int 
Sprite::getY()
{
   return m_y;
}


