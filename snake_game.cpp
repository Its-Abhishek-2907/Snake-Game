#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

const char DIR_UP = 'W';
const char DIR_DOWN = 'S';
const char DIR_LEFT = 'A';
const char DIR_RIGHT = 'D';

class Point
{
public:
    int xCoord;
    int yCoord;
    Point *next;
    Point *prev;
    Point() {}
    Point(int x, int y) : xCoord(x), yCoord(y), next(nullptr), prev(nullptr) {}
};

class Snake
{
private:
    Point *snake;
    Point *tail;
    int length, height, width, food_x, food_y;
    int gameover;
    char direction;
    int score;

public:
    Snake() { setup(); }

    void setup()
    {
        length = 1;
        height = 20;
        width = 60;
        direction = DIR_RIGHT;
        snake = new Point(10, 10);
        tail = snake;
        food_x = rand() % (width - 1) + 1;
        food_y = rand() % (height - 1) + 1;
        score = 0;
        gameover = 0;
    }

    void drawBoundary(int height, int width, char boundaryChar)
    {
        for (int i = 0; i <= height; i++)
        {
            for (int j = 0; j <= width; j++)
            {
                if (i == 0 || j == 0 || i == height || j == width)
                {
                    cout << boundaryChar;
                }
                else
                {
                    cout << " ";
                }
            }
            cout << "\n";
        }
    }

    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void drawSnake(Point *snake, char headChar, char bodyChar)
    {
        if (snake->next)
        {
            gotoxy(snake->xCoord, snake->yCoord);
            cout << headChar;
            Point *n = snake->next;
            gotoxy(n->xCoord, n->yCoord);
            cout << bodyChar;
        }
        else
        {
            gotoxy(snake->xCoord, snake->yCoord);
            cout << headChar;
        }
    }

    void drawSnakeFruit(int fruitX, int fruitY, char fruitChar)
    {
        gotoxy(fruitX, fruitY);
        cout << fruitChar;
    }

    void draw()
    {
        drawSnake(snake, 'O', '@');
        drawSnakeFruit(food_x, food_y, '*');
        gotoxy(0, height + 1);
        cout << "Current Score: " << score << "   Press 'x' to quit";
    }

    void clear()
    {
        gotoxy(tail->xCoord, tail->yCoord);
        cout << " ";
    }

    void changeDirection(char val)
    {
        if (val == DIR_UP && direction != DIR_DOWN)
        {
            direction = DIR_UP;
        }
        else if (val == DIR_DOWN && direction != DIR_UP)
        {
            direction = DIR_DOWN;
        }
        else if (val == DIR_LEFT && direction != DIR_RIGHT)
        {
            direction = DIR_LEFT;
        }
        else if (val == DIR_RIGHT && direction != DIR_LEFT)
        {
            direction = DIR_RIGHT;
        }
    }

    void input()
    {
        if (_kbhit())
        {
            char val = _getch();
            if (val == 'x')
            {
                gameover = 1;
            }
            else
                changeDirection(toupper(val));
        }
    }

    void run()
    {
        Point *head = new Point(snake->xCoord, snake->yCoord);
        switch (direction)
        {
        case DIR_UP:
            head->yCoord = snake->yCoord - 1;
            break;
        case DIR_DOWN:
            head->yCoord = snake->yCoord + 1;
            break;
        case DIR_RIGHT:
            head->xCoord = snake->xCoord + 1;
            break;
        case DIR_LEFT:
            head->xCoord = snake->xCoord - 1;
            break;
        }
        head->next = snake;
        snake->prev = head;
        snake = head;
        if (snake != tail)
        {
            Point *n = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete n;
        }

        if (snake->xCoord == 0 || snake->xCoord == width || snake->yCoord == 0 || snake->yCoord == height)
        {
            gameover = 1;
        }

        Point *temp = snake->next;
        while (tail->next != nullptr)
        {
            if (snake->xCoord == temp->xCoord && snake->yCoord == temp->yCoord)
            {
                gameover = 1;
            }
            temp = temp->next;
        }

        if (snake->xCoord == food_x && snake->yCoord == food_y)
        {
            score += 10;
            Point *n = new Point(tail->xCoord, tail->yCoord);
            n->prev = tail;
            tail->next = n;
            tail = n;
            food_x = rand() % (width - 1) + 1;
            food_y = rand() % (height - 1) + 1;
        }
    }

    int getScore() { return score; }

    bool isGameover() { return gameover; }

    int getHeight() { return height; }

    int getWidth() { return width; }
};

int main()
{
    char ch;
    int height, width, speed;
    do
    {
        Snake *s = new Snake();
        height = s->getHeight();
        width = s->getWidth();
        system("cls");
        s->drawBoundary(height, width, '#');
        while (!s->isGameover())
        {
            s->clear();
            s->run();
            s->draw();
            s->input();
            int current_score = s->getScore();
            if (current_score <= 10)
                speed = 350;
            else if (current_score >= 10 && current_score <= 20)
                speed = 300;
            else if (current_score >= 20 && current_score <= 30)
                speed = 200;
            else if (current_score >= 40 && current_score <= 50)
                speed = 100;
            Sleep(speed);
        }
        cout << "\nGame Over!" << endl;
        cout << "Final Score: " << s->getScore() << endl;
        cout << "Do you want to play again (y/n)?: ";
        cin >> ch;
        delete s;
    } while (ch == 'y' || ch == 'Y');
    return 0;
}