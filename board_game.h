#ifndef BOARD_GAME_H
#define BOARD_GAME_H

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct s_game {
    int **table;
    int length;
    int width;
} t_game;

typedef struct s_player {
    int startX;
    int startY;
    int x;
    int y;
    int type;
}   t_player;

void CreateTable();
void init_game(t_game *game);
void init_player(t_player *player, int type);
void remove_cell(t_game *game, int x, int y);
int move(t_game *game, char way, t_player *player);
void change_table(t_player *player);

#endif