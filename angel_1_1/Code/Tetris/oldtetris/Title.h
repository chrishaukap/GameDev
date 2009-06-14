#pragma once

#include "t3dlib1.h"
//#include "globals.h"

const int Y_VALUE = 30;
const int MAX_T = 7;
const int MAX_E = 10;
const int MAX_R = 11;
const int MAX_I = 9;
const int MAX_X = 9;
const int MAX_EX = 4;

class CTitle
{
private:
	BOB _t1[MAX_T];
	BOB _e[MAX_E];
	BOB _t2[MAX_T];
	BOB _r[MAX_R];
	BOB _i[MAX_I];
	BOB _x[MAX_X];
	BOB _Ex[MAX_EX];

public:
	CTitle(void);
	~CTitle(void);
	void InitTitle(BOB_PTR block_array);
	void InitT1(BOB_PTR block);
	void InitE(BOB_PTR block);
	void InitT2(BOB_PTR block);
	void InitR(BOB_PTR block);
	void InitI(BOB_PTR block);
	void InitX(BOB_PTR block);	
	void InitEx(BOB_PTR block);
	void DestroyTitle();
	void DrawTitle();
};


CTitle::CTitle(void)
{
}

CTitle::~CTitle(void)
{
}

void CTitle::InitTitle(BOB_PTR block_array)
{
	InitT1( &(block_array[0]) );
	InitE( &(block_array[1]) );
	InitT2( &(block_array[2]) );
	InitR( &(block_array[3]) );
	InitI( &(block_array[2]) );
	InitX( &(block_array[1]) );
	InitEx( &(block_array[0]) );
}

