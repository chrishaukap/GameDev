#include "dispatcherTest.h"


using namespace CDH;

class shape
{
public:
   virtual ~shape(){}
   IMPLEMENT_INDEXABLE_CLASS(shape)
};
class square : public shape
{
public:
   virtual ~square() {}  
   IMPLEMENT_INDEXABLE_CLASS(square)
};

class triangle : public shape
{
public:
	virtual ~triangle() {}
   IMPLEMENT_INDEXABLE_CLASS(triangle)
};

class circle : public shape
{
public:
	virtual ~circle() {}
   IMPLEMENT_INDEXABLE_CLASS(circle)
};
class big_circle : public circle
{
public:
	virtual ~big_circle() {}
   IMPLEMENT_INDEXABLE_CLASS(big_circle)
};
class triangle_circle : public circle, public triangle
{
public:
	virtual ~triangle_circle() {}
   IMPLEMENT_INDEXABLE_CLASS(triangle_circle)
};
void CollisionBigCircleTriangle(shape& _bigCircle, shape& _triangle)
{
   fprintf(stderr, "_bigCircle _triangle\n");
}

void CollisionSquareTriangle(shape& _square, shape& _triangle)
{
   fprintf(stderr, "_square _triangle\n");
}
void CollisionSquareSquare(shape& _square1, shape& _square2)
{
   fprintf(stderr, "_square1 _square2\n");
}
void CollisionTriangleTriangle(shape& _triangle1, shape& _triangle2)
{
   fprintf(stderr, "_triangle1 _triangle2\n");
}
void CollisionCircleTriangle(shape& _circle, shape& _triangle)
{
   fprintf(stderr, "_circle _triangle\n");
}

DispatcherTest::DispatcherTest(){}
DispatcherTest::~DispatcherTest(){}

void
DispatcherTest::run()
{
   square _sq;
   triangle _tri, _tri2;
   circle _circ;
   shape _shape;
	big_circle _bigCircle;
   
   BasicFastDispatcher<shape,shape> dispatcher;
   dispatcher.Add<big_circle,triangle>(CollisionBigCircleTriangle);
   dispatcher.Add<square,square>(CollisionSquareSquare);
   dispatcher.Add<square,triangle>(CollisionSquareTriangle);
   dispatcher.Add<circle,triangle>(CollisionCircleTriangle);
   dispatcher.Add<triangle,triangle>(CollisionTriangleTriangle);
   dispatcher.Go(_sq, _tri);   
   dispatcher.Go(_tri, _tri2);   
   dispatcher.Go(_circ, _tri);   
   dispatcher.Go(_sq, _sq);   
   dispatcher.Go(_circ, _circ);   
   dispatcher.Go(_bigCircle, _tri);   

}