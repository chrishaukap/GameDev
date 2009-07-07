
#pragma once

#include "World.h"

#include <vector>

typedef std::vector<Renderable*>	RenderList;
typedef std::map<int, RenderList>	RenderLayers;

class RenderableIterator: public std::iterator<std::forward_iterator_tag, Renderable*>
{
public:
	RenderableIterator() : _ptr(NULL)
	{
	}

	Renderable* operator*()
	{
		return (_ptr);
	}

	RenderableIterator& begin();
	RenderableIterator& end();

	bool operator!=( const RenderableIterator& iter ) const;
	RenderableIterator& erase( RenderableIterator& item_to_remove );
	const RenderableIterator& operator++();

private:	
	RenderLayers::iterator layerIterator;
	RenderList::iterator myIterator;
	Renderable *_ptr;
};
