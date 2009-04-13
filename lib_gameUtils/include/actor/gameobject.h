#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "actor/spatialObject.h"
#include "sprite/renderable.h"
#include "actor/collidable.h"
#include "math/vector3d.h"

namespace CDH
{

class Projectile;
class ForegroundObject;
class CharacterBase;
class GameObject : public SpatialObject,
						 public Renderable,
						 public Collidable
{
public:   
	enum Facing
	{
		FacingLeft = 0,
		FacingRight,
		FacingInvalid
	};

   GameObject(	const Vector3& position, 
					const Vector3& rotation, 
					const Rectangle& polygon,
					CollidableType type);  
   virtual ~GameObject();

   virtual Status process() = 0;
	Facing facing() const {return m_facing;}
	void setFacing(Facing facing) {m_facing = facing;}

   bool isEnemy() const {return m_isEnemy;}


   const Vector3& oldPosition() const {return m_oldPosition;}
protected:
   Vector3 m_oldPosition;
	Facing m_facing;
   bool m_isEnemy;  // HAUKAP - one hack begets another!

   void setEnemy(bool isEnemy) {m_isEnemy = isEnemy;}

private:
	GameObject();
   GameObject(const GameObject& C);
   const GameObject& operator=(const GameObject& C);
};


}
#endif //CHARACTERBASE_H
