#include "StdAfx.h"
#include "DemoActor.h"

#include "TextRendering.h"
#include "Xbox360Input.h"

#include "ActorFactory.h"

void DemoActor::Update(float dt)
{
	Actor::Update(dt);

	// You probably wouldn't actually want to do this kind of branching,
	//  but it's an example of how to use tags and names. 

	// Also shows how to do simple color/rotation changes and check for input. 

	if (this->IsTagged("purple"))
	{
		SetRotation(GetRotation() + (90.0f * dt));

		if (theController.IsAButtonDown())
		{
			SetColor(0,1,0);
		}
		else if (theController.IsBButtonDown())
		{
			SetColor(1,0,0);
		}
		else if (theController.IsXButtonDown())
		{
			SetColor(0,0,1);
		}
		else if (theController.IsYButtonDown())
		{
			SetColor(1,1,0);
		}

	}

	else if ("LeftWoodPiece" == GetName())
	{
		Vec2i left = theController.GetLeftThumbstick();
		Vector2 lpos;

		if (left.X)
		{
			lpos.X = -3.0f + (3.0f * ((float)left.X / 32768.0f));
		}
		else
		{
			lpos.X = -3.0f;
		}
		if (left.Y)
		{
			lpos.Y = 5.0f + (3.0f * ((float)left.Y / 32768.0f));
		}
		else
		{
			lpos.Y = 5.0f;
		}

		SetPosition(lpos.X, lpos.Y);
	}

	else if ("RightWoodPiece" == GetName())
	{
		Vec2i right = theController.GetRightThumbstick();
		Vector2 rpos;

		if (right.X)
		{
			rpos.X = 3.0f + (3.0f * ((float)right.X / 32768.0f));
		}
		else
		{
			rpos.X = 3.0f;
		}
		if (right.Y)
		{
			rpos.Y = -5.0f + (3.0f * ((float)right.Y / 32768.0f));
		}
		else
		{
			rpos.Y = -5.0f;
		}

		SetPosition(rpos.X, rpos.Y);
	}
}

void DemoActor::Render()
{
	Actor::Render();
	if ("LeftWoodPiece" == GetName())
	{
		Vector2 screenPos = MathUtil::WorldToScreen(GetPosition().X, GetPosition().Y);
		char buff[255];
		sprintf_s(buff, "%3.2f, %3.2f", GetPosition().X, GetPosition().Y);
		glColor3f(0,0,0);
		DrawGameText(buff, "ConsoleSmall", (int)screenPos.X - 50, (int)screenPos.Y + 70);
	}
}

void DemoActor::AnimCallback(String animName)
{
	if (!strcmp(animName.c_str(), "MeleeSwing"))
	{
		theActorFactory.BeginActor("particle_AnimEnd", 0);
			theActorFactory.GetCurrentActor()->SetPosition(_position.X, _position.Y);
		theActorFactory.EndActor();

		PlaySpriteAnimation(0.2f, SAT_OneShot, 16, 0, "SwingBack");
	}
	else if (!strcmp(animName.c_str(), "SwingBack"))
	{
		PlaySpriteAnimation(0.5f, SAT_OneShot, 0, 16, "MeleeSwing");
	}
	else
	{
		std::cout << "AnimCallback() couldn't find match for " << animName << std::endl;
	}
}