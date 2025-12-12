#include "board_game.h"

void remove_cell(t_game *game, int x, int y) {
	if (x < 0 || x > 6 || y < 0 || y > 6)
		return;
	if (game->table[x][y] == -1)
		return;
	game->table[x][y] = -1;
}

int move(t_game *game, char way, t_player *player, t_player *opponent) {
    if(way == 'w') {
        if(player->y == 0)
            return 0;
        if(game->table[player->x][player->y - 1] == -1)
            return 0;
        if(player->x == opponent->x && player->y - 1 == opponent->y)
            return 0;
        player->y--;
    }
    else if(way == 's') {
        if(player->y == game->height)
            return 0;
        if(game->table[player->x][player->y + 1] == -1)
            return 0;
        if(player->x == opponent->x && player->y + 1 == opponent->y)
            return 0;
        player->y++;
    }
    else if(way == 'a') {
        if(player->x == 0)
            return 0;
        if(game->table[player->x - 1][player->y] == -1)
            return 0;
        if(player->x - 1 == opponent->x && player->y == opponent->y)
            return 0;
        player->x--;
    }
    else if(way == 'd') {
        if(player->x == game->width)
            return 0;
        if(game->table[player->x + 1][player->y] == -1)
            return 0;
        if(player->x + 1 == opponent->x && player->y == opponent->y)
            return 0;
        player->x++;
    }
    else if(way == 'e') {
        if((player->x == game->width) || player->y == 0)
            return 0;
        if(game->table[player->x + 1][player->y - 1] == -1)
            return 0;
        if(player->x + 1 == opponent->x && player->y - 1 == opponent->y)
            return 0;
        player->x++;
        player->y--;
    }
    else if(way == 'q') {
        if((player->x == 0) || player->y == 0)
            return 0;
        if(game->table[player->x - 1][player->y - 1] == -1)
            return 0;
        if(player->x - 1 == opponent->x && player->y - 1 == opponent->y)
            return 0;
        player->x--;
        player->y--;
    }
    else if(way == 'z') {
        if((player->x == 0) || player->y == game->height)
            return 0;
        if(game->table[player->x - 1][player->y + 1] == -1)
            return 0;
        if(player->x - 1 == opponent->x && player->y + 1 == opponent->y)
            return 0;
        player->x--;
        player->y++;
    }
    else if(way == 'x') {
        if((player->x == game->width) || player->y == game->height)
            return 0;
        if(game->table[player->x + 1][player->y + 1] == -1)
            return 0;
        if(player->x + 1 == opponent->x && player->y + 1 == opponent->y)
            return 0;
        player->x++;
        player->y++;
    }
    return 1;
}

