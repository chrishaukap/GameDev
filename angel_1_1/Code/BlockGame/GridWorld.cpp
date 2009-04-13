#include "stdafx.h"
#include "GridWorld.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum FileReaderState
{
	READER_GET_ASSETS,
	READER_GET_WORLD,
	READER_DONE
};

#define MAX_STRING_LENGTH	50

bool IsAGroundForInit(Object *obj)
{
	return ((!obj->IsPortal()) && (!obj->IsMovable())) && (obj->IsLight() || obj->IsNull());
}

GridWorld::GridWorld(char *fileDir)
{
	mIsValid = false;
	FILE *pFile;

	if (fopen_s(&pFile, fileDir, "r") != 0)
	{
		return;
	}
	if (pFile != NULL)
	{
		FileReaderState readerState = READER_GET_ASSETS;
		char playerIcon = '\0';

		char input[MAX_STRING_LENGTH];
		fgets(input, MAX_STRING_LENGTH, pFile);
		mBackground.mFilename = input;

		Object *nullObj = new Object(' ', NULL);
		nullObj->SetProperty(IS_NULL);
		mWorldTiles.push_back(nullObj);

		Object *lightObj = new Object(',', NULL);
		lightObj->SetProperty(IS_LIGHT|IS_NULL);
		mWorldTiles.push_back(lightObj);

		Object *newlineObj = new Object('\n', NULL);
		nullObj->SetProperty(0x0);
		mWorldTiles.push_back(newlineObj);

		while (!feof(pFile) && (readerState == READER_GET_ASSETS))
		{
			fgets(input, MAX_STRING_LENGTH, pFile);
			char *context;
			char *tok1 = strtok_s(input, ":\n", &context);
			char *tok2 = strtok_s(NULL, ":\n", &context);

			if (tok1[0] == '%') // escape character!
			{
				readerState = READER_GET_WORLD;
			}
			else
			{
				Object *newObj = new Object(tok1[0], tok2);
				tok1 = strtok_s(NULL, ":\n", &context);
				long propTags = atoi(tok1);

				if ((propTags & IS_PLAYER) != 0)
				{
					mpPlayerTile = new Player(newObj->mIcon, newObj->mAsset.mFilename);
					mpPlayerTile->SetProperty(IS_PLAYER);
					playerIcon = newObj->mIcon;			
					mWorldTiles.push_back(mpPlayerTile);

					delete newObj;
				}
				else
				{
					newObj->SetProperty(propTags);
					mWorldTiles.push_back(newObj);
				}
				printf("[GridWorld] - %s \t0x%08x\n", input, propTags);
			}

		}
		assert(!feof(pFile));
		assert(mpPlayerTile != NULL);

		char c;
		int xPos = 0;
		int yPos = 0;
		mGrid[xPos][yPos] = new Tile();
		while ((readerState == READER_GET_WORLD) && ((c = fgetc(pFile)) != EOF))
		{
			if (c == '\n')
			{
				assert(xPos < MAX_WORLD_WIDTH);
				mGrid[xPos][yPos]->mpGround = FindObjectInList('\n');
				mGrid[xPos][yPos]->mpThing = FindObjectInList('\n');
				printf("%c", c);
				yPos++;
				xPos = 0;
			}
			else
			{
				assert(xPos < MAX_WORLD_HEIGHT);
				printf("%c", c);
				if (c == playerIcon)
				{
					mpPlayerTile->xCoord = xPos;
					mpPlayerTile->yCoord = yPos;
					mGrid[xPos][yPos]->mpGround = FindObjectInList(' ');
					mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
				}
				else
				{
					Object *o = FindObjectInList(c);
					if (IsAGroundForInit(o))
					{
						mGrid[xPos][yPos]->mpGround = o;
						mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
					}
					else
					{
						mGrid[xPos][yPos]->mpGround = FindObjectInList(' ');
						mGrid[xPos][yPos]->mpThing = o;
					}
				}
				xPos++;	
			}
			mGrid[xPos][yPos] = new Tile();
		}
		mGrid[xPos][yPos]->mpGround = FindObjectInList('\n');
		mGrid[xPos][yPos]->mpThing = FindObjectInList('\n');

		fclose(pFile);
		mIsValid = true;
	}
}

