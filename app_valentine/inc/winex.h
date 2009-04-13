#if !defined WINEX_H
#define WINEX_H
//------------------------------------
//  winex.h
//  Windows Exception
//  (c) Reliable Software, 1997, 98
//------------------------------------
#include <windows.h>

// out of memory handler that throws WinException
int NewHandler (size_t size);

class WinException
{
public:
    WinException (char const * msg, char const * object = 0)
    : _err (GetLastError()), _msg(msg), _obj (object)
    {}
    DWORD GetError() const { return _err; }
    char const * GetMessage () const { return _msg; }
	char const * GetObject () const { return _obj; }
private:
    DWORD  _err;
    char const * _msg;
	char const * _obj;
};

#endif