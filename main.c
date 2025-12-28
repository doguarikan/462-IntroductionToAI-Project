#include "board_game.h"

void init_game(t_game *game) {
	game->table = malloc(sizeof(int *) * BOARD_SIZE);
	for(int i = 0; i < BOARD_SIZE; i++) {
		game->table[i] = malloc(sizeof(int) * BOARD_SIZE);
		for(int j = 0; j < BOARD_SIZE; j++)
			game->table[i][j] = 0;
	}
	game->height = BOARD_MAX_INDEX;
	game->width = BOARD_MAX_INDEX;
	game->cellSize = CELL_SIZE;
	game->spacing = SPACING;
	game->count = BOARD_SIZE;
}

void init_player(t_player *player, int type) {
	if(type == 1) {
		player->startX = 3;
		player->startY = 0;
		player->x = 3;
		player->y = 0;
		player->type = 1;
	}
	else {
		player->startX = 3;
		player->startY = BOARD_MAX_INDEX;
		player->x = 3;
		player->y = BOARD_MAX_INDEX;
		player->type = 2;
	}
}

void CreateTable(t_game *game) {
    int grid = game->cellSize + game->spacing * (game->count - 1);
    int startX = (GetScreenWidth() - grid) / 2;
    int startY = (GetScreenHeight() - grid) / 2;
	int fontSize = 20;

    for (int i = 0; i < game->count; i++) {
        for (int j = 0; j < game->count; j++) {
			if (game->table[i][j] == -1) {
				int posX = startX + i * game->spacing;
				int posY = startY + j * game->spacing;
				DrawRectangle(posX + 1, posY + 1, game->cellSize - 2, game->cellSize - 2, DARKGRAY);
			}
            DrawRectangleLines( startX + game->spacing * i, startY + game->spacing * j, game->cellSize, game->cellSize, BLACK);
		}
    }

	for (int i = 0; i < game->count; i++) {
		char num[2];
		int n = i + 1;
		num[0] = '0' + n;
		num[1] = '\0';
	
		int textW = MeasureText(num, fontSize);
		int x = startX + game->spacing * i + game->cellSize / 2 - textW / 2;
		int y = startY - fontSize - 10;
		DrawText(num, x, y, fontSize, BLACK);
	}
	
	for (int j = 0; j < game->count; j++) {
		char letter[2] = { 'A' + j, '\0' };
		int textW = MeasureText(letter, fontSize);
		int x = startX - textW - 10;
		int y = startY + game->spacing * j + game->cellSize / 2 - fontSize / 2;
		DrawText(letter, x, y, fontSize, BLACK);
	}
}

void change_table(t_game *game, t_player *player) {
	int grid = game->cellSize + game->spacing * (game->count - 1);
	int startX = (GetScreenWidth() - grid) / 2;
	int startY = (GetScreenHeight() - grid) / 2;

	int posX = startX + player->x * game->spacing;
	int posY = startY + player->y * game->spacing;

	Color color;
	if (player->type == 2)
		color = RED;
	else
		color = BLUE;
	DrawRectangle(posX + 1, posY + 1, game->cellSize - 2, game->cellSize - 2, color);
}

int main(void) {
    InitWindow(800, 800, "Board Game");
	t_player *player1 = malloc(sizeof(t_player));
	t_player *player2 = malloc(sizeof(t_player));
	t_game *game = malloc(sizeof(t_game));
	int turn = 1;
	int moved = 0;
	int ai_thinking = 0;
	char ai_best_move = 'w';
	int ai_best_remove_x = 0;
	int ai_best_remove_y = 0;
	int game_over = 0;
	int winner = 0;

	init_game(game);
	init_player(player1, 1);
	init_player(player2, 2);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        CreateTable(game);

		if(turn == 1 && moved == 0 && !ai_thinking && !game_over) {
			if (count_valid_moves(game, player1, player2) == 0) {
				game_over = 1;
				winner = 2;
			} else {
				ai_thinking = 1;
				
				minimax_alpha_beta(game, player1, player2, 3, INT_MIN, INT_MAX, 1, &ai_best_move, &ai_best_remove_x, &ai_best_remove_y);
				
				move(game, ai_best_move, player1, player2);
				moved = 1;
				ai_thinking = 0;
			}
		}
		
		if(turn == 1 && moved == 1 && !game_over) {
			remove_cell(game, ai_best_remove_x, ai_best_remove_y);
			if (count_valid_moves(game, player2, player1) == 0) {
				game_over = 1;
				winner = 1;
			} else {
				turn = 2;
				moved = 0;
			}
		}

		if(turn == 2 && moved == 0 && !game_over) {
			if (count_valid_moves(game, player2, player1) == 0) {
				game_over = 1;
				winner = 1;
			} else if (IsKeyPressed(KEY_W)) {
				if (move(game,'w', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_S)) {
				if (move(game,'s', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_A)) {
				if (move(game,'a', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_D)) {
				if (move(game,'d', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_Q)) {
				if (move(game,'q', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_E)) {
				if (move(game,'e', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_Z)) {
				if (move(game,'z', player2, player1))
					moved = 1;
			}
			else if (IsKeyPressed(KEY_X)) {
				if (move(game,'x', player2, player1))
					moved = 1;
			}
		}
		
		if(turn == 2 && moved == 1 && !game_over) {
			Vector2 mousePos = GetMousePosition();
			int grid = game->cellSize + game->spacing * (game->count - 1);
			int startX = (GetScreenWidth() - grid) / 2;
			int startY = (GetScreenHeight() - grid) / 2;
			
			if (mousePos.x >= startX && mousePos.x < startX + grid &&
				mousePos.y >= startY && mousePos.y < startY + grid) {
				int col = (mousePos.x - startX) / game->spacing;
				int row = (mousePos.y - startY) / game->spacing;
				
				if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE) {
					if (IsKeyPressed(KEY_SPACE)) {
						if (game->table[col][row] != -1 &&
							!(col == player1->x && row == player1->y) &&
							!(col == player2->x && row == player2->y)) {
							remove_cell(game, col, row);
							if (count_valid_moves(game, player1, player2) == 0) {
								game_over = 1;
								winner = 2;
							} else {
								turn = 1;
								moved = 0;
							}
						}
					}
				}
			}
		}
		
		change_table(game, player1);
		change_table(game, player2);
		
		if (game_over) {
			const char *winner_text;
			Color winner_color;
			if (winner == 1) {
				winner_text = "AI (Blue) Wins!";
				winner_color = BLUE;
			} else {
				winner_text = "Human (Red) Wins!";
				winner_color = RED;
			}
			
			int grid = game->cellSize + game->spacing * (game->count - 1);
			int startX = (GetScreenWidth() - grid) / 2;
			int startY = (GetScreenHeight() - grid) / 2;
			
			int fontSize = 40;
			int textWidth = MeasureText(winner_text, fontSize);
			int x = startX + (grid - textWidth) / 2;
			int y = startY - fontSize - 30;
			
			DrawText(winner_text, x, y, fontSize, winner_color);
		}
		
        EndDrawing();
    }

    free_game(game, player1, player2);
    CloseWindow();
    return 0;
}
