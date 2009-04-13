#include <assert.h>
#include <vector>

#include "sprite/animation.h"
#include "resource/resourceLibrary.h"

using namespace CDH;

struct Animation::AnimFrames
{
	AnimFrames() : frames() {}
	~AnimFrames() {frames.clear();}
	std::vector<Frame> frames;
};

Animation::Animation(CHUint animSpeed) :

	m_curIndex(0),
	m_animSpeed(animSpeed),
	m_speedCounter(0),
	m_frames(NULL)
{
	m_frames = new AnimFrames();
}
Animation::~Animation()
{
	delete m_frames;
}

void 
Animation::pushFrameAtEnd(const Frame& frame)
{
	assert(m_frames != NULL);
	m_frames->frames.push_back(frame);
}

const Frame& 
Animation::getCurrentFrame() const
{
	assert(m_frames != NULL);
	return m_frames->frames[m_curIndex];
}

void Animation::advanceFrame()
{
	++m_curIndex;
	if(m_curIndex >= (CHUint)m_frames->frames.size())
		m_curIndex = 0;
}

void Animation::animate()
{
	if( ++m_speedCounter > m_animSpeed )
	{
		advanceFrame();
		m_speedCounter = 0;
	}
}