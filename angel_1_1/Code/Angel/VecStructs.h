#pragma once

struct Color3f
{
	float R, G, B;
	Color3f(float r, float g, float b) : R(r), G(g), B(b) {}
	Color3f() : R(0), G(0), B(0) {}
};

struct Color4f
{
	float R, G, B, A;
	Color4f(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
	Color4f() : R(0), G(0), B(0), A(0) {}
};

struct Vec2i
{
	int X, Y;
	Vec2i(int x, int y) : X(x), Y(y) {}
	Vec2i() : X(0), Y(0) {}
};
