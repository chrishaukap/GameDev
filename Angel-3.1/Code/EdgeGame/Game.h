#pragma once

#include <vector>

struct Vector2;
namespace Edge
{
	class Game
	{
	public:
		Game(unsigned int rows, unsigned int cols);
		~Game();
			
		typedef std::vector<class Indie*> INDIE_LIST;
		typedef std::vector<class Money*> MONEY_LIST;

      void update(float dt);
      void cleanup();

		void start();
		void stop();

      void moveTimTo(const Vector2& p);

		// Game state query functions
		const MONEY_LIST& getMoneys() const {return m_moneys;}

	private:
		void initMoney();
		void initIndies();
		void initTim();
		void destroyMoney();
		void destroyIndies();
		void destroyTim();

		INDIE_LIST m_indies;
		MONEY_LIST m_moneys;
      class Tim* m_tim;
	};
}