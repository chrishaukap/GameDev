//------------------------------------
//  view.cpp
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "inc/view.h"
#include "inc/canvas.h"
#include "inc/resource.h"
#include "inc/defines.h"
#include <cmath>
#include <cstring>

#include <time.h>

#include "mmsystem.h"

static void
LoadBubbleSrite(Sprite& bubbleSprite)
{
   Sprite::LoadFrame frame1(STR_BUBBLEHEARTS_BMP,STR_BUBBLEMASK_BMP);
   Sprite::LoadFrame frame2(STR_BUBBLEILOVEYOU_BMP,STR_BUBBLEMASK_BMP);
   Sprite::LoadFrame frame3(STR_BUBBLEVALENTINE_BMP,STR_BUBBLEMASK_BMP);

   Sprite::LOADFRAMES frames;
   frames.push_back(&frame1);
   frames.push_back(&frame2);
   frames.push_back(&frame3);

   Sprite::ANIMATION anim;
   anim.push_back(0);
   anim.push_back(1);
   anim.push_back(2);

   bubbleSprite.loadFrames(frames);
   bubbleSprite.loadAnimation(anim);
   bubbleSprite.setAnimation(0);
   bubbleSprite.setAnimSpeed(30);
}


static void
LoadHeartSprite(Sprite& heartSprite)
{
   Sprite::LoadFrame frame1(STR_HEART0_BMP,STR_HEARTMASK_BMP);
   Sprite::LoadFrame frame2(STR_HEART1_BMP,STR_HEARTMASK_BMP);
   Sprite::LoadFrame frame3(STR_HEART2_BMP,STR_HEARTMASK_BMP);
   Sprite::LoadFrame frame4(STR_HEART3_BMP,STR_HEARTMASK_BMP);
   Sprite::LoadFrame frame5(STR_HEART4_BMP,STR_HEARTMASK_BMP);
   
   Sprite::LOADFRAMES frames;
   frames.push_back(&frame1);
   frames.push_back(&frame2);
   frames.push_back(&frame3);
   frames.push_back(&frame4);
   frames.push_back(&frame5);

   Sprite::ANIMATION heartWakeUp;
   heartWakeUp.push_back(0);
   heartWakeUp.push_back(1);

   Sprite::ANIMATION heartSleep;
   heartSleep.push_back(2);
   heartSleep.push_back(1);
   heartSleep.push_back(0);

   Sprite::ANIMATION heartTalk;
   heartTalk.push_back(3);
   heartTalk.push_back(4);

   heartSprite.loadFrames(frames);
   heartSprite.loadAnimation(heartWakeUp);
   heartSprite.loadAnimation(heartSleep);
   heartSprite.loadAnimation(heartTalk);
   heartSprite.setAnimation(0);
   heartSprite.setAnimSpeed(10);
   heartSprite.setSpeedFactor(2);
}

static void
LoadBugSprites(std::vector<Sprite*>& sprites, HINSTANCE hInst)
{   
   unsigned int x=0, y=0;
   for( unsigned int i=0; i<10; ++i)
   {
      Sprite::LoadFrame frame1(STR_BUG1_BMP,STR_BUG1MASK_BMP);
      Sprite::LoadFrame frame2(STR_BUG2_BMP,STR_BUG2MASK_BMP);
      Sprite::LoadFrame frame3(STR_BUG3_BMP,NULL);

      Sprite::LOADFRAMES frames;
      frames.push_back(&frame1);
      frames.push_back(&frame2);
      frames.push_back(&frame3);

      Sprite::ANIMATION bugFly;
      bugFly.push_back(0);
      bugFly.push_back(1);

      Sprite::ANIMATION bugDie;
      bugDie.push_back(2);

      Sprite* sprite = new Sprite(hInst);
      sprite->loadFrames(frames);
      sprite->loadAnimation(bugFly);
      sprite->loadAnimation(bugDie);
      sprite->setAnimation(0);
      sprite->setAnimSpeed(10);
      sprite->setX(x+=33);
      sprite->setY(y+=33);
      sprites.push_back(sprite);
   }
}

View::~View()
{
   delete _heartSprite;
   delete _bubbleSprite;
   unsigned int numBugSprites = _bugSprites.size();
   for(unsigned int i=0; i<numBugSprites; ++i)
   {
      delete _bugSprites[i];
   }
   _bugSprites.clear();
}

View::View (HINSTANCE hInst) :
   _tick (0),
   _heartSprite(NULL),
   _bubbleSprite(NULL),
   _bugSprites()
{
   srand ( (unsigned int)time(NULL) );
   _background.Load (hInst, IDB_BG);
   _background.GetSize (_widthBkg, _heightBkg);

   _heartSprite = new Sprite(hInst);
   LoadHeartSprite(*_heartSprite);

   _bubbleSprite = new Sprite(hInst);
   LoadBubbleSrite(*_bubbleSprite);

   LoadBugSprites(_bugSprites, hInst);

   DesktopCanvas canvas;
   _bitmapBuf.CreateCompatible (canvas, 1, 1);
   _cx = 1;
   _cy = 1;
}

void View::Update (Canvas & canvas)
{
   Bitmap bmp (canvas, _cx, _cy);
   BitmapCanvas bmpCanvas (canvas, bmp);

   RECT rect = { 0, 0, _cx, _cy};
   bmpCanvas.WhiteWash (rect);
   // Do the off-line drawing
   Blitter bltBkg (_background);
   bltBkg.BlitTo (bmpCanvas);


   /*int xRange = (_cx - _heartSprite->getWidth()) /2;
   int yRange = (_cy - _heartSprite->getHeight()) /2;
   const double speed = 0.008;
   int x = xRange + static_cast<int> (xRange * sin (speed * _tick));
   int y = yRange + static_cast<int> (yRange * cos (4.0 * speed * _tick));
   _heartSprite->setX(x);
   _heartSprite->setY(y);*/

   static bool talking = false;
   static int talkingCounter = 0;
   if(++talkingCounter >= 100)
   {
      talkingCounter = 0;
      talking = !talking;
      _heartSprite->setAnimation( talking ? 2 : 0 );
      if(talking)
      {
         /*int num = rand() % 2;
         BOOL success;
         if(num % 2 == 0)
            success = PlaySound( WAV_ILOVEYOU, NULL, SND_FILENAME | SND_ASYNC );
         else
            success = PlaySound( WAV_VALENTINE, NULL, SND_FILENAME | SND_ASYNC );*/
      }
   }
   _heartSprite->animate();
   _heartSprite->render(bmpCanvas);

   _bubbleSprite->setVisiblity(talking);
   _bubbleSprite->setX(_heartSprite->getX() + 120);
   _bubbleSprite->setY(_heartSprite->getY() + 70);
   _bubbleSprite->animate();
   _bubbleSprite->render(bmpCanvas);

   unsigned int numBugs = _bugSprites.size();
   for(unsigned int i=0; i<numBugs; ++i)
   {
      int modX = rand()%5 -2;
      int modY = rand()%5 -2;
      _bugSprites[i]->setX( _bugSprites[i]->getX() + modX );
      _bugSprites[i]->setY( _bugSprites[i]->getY() + modY );
      _bugSprites[i]->animate();
      _bugSprites[i]->render(bmpCanvas);
   }


   // update the buffer
   _bitmapBuf = bmp;
}

void View::Paint (Canvas & canvas)
{
    Blitter blitter (_bitmapBuf);
    int x = (_cx - _widthBkg) / 2;
	int y = (_cy - _heightBkg) / 2;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	blitter.SetDest (x, y);
    blitter.BlitTo (canvas);
}