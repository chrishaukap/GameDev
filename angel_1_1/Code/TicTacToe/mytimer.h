#pragma once
#include "thread.h"

namespace TicTacToe
{
class mytimer
{
public:
	mytimer() : 
		m_tickcount(0)
	{
	}
	~mytimer()
	{
	}
	void start()
	{
		m_tickcount = GetTickCount();
	}
	void end()
	{
	   DWORD finalcount = GetTickCount();
	   m_tickcount = finalcount - m_tickcount;
	}
	DWORD tickCount() const {return m_tickcount;}
private:
	DWORD m_tickcount;
};
}