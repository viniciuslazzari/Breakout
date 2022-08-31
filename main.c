#include "raylib.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define RACKET_VELOCITY 4.0
#define BALL_RADIUS 5
#define BLOCKS_INITIAL_X 50.0
#define BLOCKS_INITIAL_Y 50.0
#define BLOCK_SPACING 8.0
#define ROW 5
#define COL 10

void populateMatrix(int row, int col, int* blocos[row][col]){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            blocos[i][j] = 1;
        }
    }
}

int main(void)
{
    float racketLeftCornerX = 0;
    float racketRightCornerX = 0;
    float ballDeviation = 0;
    float ballVelocity = 1;
    int ballBounced = 0;

    int blocos[ROW][COL];

    srand(time(NULL));

    populateMatrix(ROW, COL, &blocos);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");

    Vector2 racket = {(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/8 * 7};
    Vector2 racketSize = {(float)100, (float)15};

    Vector2 ball = {(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/3 * 2};

    Vector2 blockSize = {(float)60, (float)10};

    ballDeviation = 1 + (rand() % (2));

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 blockPosition = {BLOCKS_INITIAL_X, BLOCKS_INITIAL_Y};

        // Muda direcao da raquete
        racketLeftCornerX = racket.x;
        racketRightCornerX = racket.x + 100;

        // Verifica se a raquete esta nas bordas da tela
        if (racketRightCornerX < SCREEN_WIDTH && IsKeyDown(KEY_RIGHT)) {
            racket.x += RACKET_VELOCITY;
        }

        if (racketLeftCornerX > 0 && IsKeyDown(KEY_LEFT)) {
            racket.x -= RACKET_VELOCITY;
        }

        // Move a bola
        ball.x = ball.x + ballDeviation;
        ball.y = ball.y + ballVelocity;

        // Verifica se a bola bateu na raquete
        if ((ball.y + BALL_RADIUS >= racket.y & ball.y - BALL_RADIUS <= racket.y) & (racketLeftCornerX <= ball.x & racketRightCornerX >= ball.x)){
            ballVelocity = -ballVelocity;
        }

        // Verifica se a bola bateu no teto
        if (ball.y - BALL_RADIUS <= 0){
            ballVelocity = -ballVelocity;
        }

        // Verifica se a bola bateu nos cantos
        if (ball.x + BALL_RADIUS >= SCREEN_WIDTH || ball.x - BALL_RADIUS <= 0){
            ballDeviation = -ballDeviation;
        }

        // Verifica se a bola bateu em algum bloco

        blockPosition.y = BLOCKS_INITIAL_Y;

        for (int i = 0; i < ROW; i++){
            blockPosition.x = BLOCKS_INITIAL_X;

            for (int j = 0; j < COL; j++){
                // bloco está ativo
                if (blocos[i][j] == 1){
                    if (ball.x >= blockPosition.x & ball.x <= blockPosition.x + blockSize.x){
                        if (ball.y - BALL_RADIUS <= blockPosition.y + blockSize.y & ball.y + BALL_RADIUS >= blockPosition.y)
                        {
                            blocos[i][j] = 0;
                            ballVelocity = -ballVelocity;
                            ballBounced = 1;
                            break;
                        }
                    }

                    if (ball.y >= blockPosition.y & ball.y <= blockPosition.y + blockSize.y){
                        if (ball.x - BALL_RADIUS <= blockPosition.x + blockSize.x & ball.x + BALL_RADIUS >= blockPosition.x)
                        {
                            blocos[i][j] = 0;
                            ballDeviation = -ballDeviation;
                            ballBounced = 1;
                            break;
                        }
                    }
                }

                blockPosition.x = blockPosition.x + blockSize.x + BLOCK_SPACING;
            }

            if (ballBounced == 1) {
                ballBounced = 0;
                break;
            }

            blockPosition.y = blockPosition.y + blockSize.y + BLOCK_SPACING;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (ball.y >= SCREEN_HEIGHT){
            EndDrawing();

            continue;
        }

        blockPosition.y = BLOCKS_INITIAL_Y;

        for (int i = 0; i < ROW; i++){
            blockPosition.x = BLOCKS_INITIAL_X;

            for (int j = 0; j < COL; j++){
                if (blocos[i][j] == 1){
                    DrawRectangleV(blockPosition, blockSize, YELLOW);
                }

                blockPosition.x = blockPosition.x + blockSize.x + BLOCK_SPACING;
            }

            blockPosition.y = blockPosition.y + blockSize.y + BLOCK_SPACING;
        }

        DrawRectangleV(racket, racketSize, MAROON);
        DrawCircleV(ball, BALL_RADIUS, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
