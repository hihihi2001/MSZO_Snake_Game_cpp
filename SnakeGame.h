#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define WINDOWS
#endif/**/
#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
// Map size:
#define WIDTH 32
#define HEIGHT 16
// Directions to move:
#define STOP 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
// Colors for printing:
#define WHITE 1
#define GRAY 2
#define RED 3
#define GREEN 4
#define BLUE 5

using namespace std;

// struct for coordinates
struct Coord {
	int x;
	int y;
	Coord(int x = 0, int y = 0)
	{
		this->x = x;
		this->y = y;
	}
	int consoleX()
	{
		return x + 1;
	}
	int consoleY()
	{
		return y + 1;
	}
	Coord console()
	{
		return Coord(x + 1, y + 1);
	}
};

// Basic functions:

// Can show or hide cursor_
void showConsoleCursor(bool visible);
// Removes everything from screen
void clearScreen();
// Places cursor to (p.x, p.y)
void cursor(Coord p);
// Places cursor to (x, y)
void cursor(int x, int y);
// Sets printing color
void color(int col);
// Draws char c to (p.x, p.y) with color col
void draw(Coord p, char c, int col);
// Draws char c to (x, y) with color col
void draw(int x, int y, char c, int col);
// sets direction if a key is pressed (only on windows)
void keyInput(int &direction);
// renders a 2D int vector (like from SnakeGame::getMap())
void render(vector<vector<int>> map, int w = 2, int empty = 0);

class SnakeGame {
private:
	time_t start;	// unix time, when constructor was called
	clock_t lastCall{ 0 };	// time at the end of last .fps()'s run
	int framePerSecs[10];	// last 10 measured fps
	string title;	
	unsigned int steps{ 0 };	
	int score{ 0 };
	bool gameOver{ false };
	int direction{ STOP };
	Coord apple;
	vector<Coord> snake;
	unsigned char pixels[WIDTH + 2][HEIGHT + 2];
	int pixelColors[WIDTH + 2][HEIGHT + 2];

	// places an apple on the map, but not on the snake
	bool placeApple();
	// sets pixels[x][y] to char c and pixelColors[x][y] to color col
	void draw(int x, int y, char c, int col);
	// sets pixels[p.x][p.y] to char c and pixelColors[p.x][p.y] to color col
	void draw(Coord p, char c, int col);
	// calculates fps, and draws it an console's window header
	void fps();
	
public:
	SnakeGame(string title = "Snake Game");
	// returns true if the game is lost
	bool getGameOver();
	// returns the coordinates of the apple
	Coord getApple();
	// returns teh coordinates of the snake's head
	Coord getHead();
	// returns a 2D vector with size WIDTH*HEIGHT, where the apple is -1;
	// the snake head is 1; the snake body is 2, 3, 4...; and the empty place is 0
	vector<vector<int>> getMap();
	// returns a 1D vector with the coordinates of snake
	vector<Coord> getSnake();
	// moves the snake; refreshes the variables; determines gameover, score, growth;
	// returns true, if you cloud move in the designated direction
	bool move(int dir = STOP);
	// draws everything with ascii chars; prints some useful numbers; calls .fps()
	void render();
};