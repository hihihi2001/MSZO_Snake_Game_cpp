#include "SnakeGame.h"

using namespace std;

// map minden elemén végrehajt egy függvényt
void change(vector<vector<int>> &map, int (*func)(int))
{
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			map[j][i] = func(map[j][i]);
}

// Fogja a tilesban lévő utolsó koordináta szerinti mezőt a mapben, és megnézi a 4 szomszédos mezőt.
// Ha azok léteznek, nem kígyók (<0), és kisebbek, mint map[x][y]-1, akkor
// Átírja az értékük map[x][y]-1re és beteszi őket a tiles elejére.
void way(vector<vector<int>>& map, vector<Coord> &tiles)
{
	int x = tiles.back().x;
	int y = tiles.back().y;
	if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT && map[x][y] < 0)
	{
		int dist = map[x][y] - 1;
		if (y - 1 >= 0 && map[x][y - 1] < 0 && map[x][y - 1] < dist)
		{
			map[x][y - 1] = dist;
			tiles.insert(tiles.begin(), Coord(x, y - 1));
		}
		if (y + 1 < HEIGHT && map[x][y + 1] < 0 && map[x][y + 1] < dist)
		{
			map[x][y + 1] = dist;
			tiles.insert(tiles.begin(), Coord(x, y + 1));
		}
		if (x - 1 >= 0 && map[x - 1][y] < 0 && map[x - 1][y] < dist)
		{
			map[x - 1][y] = dist;
			tiles.insert(tiles.begin(), Coord(x - 1, y));
		}
		if (x + 1 < WIDTH && map[x + 1][y] < 0 && map[x + 1][y] < dist)
		{
			map[x + 1][y] = dist;
			tiles.insert(tiles.begin(), Coord(x + 1, y));
		}
	}
	tiles.pop_back();
}

int main()
{
	int direction = STOP;
	SnakeGame Game = SnakeGame();
	clearScreen();
	while (!Game.getGameOver())
	{
		Coord apple = Game.getApple();
		Coord head = Game.getHead();
		// map létrehozása, a 0-k (üres helyek) cseréje -99re
		vector<vector<int>> map = Game.getMap();
		int maxDist = -WIDTH * HEIGHT;
		change(map, [](int m) -> int {return (m == 0) ? -99 : m; });
		// pathfinding, az apple helye -1, minden üres mező (-99) helyére az almától mért táv*-1 kerül, az alma köré -2, azok köré -3, ...
		// nem működik
		vector<Coord> tiles;
		tiles.push_back(apple);
		while (!tiles.empty())
		{
			way(map, tiles);
		}
		// a legnagyobb szám felé megy
		int x = head.x;
		int y = head.y;
		int biggest = -WIDTH * HEIGHT;
		if (x - 1 >= 0 && map[x - 1][y] < 0 && map[x - 1][y] > biggest)
		{
			biggest = map[x - 1][y];
			direction = LEFT;
		}
		if (x + 1 < WIDTH && map[x + 1][y] < 0 && map[x + 1][y] > biggest)
		{
			biggest = map[x + 1][y];
			direction = RIGHT;
		}
		if (y - 1 >= 0 && map[x][y - 1] < 0 && map[x][y-1] > biggest)
		{
			biggest = map[x][y - 1];
			direction = UP;
		}
		if (y + 1 < HEIGHT && map[x][y + 1] < 0 && map[x][y + 1] > biggest)
		{
			biggest = map[x][y + 1];
			direction = DOWN;
		}
		Game.move(direction);

		Game.render();
		//render(map, 3, -99);
		//system("pause");
	}/**/

	system("pause");
	return 0;
}