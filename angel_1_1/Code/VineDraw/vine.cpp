#include "StdAfx.h"

#include "vine.h"
#include "MathUtil.h"
#include "VNode.h"

#include <list>
#include <queue>

class Vine::Data
{
public:
	Data() : 
		root(new VNode(Vector2(0.0f, 0.0f), Vector2(1,0))), 
		shoots(), points()
	{}
	~Data()
	{
		delete root; root = NULL;

		// HAUKAP - make sure no leak on queue
		while(!points.empty())
			points.pop();
		shoots.clear();
	}
	
	void cleanseChildren(VINENODES& newChildren, VINENODES& staleChildren)
	{
		removeStaleChildren(staleChildren);
		addNewChildren(newChildren);
	}
	void growShoots()
	{
		bool reachedTarget = false;
		VINENODES newChildren;
		VINENODES staleChildren;

		std::list<VNode*>::iterator iter = shoots.begin();
		while( iter != shoots.end() )
		{
			(*iter)->grow( *points.front(), reachedTarget, newChildren, staleChildren );			
			++iter;
		}

		if(reachedTarget)
			points.pop();
		cleanseChildren(newChildren, staleChildren);
	}
	VNode* root;
	std::list<VNode*> shoots;
	std::queue<Vector2*> points;
private:
	void removeStaleChildren(VINENODES& children)
	{
		// HAUKAP - may be slow if leaves gets huge
		int sz = (int)children.size();
		for( int i=0; i<sz; ++i )
			shoots.remove(children[i]);
	}
	void addNewChildren(VINENODES& children)
	{
		int sz = (int)children.size();
		for( int i=0; i<sz; ++i )
			shoots.push_back(children[i]);
	}
};

Vine::Vine() : 
	_vine(new Data())
{
	createPointList();
	_vine->shoots.push_back(_vine->root);
}
Vine::~Vine()
{
	delete _vine; _vine = NULL;
}
void Vine::update()
{
	_vine->growShoots();
}
void Vine::render() const
{
	_vine->root->render();
}
void Vine::createPointList()
{
	_vine->points.push(new Vector2(2.0f, 2.0f));
	_vine->points.push(new Vector2(-2.0f, 2.0f));
	_vine->points.push(new Vector2(-2.0f, -2.0f));
	_vine->points.push(new Vector2(3.0f, -2.0f));

	_vine->points.push(new Vector2(3.0f, 3.0f));
	_vine->points.push(new Vector2(-3.0f, 3.0f));
	_vine->points.push(new Vector2(-3.0f, -3.0f));
	_vine->points.push(new Vector2(4.0f, -3.0f));

	_vine->points.push(new Vector2(4.0f, 4.0f));
	_vine->points.push(new Vector2(-4.0f, 4.0f));
	_vine->points.push(new Vector2(-4.0f, -4.0f));
	_vine->points.push(new Vector2(5.0f, -4.0f));

	_vine->points.push(new Vector2(5.0f, 5.0f));
	_vine->points.push(new Vector2(-5.0f, 5.0f));
	_vine->points.push(new Vector2(-5.0f, -5.0f));
	_vine->points.push(new Vector2(6.0f, -5.0f));

	_vine->points.push(new Vector2(6.0f, 6.0f));
	_vine->points.push(new Vector2(-6.0f, 6.0f));
	_vine->points.push(new Vector2(-6.0f, -6.0f));
	_vine->points.push(new Vector2(7.0f, -6.0f));
   
	_vine->points.push(new Vector2(7.0f, 7.0f));
	_vine->points.push(new Vector2(-7.0f, 7.0f));
	_vine->points.push(new Vector2(-7.0f, -7.0f));
	_vine->points.push(new Vector2(8.0f, -7.0f));
   
	_vine->points.push(new Vector2(8.0f, 8.0f));
	_vine->points.push(new Vector2(-8.0f, 8.0f));
	_vine->points.push(new Vector2(-8.0f, -8.0f));
	_vine->points.push(new Vector2(9.0f, -8.0f));
   
	_vine->points.push(new Vector2(9.0f, 9.0f));
	_vine->points.push(new Vector2(-9.0f, 9.0f));
	_vine->points.push(new Vector2(-9.0f, -9.0f));
	_vine->points.push(new Vector2(10.0f, -9.0f));
   
	_vine->points.push(new Vector2(10.0f, 10.0f));
	_vine->points.push(new Vector2(-10.0f, 10.0f));
	_vine->points.push(new Vector2(-10.0f, -10.0f));
	_vine->points.push(new Vector2(11.0f, -10.0f));
}