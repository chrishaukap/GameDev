#ifndef TETRISBLOCK_H
#define TETRISBLOCK_H

#include "cdhBase.h"

namespace CDH
{
namespace Tetris
{

class TetrisBlock
{
public:
	TetrisBlock();     
	~TetrisBlock();

private:
   CHUint m_gridRow;
};

}
}
#endif // TETRISBLOCK_H