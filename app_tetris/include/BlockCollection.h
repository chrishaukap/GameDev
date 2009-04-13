#ifndef BLOCKCOLLECTION_H
#define BLOCKCOLLECTION_H

#include "block.h"
#include <list>
using namespace std;

#define SUBTRACT_BOOL false
#define ADD_BOOL true
#define CASCADE 2

struct COLLECTION_SQUARE
{
	BLOCK_PTR block;
	int empty;
};

class Logger;

class BlockCollection
{
public:
	BlockCollection(const Logger& logger);
	~BlockCollection();
	void DeleteCollection();
	void CreateCollection();
	bool Collisions(BLOCK_PTR block, int direction);
	bool CollisionLeft( BLOCK_PTR block );
	bool CollisionRight( BLOCK_PTR block );
	bool CollisionDown( BLOCK_PTR block );
	bool CollisionUp( BLOCK_PTR block );
	bool CollisionCollection( BLOCK_PTR block );
	int AddBlock(BLOCK_PTR block);
	void Draw();
	void Animate();
	bool IsRowComplete(int y);
	void Cascade();
	void DoDeletion(BLOCK_PTR block, int x, int y);
	void AddSplitBlock(BLOCK_PTR new_block, BLOCK_PTR block);
	void RemoveBlock(BLOCK_PTR block);
	void AddRowToDelete(int row);
	bool RowHasBeenCompleted() const {return m_rowCompleted;}   
	size_t GetSize() const           {return  m_collection.size();}
	int PrepRowDelete();
	void SetAnimation(int index);	
	
private:
	
	COLLECTION_SQUARE	         m_collisionArray[NUM_X_POSITIONS][NUM_Y_POSITIONS];
	list<BLOCK_PTR>		      m_collection;
	int					         m_completionArray[8];
	list<BLOCK_PTR>::iterator  m_collectionIter;
	bool m_rowCompleted;
   const Logger& m_logger;

	// helper functions
	bool AllSquaresDeleted();
	int GetLowestRow();
	void MakeBlocksFall();
	void ResetCompletionArray();
	void AdjustCollisionArray( BLOCK_PTR block, bool addOrSubtract);
};

#endif // BLOCKCOLLECTION_H
