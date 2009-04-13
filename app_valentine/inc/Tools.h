#if !defined (CANVTOOLS_H)
#define CANVTOOLS_H
//-----------------------------
// (c) Reliable Software 1997, 99
//-----------------------------

#include <windows.h>

class StockObject
{
public:
    StockObject (HDC hdc, int type)
      : _hdc (hdc)
    {
        _hObjOld = ::SelectObject (_hdc, GetStockObject (type));
    }

    ~StockObject ()
    {
        ::SelectObject (_hdc, _hObjOld);
    }
private:
    HGDIOBJ  _hObjOld;
    HDC      _hdc;
};

class WhitePen : public StockObject
{
public:
    WhitePen (HDC hdc): StockObject (hdc, WHITE_PEN) {}
};

class BlackPen : public StockObject
{
public:
    BlackPen (HDC hdc): StockObject (hdc, BLACK_PEN) {}
};

class Pen
{
public:
    Pen (COLORREF color, int style = PS_SOLID)
    {
        _hPen = ::CreatePen (style, 0, color);
    }
    ~Pen ()
    {
        ::DeleteObject (_hPen);
    }
    operator HPEN () { return _hPen; }
private:
    HPEN    _hPen;
};

class PenHolder
{
public:
    PenHolder (HDC hdc, HPEN hPen)
        : _hdc (hdc)
    {
        _hPenOld = (HPEN) ::SelectObject (_hdc, hPen); 
    }
    ~PenHolder ()
    {
        ::SelectObject (_hdc, _hPenOld);
    }
private:
    HDC     _hdc;
    HPEN    _hPenOld;
};

class ColorPen
{
public:
    ColorPen (HDC hdc, COLORREF color)
        : _hdc (hdc)
    {
        _hPen = ::CreatePen (PS_SOLID, 0, color);
        _hPenOld = (HPEN)::SelectObject (_hdc, _hPen); 
    }
    ~ColorPen ()
    {
        ::SelectObject (_hdc, _hPenOld);
        ::DeleteObject (_hPen);
    }
private:
    HDC     _hdc;
    HPEN    _hPen;
    HPEN    _hPenOld;
};

class Brush
{
public:
    Brush (COLORREF color)
    {
        _hBrush = ::CreateSolidBrush (color);
    }
	Brush (HBITMAP hBmp)
	{
		_hBrush = ::CreatePatternBrush (hBmp);
	}
    ~Brush ()
    {
        ::DeleteObject (_hBrush);
    }
    operator HBRUSH () const { return _hBrush; }
private:
    HBRUSH  _hBrush;
};

class BrushHolder
{
public:
    BrushHolder (HDC hdc, HBRUSH hBrush)
        : _hdc (hdc)
    {
        _hBrushOld = (HBRUSH) ::SelectObject (_hdc, hBrush); 
    }
    ~BrushHolder ()
    {
        ::SelectObject (_hdc, _hBrushOld);
    }
private:
    HDC     _hdc;
    HBRUSH  _hBrushOld;
};

class ColorBrush
{
public:
    ColorBrush (HDC hdc, COLORREF color)
        : _hdc (hdc)
    {
        _hBrush = ::CreateSolidBrush (color);
        _hBrushOld = (HBRUSH)::SelectObject (_hdc, _hBrush); 
    }
    ~ColorBrush ()
    {
        ::SelectObject (_hdc, _hBrushOld);
        ::DeleteObject (_hBrush);
    }
    operator HBRUSH () const { return _hBrush; }
private:
    HDC     _hdc;
    HBRUSH  _hBrush;
    HBRUSH  _hBrushOld;
};

class WhiteBrush : public StockObject
{
public:
    WhiteBrush (HDC hdc): StockObject (hdc, WHITE_BRUSH) {}
};

class BlackBrush : public StockObject
{
public:
    BlackBrush (HDC hdc): StockObject (hdc, BLACK_BRUSH) {}
};

class TextColor
{
public:
    TextColor (HDC hdc, COLORREF color)
        : _hdc (hdc) 
    {
        _prevColor = ::SetTextColor (_hdc, color);
    }
    ~TextColor ()
    {
        ::SetTextColor (_hdc, _prevColor);
    }
private:
    HDC         _hdc;
    COLORREF    _prevColor;
};

class BackgroundColor
{
public:
    BackgroundColor (HDC hdc, COLORREF color)
        : _hdc (hdc) 
    {
        _prevColor = ::SetBkColor (_hdc, color);
    }
    ~BackgroundColor ()
    {
        ::SetBkColor (_hdc, _prevColor);
    }
private:
    HDC         _hdc;
    COLORREF    _prevColor;
};

class TransparentBackground
{
public:
    TransparentBackground (HDC hdc)
        : _hdc (hdc) 
    {
        _prevMode = ::SetBkMode (_hdc, TRANSPARENT);
    }
    ~TransparentBackground ()
    {
        ::SetBkMode (_hdc, _prevMode);
    }
private:
    HDC     _hdc;
    int     _prevMode;
};

class ModeSetter
{
public:
    ModeSetter (HDC hdc, int mode = R2_COPYPEN)
    : _hdc (hdc)
    {
        _modeOld = ::GetROP2 (_hdc);
        ::SetROP2 (_hdc, mode);
    }
    ~ModeSetter ()
    {
        ::SetROP2 (_hdc, _modeOld);
    }
private:
    HDC _hdc;
    int _modeOld;
};

class ClientRect: public RECT
{
public:
    ClientRect (HWND hwnd)
    {
        ::GetClientRect (hwnd, this);
    }
};

#endif