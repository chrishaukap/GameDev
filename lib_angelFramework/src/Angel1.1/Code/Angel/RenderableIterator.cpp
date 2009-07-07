//#pragma once

#include "StdAfx.h"

#include "World.h"
#include "RenderableIterator.h"

RenderableIterator& RenderableIterator::begin()
{
	layerIterator = theWorld.GetLayers().begin();
	myIterator = layerIterator->second.begin();
	_ptr = *myIterator;
	return *this;
}

RenderableIterator& RenderableIterator::end()
{
	_ptr = 0;
	return *this;
}

bool RenderableIterator::operator!=( const RenderableIterator& iter ) const
{
	return ( _ptr != iter._ptr );
}

RenderableIterator& RenderableIterator::erase( RenderableIterator& item_to_remove )
{
	Renderable* pValueToErase = *item_to_remove;

	RenderLayers::iterator layerElements = theWorld.GetLayers().find( pValueToErase->GetLayer() );
	if (layerElements != theWorld.GetLayers().end())
	{
		RenderList::iterator element = (*layerElements).second.begin();

		while ( element != (*layerElements).second.end() )
		{
			if ( *element == pValueToErase)
			{					
				myIterator = (*layerElements).second.erase(element);

				// Have to force to next layer.
				// Let our ++ operator do it's magic.
				if ( myIterator == (*layerElements).second.end() )
				{
					++item_to_remove;
				}
				else
				{
					_ptr = *myIterator;
				}

				return *this;
			}
			++element;
		}
	}
	return *this;
}

const RenderableIterator& RenderableIterator::operator++()
{
	if (myIterator == layerIterator->second.end())
	{
		if (layerIterator == theWorld.GetLayers().end())
		{
			_ptr = 0;
			return *this;
		}
	}
	else
		++myIterator;

	// See if we've reached the end of the vector for the current layer.
	if (myIterator == layerIterator->second.end())
	{
		++layerIterator;
		// Go through layers until you reach the end or a non-empty layer.
		while (layerIterator != theWorld.GetLayers().end() && layerIterator->second.size() == 0)
			++layerIterator;

		if (layerIterator != theWorld.GetLayers().end() && layerIterator->second.size() > 0)
		{	
			myIterator = layerIterator->second.begin();
		}
		else
			_ptr = NULL;
	}
	if (_ptr)
		_ptr = *myIterator;

	return *this;
}