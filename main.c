#include "board_game.h"

void init_game(t_game *game) {
	game->table = malloc(sizeof(int *) * 7);
	for(int i = 0; i < 7; i++) {
		game->table[i] = malloc(sizeof(int) * 7);
		for(int j = 0; j < 7; j++) {
			game->table[i][j] = 0;
		}
	}
	game->length = 6;
	game->width = 6;
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
		player->startY = 6;
		player->x = 3;
		player->y = 6;
		player->type = 2;
	}
}

void CreateTable() {
    int cellSize = 70;
    int spacing = 70;
    int count = 7;
    int grid = cellSize + spacing * (count - 1);
    int startX = (GetScreenWidth() - grid) / 2;
    int startY = (GetScreenHeight() - grid) / 2;
	int fontSize = 20;

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++)
            DrawRectangleLines( startX + spacing * i, startY + spacing * j, cellSize, cellSize, BLACK);
    }

	for (int i = 0; i < count; i++) {
		char num[2];
		int n = i + 1;
		num[0] = '0' + n;
		num[1] = '\0';
	
		int textW = MeasureText(num, fontSize);
		int x = startX + spacing * i + cellSize / 2 - textW / 2;
		int y = startY - fontSize - 10;
		DrawText(num, x, y, fontSize, BLACK);
	}
	
	for (int j = 0; j < count; j++) {
		char letter[2] = { 'A' + j, '\0' };
		int textW = MeasureText(letter, fontSize);
		int x = startX - textW - 10;
		int y = startY + spacing * j + cellSize / 2 - fontSize / 2;
		DrawText(letter, x, y, fontSize, BLACK);
	}
}

void change_table(t_player *player) {
	int cellSize = 70;
	int spacing = 70;
	int count = 7;
	int grid = cellSize + spacing * (count - 1);
	int startX = (GetScreenWidth() - grid) / 2;
	int startY = (GetScreenHeight() - grid) / 2;

	int posX = startX + player->x * spacing;
	int posY = startY + player->y * spacing;

	Color color = (player->type == 2) ? RED : BLUE;
	DrawRectangle(posX + 1, posY + 1, cellSize - 2, cellSize - 2, color);
}


int main(void) {
    InitWindow(800, 800, "Strategic Board Game");
	t_player *player1 = malloc(sizeof(t_player));
	t_player *player2 = malloc(sizeof(t_player));
	t_game *game = malloc(sizeof(t_game));

	init_game(game);
	init_player(player1, 1);
	init_player(player2, 2);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        CreateTable();

		if (IsKeyPressed(KEY_W)) move(game,'w', player2);
		if (IsKeyPressed(KEY_S)) move(game,'s', player2);
		if (IsKeyPressed(KEY_A)) move(game,'a', player2);
		if (IsKeyPressed(KEY_D)) move(game,'d', player2);
		change_table(player1);
		change_table(player2);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
