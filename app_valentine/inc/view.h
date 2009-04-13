#if !defined VIEW_H
#define VIEW_H
//------------------------------------
//  view.h
//  View
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include "inc/bitmap.h"
#include "inc/Sprite.h"
#include <windows.h>

#include <vector>

class Canvas;

class View
{
public:
    View (HINSTANCE hInst);
    ~View();

    void SetSize (int cxNew, int cyNew)
    {
        _cx = cxNew;
        _cy = cyNew;
    }
    void Step () { ++_tick; }
    void Update (Canvas & canvas);
    void Paint (Canvas & canvas);

    Sprite* heart() const {return _heartSprite;}
protected:
    int     _cx, _cy;
    int     _tick;
    Bitmap  _bitmapBuf;

    Bitmap  _background;
    int     _widthBkg, _heightBkg;
    Bitmap  _sprite;
    Bitmap  _mask;
    int     _widthSprite, _heightSprite;

    Sprite* _heartSprite;
    Sprite* _bubbleSprite;
    std::vector<Sprite*> _bugSprites;
};

#endif
