#ifndef CDHBASE_H
#define CDHBASE_H

enum Status
{
   StatusSuccess = 0,
   StatusFailure ,
   StatusFileNotFound,
   StatusInvalidArgs,
   StatusOutOfMemory,
   StatusBadContext
};

#define StatusFailed(stat) ((stat) == StatusSuccess ? false : true)
#define StatusSucceeded(stat) ((stat) == StatusSuccess ? true : false)


#define PI 3.141592f
#define PIOver2 1.570796f
#define PIOver3 1.047197f
#define PIOver4 0.785398f
#define PIOver5 0.628318f
#define PIOver6 0.523598f
#define PIOver7 0.448798f
#define PIOver8 0.392699f
#define PIOver9 0.349065f
#define PIOver16 0.1963495
#define PIOver32 0.09817475
#define PIOver64 0.049087375
#define PIOver128 0.0245436875
#define PIOver256 0.01227184375
#define TWOPI 6.283184

template<class T>
struct Point
{
   Point(T _x, T _y) : x(_x), y(_y) {}
   T x,y;
};

template<class T>
struct Rect
{
   Rect(const Point<T>& _ul, const Point<T>& _lr) : ul(_ul), lr(_lr) {}
   Point<T> ul, lr;
};

template<class T>
bool RectanglesIntersect(const Rect<T>& r1, const Rect<T>& r2)
{
   if (r1.lr.y < r2.ul.y) return false;
   if (r1.ul.y > r2.lr.y) return false;
   if (r1.lr.x < r2.ul.x) return false;
   if (r1.ul.x > r2.lr.x) return false;   
   return true;
}

template<class T>
bool PointInRect(const Point<T>& p, const Rect<T> r)
{
   if (p.x >= r.ul.x && p.x <= r.lr.x &&
       p.y >= r.ul.y && p.y <= r.lr.y )
       return true;
   return false;
}

#endif //CDHBASE_H
