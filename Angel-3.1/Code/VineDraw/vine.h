#pragma once

class Vine
{
public:
	Vine();
	~Vine();
	void update();
	void render() const;
private:
	void createPointList();
	class Data;
	Data* _vine;
};