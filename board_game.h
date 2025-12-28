#ifndef BOARD_GAME_H
#define BOARD_GAME_H

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define BOARD_SIZE 7
#define BOARD_MAX_INDEX 6
#define CELL_SIZE 70
#define SPACING 70

typedef struct s_game {
    int **table;
    int height;
    int width;
    int cellSize;
    int spacing;
    int count;
} t_game;

typedef struct s_player {
    int startX;
    int startY;
    int x;
    int y;
    int type;
}   t_player;

void CreateTable(t_game *game);
void init_game(t_game *game);
void init_player(t_player *player, int type);
void change_table(t_game *game, t_player *player);

void remove_cell(t_game *game, int x, int y);
int move(t_game *game, char way, t_player *player, t_player *opponent);
int count_valid_moves(t_game *game, t_player *player, t_player *opponent);
void free_game(t_game *game, t_player *player1, t_player *player2);

void copy_game_state(t_game *dest, t_game *src, t_player *p1_dest, t_player *p1_src, t_player *p2_dest, t_player *p2_src);
void get_valid_move_directions(t_game *game, t_player *player, t_player *opponent, char moves[8], int *count);
void get_valid_remove_cells(t_game *game, t_player *p1, t_player *p2, int cells[][2], int *count);
int evaluate(t_game *game, t_player *player, t_player *opponent);
int minimax_alpha_beta(t_game *game, t_player *ai, t_player *human, int depth, int alpha, int beta, int maximizing, char *best_move, int *best_remove_x, int *best_remove_y);

#endif