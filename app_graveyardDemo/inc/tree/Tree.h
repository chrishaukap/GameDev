#ifndef TREE_H
#define TREE_H

#include "defines.h"
#include "vector3d.h"
#include <vector>

class MoveableObject;
class Limb;
class Tree
{
public:
   Tree(const Vector3& position);
   ~Tree();

   void pursue(MoveableObject& moveableObject);
   bool canPursue() const;
   void process(bool &targetAquired);
   void render() const;
   
   const Vector3& position() const {return m_position;}

private:
   std::vector<Limb*> m_limbs;
   Vector3 m_position;
};

#endif // TREE_H