void CTitle::InitT1(BOB_PTR block)
{
	float sx = 80;
	float sy = Y_VALUE;

	// T
	for(int i=0; i< MAX_T; ++i)
	{
		Clone_BOB( block, &_t1[i]);
		Set_Animation_BOB( &(_t1[i]), 1 );
		Set_Anim_Speed_BOB( &(_t1[i]), 15 );
	}
	// row 1
	_t1[0].x = sx;
	_t1[0].y = sy;	
	_t1[1].x = sx + SQUARE_WIDTH;
	_t1[1].y = sy;
	_t1[2].x = sx +2*SQUARE_WIDTH;
	_t1[2].y = sy;
	// row 2
	_t1[3].x = sx + SQUARE_WIDTH;
	_t1[3].y = sy + SQUARE_WIDTH;
	// row 3
	_t1[4].x = sx + SQUARE_WIDTH;
	_t1[4].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_t1[5].x = sx + SQUARE_WIDTH;
	_t1[5].y = sy + 3*SQUARE_WIDTH;
	// row 5
	_t1[6].x = sx + SQUARE_WIDTH;
	_t1[6].y = sy + 4*SQUARE_WIDTH;

}
void CTitle::InitE(BOB_PTR block)
{
	float sx = 150;
	float sy = Y_VALUE;
	
	for(int i=0; i< MAX_E; ++i)
	{
		Clone_BOB( block, &_e[i]);
		Set_Animation_BOB( &(_e[i]), 1 );
		Set_Anim_Speed_BOB( &(_e[i]), 20 );
	}
	// row 1
	_e[0].x = sx;
	_e[0].y = sy;	
	_e[1].x = sx + SQUARE_WIDTH;
	_e[1].y = sy;
	_e[2].x = sx +2*SQUARE_WIDTH;
	_e[2].y = sy;
	// row 2
	_e[3].x = sx;
	_e[3].y = sy + SQUARE_WIDTH;
	// row 3
	_e[4].x = sx;
	_e[4].y = sy + 2*SQUARE_WIDTH;
	_e[5].x = sx + SQUARE_WIDTH;
	_e[5].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_e[6].x = sx;
	_e[6].y = sy + 3*SQUARE_WIDTH;
	// row 5
	_e[7].x = sx;
	_e[7].y = sy + 4*SQUARE_WIDTH;
	_e[8].x = sx + SQUARE_WIDTH;
	_e[8].y = sy + 4*SQUARE_WIDTH;
	_e[9].x = sx + 2*SQUARE_WIDTH;
	_e[9].y = sy + 4*SQUARE_WIDTH;

}
void CTitle::InitT2(BOB_PTR block)
{
	float sx = 220;
	float sy = Y_VALUE;

	// T
	for(int i=0; i< MAX_T; ++i)
	{
		Clone_BOB( block, &_t2[i]);
		Set_Animation_BOB( &(_t2[i]), 1 );
		Set_Anim_Speed_BOB( &(_t2[i]), 25 );
	}
	// row 1
	_t2[0].x = sx;
	_t2[0].y = sy;	
	_t2[1].x = sx + SQUARE_WIDTH;
	_t2[1].y = sy;
	_t2[2].x = sx +2*SQUARE_WIDTH;
	_t2[2].y = sy;
	// row 2
	_t2[3].x = sx + SQUARE_WIDTH;
	_t2[3].y = sy + SQUARE_WIDTH;
	// row 3
	_t2[4].x = sx + SQUARE_WIDTH;
	_t2[4].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_t2[5].x = sx + SQUARE_WIDTH;
	_t2[5].y = sy + 3*SQUARE_WIDTH;
	// row 5
	_t2[6].x = sx + SQUARE_WIDTH;
	_t2[6].y = sy + 4*SQUARE_WIDTH;

}
void CTitle::InitR(BOB_PTR block)
{
	float sx = 290;
	float sy = Y_VALUE;

	for(int i=0; i< MAX_R; ++i)
	{
		Clone_BOB( block, &_r[i]);
		Set_Animation_BOB( &(_r[i]), 1 );
		Set_Anim_Speed_BOB( &(_r[i]), 30 );
	}
	// row 1
	_r[0].x = sx;
	_r[0].y = sy;	
	_r[1].x = sx + SQUARE_WIDTH;
	_r[1].y = sy;
	_r[2].x = sx +2*SQUARE_WIDTH;
	_r[2].y = sy;
	// row 2
	_r[3].x = sx;
	_r[3].y = sy + SQUARE_WIDTH;	
	_r[4].x = sx + 2*SQUARE_WIDTH;
	_r[4].y = sy + SQUARE_WIDTH;
	// row 3
	_r[5].x = sx;
	_r[5].y = sy + 2*SQUARE_WIDTH;	
	_r[6].x = sx + SQUARE_WIDTH;
	_r[6].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_r[7].x = sx;
	_r[7].y = sy + 3*SQUARE_WIDTH;
	_r[8].x = sx + 2*SQUARE_WIDTH;
	_r[8].y = sy + 3*SQUARE_WIDTH;
	// row 5
	_r[9].x = sx;
	_r[9].y = sy + 4*SQUARE_WIDTH;
	_r[10].x = sx + 2*SQUARE_WIDTH;
	_r[10].y = sy + 4*SQUARE_WIDTH;
}
void CTitle::InitI(BOB_PTR block)
{
	float sx = 360;
	float sy = Y_VALUE;

	for(int i=0; i< MAX_I; ++i)
	{
		Clone_BOB( block, &_i[i]);
		Set_Animation_BOB( &(_i[i]), 1 );
		Set_Anim_Speed_BOB( &(_i[i]), 25 );
	}
	// row 1
	_i[0].x = sx;
	_i[0].y = sy;	
	_i[1].x = sx + SQUARE_WIDTH;
	_i[1].y = sy;
	_i[2].x = sx +2*SQUARE_WIDTH;
	_i[2].y = sy;
	// row 2
	_i[3].x = sx + SQUARE_WIDTH;
	_i[3].y = sy + SQUARE_WIDTH;
	// row 3
	_i[4].x = sx + SQUARE_WIDTH;
	_i[4].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_i[5].x = sx + SQUARE_WIDTH;
	_i[5].y = sy + 3*SQUARE_WIDTH;
	// row 5
	_i[6].x = sx;
	_i[6].y = sy + 4*SQUARE_WIDTH;
	_i[7].x = sx + SQUARE_WIDTH;
	_i[7].y = sy + 4*SQUARE_WIDTH;
	_i[8].x = sx + 2*SQUARE_WIDTH;
	_i[8].y = sy + 4*SQUARE_WIDTH;
}
void CTitle::InitX(BOB_PTR block)
{
	float sx = 430;
	float sy = Y_VALUE;
	
	for(int i=0; i< MAX_X; ++i)
	{
		Clone_BOB( block, &_x[i]);
		Set_Animation_BOB( &(_x[i]), 1 );
		Set_Anim_Speed_BOB( &(_x[i]), 20 );
	}
	// row 1
	_x[0].x = sx;
	_x[0].y = sy;	
	_x[1].x = sx + 2*SQUARE_WIDTH;
	_x[1].y = sy;

	// row 2
	_x[2].x = sx;
	_x[2].y = sy + SQUARE_WIDTH;
	_x[3].x = sx + 2*SQUARE_WIDTH;
	_x[3].y = sy + SQUARE_WIDTH;

	// row 3	
	_x[4].x = sx + SQUARE_WIDTH;
	_x[4].y = sy + 2*SQUARE_WIDTH;
	
	// row 4
	_x[5].x = sx;
	_x[5].y = sy + 3*SQUARE_WIDTH;	
	_x[6].x = sx + 2*SQUARE_WIDTH;
	_x[6].y = sy + 3*SQUARE_WIDTH;

	// row 5
	_x[7].x = sx;
	_x[7].y = sy + 4*SQUARE_WIDTH;
	_x[8].x = sx + 2*SQUARE_WIDTH;
	_x[8].y = sy + 4*SQUARE_WIDTH;
}
void CTitle::InitEx(BOB_PTR block)
{
	float sx = 500;
	float sy = Y_VALUE;
	for(int i=0; i< MAX_EX; ++i)
	{
		Clone_BOB( block, &_Ex[i]);
		Set_Animation_BOB( &(_Ex[i]), 1 );
		Set_Anim_Speed_BOB( &(_Ex[i]), 15 );
	}
	// row 1
	_Ex[0].x = sx;
	_Ex[0].y = sy;	
	// row 2
	_Ex[1].x = sx;
	_Ex[1].y = sy + SQUARE_WIDTH;
	// row 3
	_Ex[2].x = sx;
	_Ex[2].y = sy + 2*SQUARE_WIDTH;
	// row 4
	_Ex[3].x = sx;
	_Ex[3].y = sy + 4*SQUARE_WIDTH;
}


