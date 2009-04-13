#pragma once

#include "Renderable.h"
#include "StringUtil.h"

#include <set>
#include <map>

#define MAX_SPRITE_FRAMES 64

enum spriteAnimationType
{
	SAT_None,
	SAT_Loop,
	SAT_PingPong,
	SAT_OneShot
};

enum actorDrawShape
{
    ADS_Square,
    ADS_Circle
};

class Actor : public Renderable
{
public:
	Actor();
	virtual ~Actor();

	// equal dimensions by default
	virtual void SetSize(float x, float y = -1.f);
	const Vector2 GetSize();
	virtual void SetPosition(float x, float y);
	virtual void SetPosition(Vector2 pos);
	const Vector2 GetPosition();
	void SetRotation(float rotation);
	const float GetRotation();
	const Color4f GetColor();
	void SetColor(float r, float g, float b);
	void SetColor(float r, float g, float b, float a);
	void SetColor(Color3f color);
	void SetColor(Color4f color);
	void SetAlpha(float newAlpha);
	const float GetAlpha();
    void SetDrawShape( actorDrawShape DrawShape );

	const int GetSpriteTexture(int frame = 0);

	bool SetSprite(String filename, int frame = 0, GLint clampmode = GL_CLAMP, GLint filtermode = GL_LINEAR, bool optional=0);
	void ClearSpriteInfo();
	void LoadSpriteFrames(String firstFilename, GLint clampmode = GL_CLAMP, GLint filtermode = GL_LINEAR);
	void PlaySpriteAnimation(float delay, spriteAnimationType animType = SAT_Loop, int startFrame = -1, int endFrame = -1, const char* _animName = NULL); //rb - TODO - Add a way to associate anim type, and frame indices to a name.
	void SetSpriteFrame(int frame);
	void UpdateSpriteAnimation(float dt);
	bool IsSpriteAnimPlaying()
	{
		return (_spriteFrameDelay > 0);
	}
	void SetUVs(const Vector2 upright, const Vector2 lowleft);
	void GetUVs(Vector2 &upright, Vector2 &lowleft) const;

	const bool IsTagged(String tag);
	void Tag(String newTag);
	void Untag(String oldTag);
	const StringSet GetTags();

	const String SetName(String newName);
	const String GetName();
	static const Actor* GetNamed(String nameLookup);

	virtual void OnCollision(Actor* other);
	virtual void Update(float dt);
	virtual void Render();
	virtual void AnimCallback(String animName) {}

	// Called for every actor that doesn't get unloaded in World::UnloadAll().   
	// This is a good place to clear out any cached pointers, etc.
	virtual void LevelUnloaded() {}

protected:
	Vector2 _size;
	Vector2 _position;
	Color4f _color;
	float _rotation;

	Vector2 UV_rightup;
	Vector2 UV_leftlow;

    actorDrawShape _drawShape;

	int					_spriteCurrentFrame;
	int					_spriteNumFrames;
	float				_spriteFrameDelay;
	float				_spriteCurrentFrameDelay;
	int					_spriteTextureReferences[MAX_SPRITE_FRAMES];
	spriteAnimationType _spriteAnimType;
	int					_spriteAnimStartFrame;
	int					_spriteAnimEndFrame;
	int					_spriteAnimDirection;

	StringSet _tags;

	String _name;
	static std::map<String, Actor*> _nameList;

	String _currentAnimName;

private:
	void SetSpriteTexture(int texRef, int frame = 0);
};

typedef std::vector<Actor*>		ActorList;
typedef std::set<Actor*>		ActorSet;

class ActorFactoryDelegate
{
public:
	ActorFactoryDelegate::ActorFactoryDelegate()
		: _renderLayer(1)
	{}
	virtual void RegisterOriginalConsoleCommands();
	virtual Actor* CreateInstance(); 
	virtual void FinishInstance(Actor* pActor ) {}

	virtual void SetSize( float x, float y );
	virtual void SetPosition(float x, float y);
	virtual void SetRotation(float rotation);
	virtual void SetColor(float r, float g, float b);
	virtual void SetAlpha(float newAlpha);
	virtual void SetTag(String tag);
	virtual void SetName(String name);
	virtual void SetLayer(int layerIndex);
	virtual void SetLayerName(String layerName );
	virtual void SetSprite(String filename, int frame );
	virtual void LoadSpriteFrames(String firstFilename);
	virtual void Tag( const String& tag );
	friend class ActorFactory;

protected:
	virtual void InitializeDelegate();

private:
	int						_renderLayer;
};

