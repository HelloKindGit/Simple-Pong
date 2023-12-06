#include <iostream>
#include <raylib.h>

using namespace std;

Color green = Color{38, 185, 154, 255};
Color dark_green = Color{20, 160, 133, 255};
Color light_green = Color{129, 204, 184, 255};
Color yellow = Color{243, 213, 91, 255};

int playerScore = 0;
int cpuScore = 0;

class Ball
{
public:
    float x, y;
    int speedX, speedY;
    int radius;

    void draw()
    {
        DrawCircle(x, y, radius, yellow);
    }

    void update()
    {
        x += speedX;
        y += speedY;

        // boundary collision checks
        if((y + radius) >= GetScreenHeight() || (y - radius) <= 0)
        {
            speedY *= -1;
        }

        if((x + radius) >= GetScreenWidth())
        {
            cpuScore++;
            resetBall();
        }

        if((x - radius) <= 0)
        {
            playerScore++;
            resetBall();
        }
    }

    void resetBall()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void limitMovement()
    {
        if(y <= 0)
        {
            y = 0;
        }

        if(y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    float speed;

    void draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }

        limitMovement();
    }
};

class CpuPaddle: public Paddle
{
public:
    void update(int ballY)
    {
        if(y + (height/2) > ballY)
        {
            y -= speed;
        }
        if(y + (height/2) <= ballY)
        {
            y += speed;
        }

        limitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout << "Game Started" << endl;

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT/2;
    ball.radius = 20;
    ball.speedX = 8;
    ball.speedY = 8;

    player.width = 25;
    player.height = 120;
    player.x = SCREEN_WIDTH - player.width - 10;
    player.y = (SCREEN_HEIGHT / 2) - (player.height / 2);
    player.speed = 7;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = (SCREEN_HEIGHT / 2) - (cpu.height / 2);
    cpu.speed = 7;

    while(!WindowShouldClose())
    {
        BeginDrawing();

        // Updating
        ball.update();
        player.update();
        cpu.update(ball.y);

        // paddle collision check
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speedX *= -1;
        }

        // Drawing
        ClearBackground(dark_green);
        DrawRectangle((SCREEN_WIDTH/2), 0, (SCREEN_WIDTH/2), SCREEN_HEIGHT, green);
        DrawCircle((SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), 150, light_green);
        
        DrawLine((SCREEN_WIDTH/2), 0, (SCREEN_WIDTH/2), SCREEN_HEIGHT, WHITE);
        
        ball.draw();

        player.draw();
        cpu.draw();

        DrawText(TextFormat("%i", cpuScore), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}