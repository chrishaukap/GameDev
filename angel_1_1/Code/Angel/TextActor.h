#pragma once

#include "Actor.h"

#include <vector>

enum TextAlignment
{
	TXT_Left,
	TXT_Center,
	TXT_Right
};

class TextActor : public Actor
{
public:
	TextActor(String fontNickname, String displayString="", TextAlignment align=TXT_Left, int lineSpacing=5);

	virtual void Render();
	virtual void Update(float dt);

	const String GetFont();
	void SetFont(String newFont);
	const String GetDisplayString();
	void SetDisplayString(String newString);

	const TextAlignment GetAlignment();
	void SetAlignment(TextAlignment newAlignment);

	const int GetLineSpacing();
	void SetLineSpacing(int newSpacing);

	virtual void SetPosition(float x, float y);
	virtual void SetPosition(Vector2 position);

private:
	struct TextNugget
	{
		String _string;
		Vector2 _extents;
		Vector2 _position;
	};

	void CalculatePosition();

	String _fontNickname;
	String _rawString;
	TextAlignment _alignment;

	std::vector<TextNugget> _displayStrings;

	int _lineSpacing;

	Vector2 _screenPosition;
};
