#include "board_game.h"

//void remove_cell(t_game *game, int x, int y) {}

int move(t_game *game, char way, t_player *player) {
    if(way == 'w') {
        if(player->y == 0)
            return 0;
        player->y--;
    }
    else if(way == 's') {
        if(player->y == game->length)
            return 0;
        player->y++;
    }
    else if(way == 'a') {
        if(player->x == 0)
            return 0;
        player->x--;
    }
    else if(way == 'd') {
        if(player->x == game->width)
            return 0;
        player->x++;
    }
    return 1;
}

