#ifndef ANIMATION_H
#define ANIMATION_H

#include "cdhBase.h" 

namespace CDH
{

struct Frame
{
	Frame(RES_HANDLE theimg, RES_HANDLE themask) :
		img(theimg), mask(themask)
	{}
	~Frame(){}
	RES_HANDLE img;
	RES_HANDLE mask;
};

class Animation
{
public:

	Animation(CHUint animSpeed);
	~Animation();

	void pushFrameAtEnd(const Frame& frame);
	const Frame& getCurrentFrame() const;
	void animate();

private:
	CHUint m_curIndex;
	CHUint m_animSpeed;
	CHUint m_speedCounter;
	struct AnimFrames;
	AnimFrames *m_frames;

	void advanceFrame();

   Animation(const Animation& C);
   const Animation& operator=(const Animation& C);
};
}

#endif // ANIMATION_H