#ifndef PLATFORM_H
#define PLATFORM_H

#include "actor/foregroundObject.h"

namespace CDH
{

class Platform : public ForegroundObject
{
public:
   Platform(const Vector3& position, 
			   const Vector3& rotation, 						  
			   const Rectangle& polygon);  
   virtual ~Platform();

	Status render() const;
private:

	Platform();
   Platform(const Platform& C);
   const Platform& operator=(const Platform& C);
};


}
#endif //PROJECTILE_h
