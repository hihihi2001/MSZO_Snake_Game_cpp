#include "SnakeGame.h"

using std::cout;

// global variables (visible only in SnakeGame.cpp)
// https://theasciicode.com.ar
const unsigned char sideBarrier = 219; // █
const unsigned char bottomBarrier = 223; // ▀
const unsigned char topBarrier = 220; // ▄
const unsigned char square = 254; // ■
const unsigned char vertical = 186; // ║
const unsigned char horizontal = 205; // ═
const unsigned char right2down = 187; // ╗
const unsigned char right2up = 188; // ╝
const unsigned char left2down = 201; // ╔
const unsigned char left2up = 200; // ╚

void showConsoleCursor(bool visible)
{
#ifdef WINDOWS
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(out, &cursorInfo);
#else
    if (visible) cout << "\033[?25h";
    else cout << "\033[?25l";
#endif
}

void clearScreen()
{
#ifdef WINDOWS
    system("cls");
#else
    cout << "\033[2K";
#endif
}

void cursor(Coord p)
{
#ifdef WINDOWS
    COORD coord = { (SHORT)p.x, (SHORT)p.y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", p.y + 1, p.x + 1);
#endif
}
void cursor(int x, int y)
{
#ifdef WINDOWS
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

void color(int col)
{
    if (col == WHITE) cout << "\033[1;37m";
    else if (col == GRAY) cout << "\033[0m";
    else if (col == RED) cout << "\033[1;31m";
    else if (col == GREEN) cout << "\033[1;32m";
    else if (col == BLUE) cout << "\033[1;34m";
}

void keyInput(int &direction)
{
#ifdef WINDOWS
    if (GetKeyState('W') & 0x8000 || GetKeyState(VK_UP) & 0x8000) direction = UP;
    else if (GetKeyState('A') & 0x8000 || GetKeyState(VK_LEFT) & 0x8000) direction = LEFT;
    else if (GetKeyState('S') & 0x8000 || GetKeyState(VK_DOWN) & 0x8000) direction = DOWN;
    else if (GetKeyState('D') & 0x8000 || GetKeyState(VK_RIGHT) & 0x8000) direction = RIGHT;
#else 
    // RIP
#endif
}

void render(vector<vector<int>> map, int w, int empty)
{
    for (int k = 0; k < w; k++) cout << ' ';
    for (int j = 0; j < WIDTH; j++) cout << setw(w) << j;
    cout << endl;
    for (int k = 0; k < w; k++) cout << ' ';
    cout << '+';
    for (int j = 0; j < WIDTH; j++) for (int k = 0; k < w; k++) cout << '-';
    cout << '+' << endl;
    for (int i = 0; i < HEIGHT; i++)
    {
        cout << setw(w) << i << '|';
        for (int j = 0; j < WIDTH; j++)
            if (map[j][i] != empty) cout << setw(w) << map[j][i]; else for (int k = 0; k < w; k++) cout << ' ';
        cout << '|' << endl;
    }
    for (int k = 0; k < w; k++) cout << ' ';
    cout << '+';
    for (int j = 0; j < WIDTH; j++) for (int k = 0; k < w; k++) cout << '-';
    cout << '+' << endl;
}

void draw(Coord p, char c = ' ', int col = WHITE)
{
    cursor(p);
    color(col);
    cout << c;
}
void draw(int x, int y, char c = ' ', int col = WHITE)
{
    cursor(x, y);
    color(col);
    cout << c;
}

bool SnakeGame::placeApple()
{
    if (score >= WIDTH * HEIGHT - 4) return true;
    // generate new apple while it isn't in the snake
    bool fail;
    do
    {
        fail = false;
        apple.x = rand() % WIDTH; apple.y = rand() % HEIGHT;
        for (size_t i = 0; i < snake.size(); i++)
            if (snake.at(i).x == apple.x && snake.at(i).y == apple.y) fail = true;
    } while (fail);
    // draw new apple
    this->draw(apple.console(), square, RED);
    return false;
}

void SnakeGame::draw(int x, int y, char c = ' ', int col = WHITE)
{
    pixels[x][y] = c;
    pixelColors[x][y] = col;
}

void SnakeGame::draw(Coord p, char c = ' ', int col = WHITE)
{
    pixels[p.x][p.y] = c;
    pixelColors[p.x][p.y] = col;
}

void SnakeGame::fps()
{
    for (int i = 0; i < 9; i++) framePerSecs[i] = framePerSecs[i + 1];  // shift framePerSecs[]
    framePerSecs[9] = (int)(CLOCKS_PER_SEC / (clock() - lastCall));     // sets last value to current fps
    // calculate and print avarage
    int sum = 0;
    for (int i = 0; i < 10; i++) sum += framePerSecs[i];
    cout << "\033]0;" << title << ' ' <<
        (int)(sum / 10) << " fps" << "\007";
    lastCall = clock(); // save current time for next call
}

SnakeGame::SnakeGame(string title)
{

    clearScreen();
    showConsoleCursor(false);
    start = time(NULL);
    srand((unsigned int)start);
    for (int i = 0; i < 10; i++) framePerSecs[i] = 1;
    this->title = title;
    lastCall = clock();
    // fill pixels and pixelColors:
    for (int i = 0; i < HEIGHT + 2; i++)
        for (int j = 0; j < WIDTH + 2; j++)
        {
            pixels[j][i] = ' ';
            pixelColors[j][i] = WHITE;
        }
    // draw barriers:
    for (int i = 0; i < WIDTH + 2; i++)
    {
        pixels[i][0] = topBarrier;
        pixelColors[i][0] = GRAY;
        pixels[i][HEIGHT + 1] = bottomBarrier;
        pixelColors[i][HEIGHT + 1] = GRAY;
    }
    for (int j = 1; j < HEIGHT + 1; j++)
    {
        pixels[0][j] = sideBarrier;
        pixelColors[0][j] = GRAY;
        pixels[WIDTH + 1][j] = sideBarrier;
        pixelColors[WIDTH + 1][j] = GRAY;
    }

    placeApple();
    snake.push_back(Coord((int)WIDTH / 2 - 1, (int)HEIGHT / 2 - 1) );    // snake position in the middle
    snake.push_back(Coord((int)WIDTH / 2 - 1, (int)HEIGHT / 2 - 1));
    this->draw(snake.front().console(), square);
    this->render();
}

bool SnakeGame::getGameOver()
{
    return gameOver;
}

Coord SnakeGame::getApple()
{
    return apple;
}

Coord SnakeGame::getHead()
{
    return snake.front();
}

vector<vector<int>> SnakeGame::getMap()
{
    vector<vector<int>> map;
    // create empty map with 0s
    for (int i = 0; i < WIDTH; i++)
    {
        vector<int> line;
        for (int j = 0; j < HEIGHT; j++)
            line.push_back(0);
        map.push_back(line);
    }
    // set apple to -1; and snake to 1, 2, 3...
    map[apple.x][apple.y] = -1;
    if (gameOver) return map;
    for (size_t i = 0; i < snake.size(); i++)
        map[snake.at(i).x][snake.at(i).y] = (int)(i + 1);
    return map;
}

vector<Coord> SnakeGame::getSnake()
{
    return snake;
}

bool SnakeGame::move(int dir)
{
    if (gameOver) return false;
    steps++;
    Coord head = this->getHead();
    char front = square;
    char neck = vertical;
    // the snake can't turn backwards
    bool succes = true;
    if (dir == UP && direction == DOWN) succes = false;
    if (dir == LEFT && direction == RIGHT) succes = false;
    if (dir == DOWN && direction == UP) succes = false;
    if (dir == RIGHT && direction == LEFT) succes = false;
    if (!succes)dir = direction;
    direction = dir;
    // set the position of the head, and the char for the old head (neck)
    switch (direction) {
    case STOP:
        start = time(NULL);
        steps = 0;
        break;
    case UP:
        head.y--;
        if (snake.at(1).x < head.x) neck = right2up;
        else if (snake.at(1).x > head.x) neck =  left2up;
        else neck = vertical;
        front = topBarrier;
        break;
    case LEFT:
        head.x--;
        if (snake.at(1).y < head.y) neck = right2up;
        else if (snake.at(1).y > head.y) neck = right2down;
        else neck = horizontal;
        break;
    case DOWN:
        head.y++;
        if (snake.at(1).x < head.x) neck = right2down;
        else if (snake.at(1).x > head.x) neck = left2down;
        else neck = vertical;
        front = bottomBarrier;
        break;
    case RIGHT:
        head.x++;
        if (snake.at(1).y < head.y) neck = left2up;
        else if (snake.at(1).y > head.y) neck = left2down;
        else neck = horizontal;
        break;
    }
    // insert and draw snake head and neck
    this->draw(snake.front().console(), neck);
    snake.insert(snake.begin(), head);
    // check for death
    if (head.x < 0 || head.y < 0 || head.x >= WIDTH || head.y >= HEIGHT) gameOver = true;
    if (snake.size() > 2 && direction != 0) for (size_t i = 1; i < snake.size(); i++)
        if (snake.at(i).x == head.x && snake.at(i).y == head.y) gameOver = true;
    // check for apple
    if (head.x == apple.x && head.y == apple.y)
    {
        // grow, new apple
        score++;
        gameOver = this->placeApple();
    }
    else
    {
        // don't grow
        this->draw(snake.back().console());
        snake.pop_back();  
    }
    this->draw(snake.front().console(), front); // draw head

    return succes;
}

void SnakeGame::render()
{
    // print map
    cursor(0, 0);
    showConsoleCursor(false);
    int currentColor = WHITE;
    color(currentColor);
    for (int i = 0; i < HEIGHT + 2; i++)
    {
        for (int j = 0; j < WIDTH + 2; j++)
        {
            if (pixelColors[j][i] != currentColor)
            {
                currentColor = pixelColors[j][i];
                color(currentColor);
            }
            cout << pixels[j][i];
        }
        cout << endl;
    }
    // print data
    color(WHITE);
    cursor(0, HEIGHT + 2); cout << "                \n                                \n                ";
    cursor(0, HEIGHT + 2);
    cout << "head: " << snake.front().x << "; " << snake.front().y << endl;
    cout << "steps: " << steps << "\ttime: " << time(NULL) - start << " s" << endl;
    cout << "score: " << score << endl;
    this->fps();
}