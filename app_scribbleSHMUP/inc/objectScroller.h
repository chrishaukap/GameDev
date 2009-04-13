#ifndef OBJECTSCROLLER
#define OBJECTSCROLLER

namespace CDH
{

class SpatialObject;
class ObjectScroller
{
public:
	enum Direction
	{
		Left,
		Right
	};
   ObjectScroller();
   ~ObjectScroller();

	void setScrollRate(float rate);
	void setDirectionToScroll(Direction direction);
   void clear();
	void addObject(SpatialObject* objectToScroll);
   void process();
   
private:
	struct Data;
	Data* m_data;

   ObjectScroller(const ObjectScroller& C);
   const ObjectScroller& operator=(const ObjectScroller& C);
};

}

#endif // OBJECTSCROLLER

