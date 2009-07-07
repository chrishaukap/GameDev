#include "stdafx.h"
#include "TagCollection.h"


TagCollection* TagCollection::s_TagCollection = NULL;

TagCollection& TagCollection::GetInstance()
{
	if (s_TagCollection == NULL)
	{
		s_TagCollection = new TagCollection();
	}
	return *s_TagCollection;
}

TagCollection::TagCollection()
{

}

ActorSet TagCollection::GetObjectsTagged(String findTag)
{
	std::map<String, ActorSet>::iterator it = _tagMappings.find(findTag);
	if (it != _tagMappings.end())
	{
		return it->second;
	}
	else
	{
		return ActorSet();
	}
}

StringSet TagCollection::GetTagList()
{
	StringSet forReturn;
	std::map<String, ActorSet>::iterator it = _tagMappings.begin();
	while (it != _tagMappings.end())
	{
		forReturn.insert(it->first);
		it++;
	}

	return forReturn;
}


void TagCollection::AddObjToTagList(Actor* obj, String tag)
{
	_tagMappings[tag].insert(obj);
}

void TagCollection::RemoveObjFromTagList(Actor* obj, String tag)
{
	_tagMappings[tag].erase(obj);
	if (_tagMappings[tag].empty())
	{
		_tagMappings.erase(tag);
	}
}