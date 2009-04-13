#ifndef SPRITE_H
#define SPRITE_H

#include "cdhBase.h" 

namespace CDH
{

struct Frame;
class Animation;
class Sprite
{
public:
	Sprite();
	~Sprite();

	CHUint width() const;
	CHUint height() const;

	const Frame& currentFrame() const;
   const Animation* getCurrentAnimation() const;
   const char* getCurrentAnimationName() const {return m_currentAnimation;}

   // adds the new animation to an internal map (mapped to name)
   // takes ownership of animation (which must be valid for the lifetime of this object)
	Status addAnimation(const char* name, Animation* animation);

	Status setCurrentAnimation(const char* name);
   void animate();

private:
	struct Data;
	Data* m_data;
   CHUint m_width;
   CHUint m_height;
   const char* m_currentAnimation;

   Sprite(const Sprite& C);
   const Sprite& operator=(const Sprite& C);
};
}

#endif // SPRITE_H