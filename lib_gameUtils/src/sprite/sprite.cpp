#include <assert.h>
#include <map>

#include "sprite/sprite.h"
#include "sprite/animation.h"
#include "string.h"

using namespace CDH;
typedef std::map<const char*, Animation* > ANIMATIONMAP;

struct Sprite::Data
{
   
   Data() : animations() {}
   ~Data()
   {
      ANIMATIONMAP::iterator iter = animations.begin();
      while( iter != animations.end() )
      {
         delete iter->first;
			++iter;
      }
      animations.clear();
   }
   ANIMATIONMAP animations;
};

Sprite::Sprite() : 
   m_data(NULL),
   m_width(0),
   m_height(0),
   m_currentAnimation(NULL)
{
   m_data = new Data();
}
Sprite::~Sprite()
{
   delete m_data;
}

CHUint 
Sprite::width() const
{
   return m_width;
}
CHUint 
Sprite::height() const
{
   return m_height;
}

const Frame& 
Sprite::currentFrame() const
{
   assert(m_data != NULL && m_currentAnimation != NULL);
   return m_data->animations[m_currentAnimation]->getCurrentFrame();
}

Status 
Sprite::addAnimation(const char* name, Animation* anim)
{
   assert(m_data != NULL);
   char* animName = _strdup(name);
   if(animName == NULL)
      return StatusOutOfMemory;

   m_data->animations[animName] = anim;
	if(m_currentAnimation == NULL)
		m_currentAnimation = animName;
   
   return StatusSuccess;
}

Status 
Sprite::setCurrentAnimation(const char* name)
{
   assert( m_data != NULL );
   assert( name && *name );
   if(!name || !*name)
      return StatusInvalidArgs;

   Status stat = StatusFailure;

   ANIMATIONMAP::iterator iter = m_data->animations.begin();
   while( iter != m_data->animations.end() )
   {
      if(0 == strcmp(name, iter->first))
      {
         m_currentAnimation = iter->first;
         stat = StatusSuccess;
         break;
      }
		++iter;
   }

   return stat;
}

const Animation*
Sprite::getCurrentAnimation() const
{
   assert(m_data != NULL && m_currentAnimation != NULL);
   return m_data->animations[m_currentAnimation];
}

void 
Sprite::animate()
{   
   assert(m_data != NULL && m_currentAnimation != NULL);
   m_data->animations[m_currentAnimation]->animate();
}
