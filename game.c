#include "board_game.h"

/* Remove a cell from the board by marking it as -1 */
void remove_cell(t_game *game, int x, int y) {
	if (x < 0 || x > BOARD_MAX_INDEX || y < 0 || y > BOARD_MAX_INDEX)
		return;
	if (game->table[x][y] == -1)
		return;
	game->table[x][y] = -1;
}

/* Move player in specified direction
   Returns 1 if valid, 0 if invalid */
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

int count_valid_moves(t_game *game, t_player *player, t_player *opponent) {
	int count = 0;
	int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, 
	                         {1, -1}, {-1, -1}, {-1, 1}, {1, 1}};
	
	for (int i = 0; i < 8; i++) {
		int nx = player->x + directions[i][0];
		int ny = player->y + directions[i][1];
		
		/* Check boundaries, removed cells, and opponent position */
		if (nx < 0 || nx > game->width || ny < 0 || ny > game->height)
			continue;
		if (game->table[nx][ny] == -1)
			continue;
		if (nx == opponent->x && ny == opponent->y)
			continue;
		count++;
	}
	return count;
}

void free_game(t_game *game, t_player *player1, t_player *player2) {
	if (game && game->table) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (game->table[i])
				free(game->table[i]);
		}
		free(game->table);
	}
	if (player1)
		free(player1);
	if (player2)
		free(player2);
	if (game)
		free(game);
}

