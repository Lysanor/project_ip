// menu.c
#include "raylib.h"
#include "menu.h"
#include <stdio.h>

// Definições locais
static const int screenWidth = 1280;
static const int screenHeight = 720;

static Rectangle playButton;
static Rectangle creditsButton;
static Rectangle exitButton;

static GameState currentState = MENU;

GameState GetGameState()
{
    return currentState;
}

static void InitButtons();

void InitMenu()
{
    InitWindow(screenWidth, screenHeight, "Menu Principal");
    SetTargetFPS(60);
    InitButtons();
}

static void InitButtons()
{
    playButton = (Rectangle){screenWidth / 2 - 100, screenHeight / 2 - 60, 200, 40};
    creditsButton = (Rectangle){screenWidth / 2 - 100, screenHeight / 2, 200, 40};
    exitButton = (Rectangle){screenWidth / 2 - 100, screenHeight / 2 + 60, 200, 40};
}

void UpdateMenu()
{
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentState = GAME;
    }
    else if (CheckCollisionPointRec(mousePoint, creditsButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentState = CREDITS;
    }
    else if (CheckCollisionPointRec(mousePoint, exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        currentState = EXIT;
    }
}

void DrawMenu()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Menu Principal", screenWidth / 2 - MeasureText("Menu Principal", 20) / 2, screenHeight / 2 - 120, 20, LIGHTGRAY);

    DrawRectangleRec(playButton, RED);
    DrawRectangleRec(creditsButton, GREEN);
    DrawRectangleRec(exitButton, BLUE);

    DrawText("Jogar", playButton.x + playButton.width / 2 - MeasureText("Jogar", 20) / 2, playButton.y + 10, 20, BLACK);
    DrawText("Créditos", creditsButton.x + creditsButton.width / 2 - MeasureText("Créditos", 20) / 2, creditsButton.y + 10, 20, BLACK);
    DrawText("Sair", exitButton.x + exitButton.width / 2 - MeasureText("Sair", 20) / 2, exitButton.y + 10, 20, BLACK);
    EndDrawing();
}

void CloseMenu()
{
    // Aqui poderia ter código de limpeza, se necessário
    CloseWindow();
}