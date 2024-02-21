#ifndef MENU_H
#define MENU_H

void InitMenu();
void UpdateMenu();
void DrawMenu();
void CloseMenu();


//Define o estado do jogo
typedef enum GameState {
    MENU,
    GAME,
    CREDITS,
    EXIT
} GameState;

GameState GetGameState();

#endif