bool GridWorld::ICanMove(MoveDir dir, int xPos, int yPos, int boxDepth)
{
	switch (dir)
	{
	case UP:
		if (mGrid[xPos][yPos-1]->mpThing->IsNull())
		{	
			return true;
		}
		else if (mGrid[xPos][yPos-1]->mpThing->IsFriendly())
		{
			return ICanMove(UP, xPos, yPos-1, boxDepth);
		}
		else if (mGrid[xPos][yPos-1]->IsMovable() && boxDepth < 1)
		{
			boxDepth++;
			return ICanMove(UP, xPos, yPos-1, boxDepth);
		}
		break;
	case DOWN:
		if (mGrid[xPos][yPos+1]->mpThing->IsNull())
		{	
			return true;
		}
		else if (mGrid[xPos][yPos+1]->mpThing->IsFriendly())
		{
			return ICanMove(DOWN, xPos, yPos+1, boxDepth);
		}
		else if (mGrid[xPos][yPos+1]->IsMovable() && boxDepth < 1)
		{
			boxDepth++;
			return ICanMove(DOWN, xPos, yPos+1, boxDepth);
		}
		break;
	case LEFT:
		if (mGrid[xPos-1][yPos]->mpThing->IsNull())
		{	
			return true;
		}
		else if (mGrid[xPos-1][yPos]->mpThing->IsFriendly())
		{
			return ICanMove(LEFT, xPos-1, yPos, boxDepth);
		}
		else if (mGrid[xPos-1][yPos]->IsMovable() && boxDepth < 1)
		{
			boxDepth++;
			return ICanMove(LEFT, xPos-1, yPos, boxDepth);
		}
		break;
	case RIGHT:
		if (mGrid[xPos+1][yPos]->mpThing->IsNull())
		{	
			return true;
		}
		else if (mGrid[xPos+1][yPos]->mpThing->IsFriendly())
		{
			return ICanMove(RIGHT, xPos+1, yPos, boxDepth);
		}
		else if (mGrid[xPos+1][yPos]->IsMovable() && boxDepth < 1)
		{
			boxDepth++;
			return ICanMove(RIGHT, xPos+1, yPos, boxDepth);
		}
		break;
	}
	return false;
}

void GridWorld::PushInDir(MoveDir dir, int xPos, int yPos)
{
	switch (dir)
	{
	case UP:
		if (!mGrid[xPos][yPos-1]->mpThing->IsNull())
		{
			PushInDir(dir, xPos, yPos-1);
		}
		mGrid[xPos][yPos-1]->mpThing = mGrid[xPos][yPos]->mpThing;
		mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
		break;
	case DOWN:
		if (!mGrid[xPos][yPos+1]->IsNull())
		{
			PushInDir(dir, xPos, yPos+1);
		}
		mGrid[xPos][yPos+1]->mpThing = mGrid[xPos][yPos]->mpThing;
		mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
		break;
	case LEFT:
		if (!mGrid[xPos-1][yPos]->IsNull())
		{
			PushInDir(dir, xPos-1, yPos);
		}
		mGrid[xPos-1][yPos]->mpThing = mGrid[xPos][yPos]->mpThing;
		mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
		break;
	case RIGHT:
		if (!mGrid[xPos+1][yPos]->IsNull())
		{
			PushInDir(dir, xPos+1, yPos);
		}
		mGrid[xPos+1][yPos]->mpThing = mGrid[xPos][yPos]->mpThing;
		mGrid[xPos][yPos]->mpThing = FindObjectInList(' ');
		break;
	}
}

bool gLightTop, gLightBottom;
bool GridWorld::IsAlive()
{
	if (mGrid[mpPlayerTile->xCoord][mpPlayerTile->yCoord]->mpGround->IsLight())
	{
		for (int y = mpPlayerTile->yCoord; y >= 0; y--)
		{
			if (mGrid[mpPlayerTile->xCoord][y]->mpThing->IsCover())
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

void GridWorld::Update(MoveDir dir)
{
	if (!IsAlive())
	{
		printf("YOU DEAD\n");
	}
	if (dir == NONE)
	{
		return;
	}
	int xPos = mpPlayerTile->xCoord;
	int yPos = mpPlayerTile->yCoord;

	if (ICanMove(dir, xPos, yPos))
	{
		PushInDir(dir, xPos, yPos);
		mpPlayerTile->Move(dir);
	}
	
	if (!IsAlive())
	{
		printf("YOU DEAD\n");
	}
}

Object *GridWorld::FindObjectInList(char c)
{
	for(unsigned int i = 0; i < mWorldTiles.size(); i++)
	{
		if (mWorldTiles[i]->mIcon == c)
		{
			return mWorldTiles[i];
		}
	}
	assert(0);
	return NULL;
}

void GridWorld::DebugDraw() const
{
	for (int j = 0; ((j < MAX_WORLD_HEIGHT) && (mGrid[0][j] != NULL) && (mGrid[0][j]->mpGround->mIcon != '\n')); j++)
	{
		for (int i = 0; ((i < MAX_WORLD_WIDTH) && (mGrid[i][j]->mpGround->mIcon != '\n')); i++)
		{
			if (i==mpPlayerTile->xCoord && j==mpPlayerTile->yCoord)
			{
				printf("$");
			}
			else
			{
				if (!mGrid[i][j]->mpThing->IsNull())
				{
					printf("%c", mGrid[i][j]->mpThing->mIcon);
				}
				else
				{
					printf("%c", mGrid[i][j]->mpGround->mIcon);
				}
			}
		}
		printf("\n");
	}
}

GridWorld::~GridWorld()
{
	mWorldTiles.clear();
}