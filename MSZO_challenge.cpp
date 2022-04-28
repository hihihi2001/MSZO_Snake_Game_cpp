#include "SnakeGame.h"	// include the game

using namespace std;

int main()
{
	int direction = STOP;	// default direction
	SnakeGame Game = SnakeGame();	// create the game
	clock_t start = clock();	
	while (!Game.getGameOver())	// play while you are alive
	{
		keyInput(direction);	
		if (clock() - start > (int)(200 * CLOCKS_PER_SEC / 1000))	// move only in evry 200th ms
		{
			start = clock();
			Game.move(direction);	// make a step with the snake
			Game.render();	// draw the board with the snake and the apple
			vector<vector<int>> map = Game.getMap();	// get the board as a 2D vector
			render(map); // you can view the vector
		}
	}
	showConsoleCursor(true);
	while (clock() - start > (int)(500 * CLOCKS_PER_SEC / 1000)) {}	// wait 500 ms
	system("pause");
	return 0;
}