// destroys all BOBs in the tetrix Title
void CTitle::DestroyTitle()
{
	for(int i=0; i< MAX_T; ++i)
	{	
		Destroy_BOB( &(_t1[i]) );
		Destroy_BOB( &(_t2[i]) );
	}
	for(int i=0; i< MAX_E; ++i)
		Destroy_BOB( &(_e[i]) );
	for(int i=0; i< MAX_R; ++i)
		Destroy_BOB( &(_r[i]) );
	for(int i=0; i< MAX_I; ++i)
		Destroy_BOB( &(_i[i]) );
	for(int i=0; i< MAX_X; ++i)
		Destroy_BOB( &(_x[i]) );
	for(int i=0; i< MAX_EX; ++i)
		Destroy_BOB( &(_Ex[i]) );
}
void CTitle::DrawTitle()
{
	// draw the 2 't's
	for( int i=0; i<MAX_T; ++i )
	{
		Animate_BOB( &(_t1[i]) );
		Draw_BOB16( &(_t1[i]), lpddsback);
		Animate_BOB( &(_t2[i]) );
		Draw_BOB16( &(_t2[i]), lpddsback);
	}
	// draw e
	for( int i=0; i<MAX_E; ++i )
	{
		Animate_BOB( &(_e[i]) );
		Draw_BOB16( &(_e[i]), lpddsback);
	}
	// draw r
	for( int i=0; i<MAX_R; ++i )
	{
		Animate_BOB( &(_r[i]) );
		Draw_BOB16( &(_r[i]), lpddsback);
	}
	// draw i
	for( int i=0; i<MAX_I; ++i )
	{
		Animate_BOB( &(_i[i]) );
		Draw_BOB16( &(_i[i]), lpddsback);
	}
	// draw s
	for( int i=0; i<MAX_X; ++i )
	{
		Animate_BOB( &(_x[i]) );
		Draw_BOB16( &(_x[i]), lpddsback);
	}
	// draw !
	for( int i=0; i<MAX_EX; ++i )
	{
		Animate_BOB( &(_Ex[i]) );
		Draw_BOB16( &(_Ex[i]), lpddsback);
	}
}