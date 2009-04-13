#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "cdhBase.h"
#include "renderable.h"

namespace CDH
{

class Background : public Renderable
{
public:
   Background();
   ~Background();

	void setBackground(RES_HANDLE bg, float x, float y, float rate);

	Status render() const;
   
private:
	RES_HANDLE m_background;
	float m_x, m_y; // position of the lower left corner of the background
   float m_rate;

   Background(const Background& C);
   const Background& operator=(const Background& C);
};

}

#endif // BACKGROUND_H