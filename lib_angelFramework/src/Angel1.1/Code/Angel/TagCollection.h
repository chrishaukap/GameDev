#pragma once

#include "Actor.h"

#include <set>
#include <map>

#define tagList TagCollection::GetInstance()

class TagCollection
{
public:
	static TagCollection& GetInstance();
	ActorSet GetObjectsTagged(String findTag);
	StringSet GetTagList();

	void AddObjToTagList(Actor* obj, String tag);
	void RemoveObjFromTagList(Actor* obj, String tag);

protected:
	TagCollection();
	static TagCollection* s_TagCollection;

private:
	std::map<String, ActorSet> _tagMappings;
};
