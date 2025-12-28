#include "board_game.h"

/* Copy entire game state for AI simulation*/
void copy_game_state(t_game *dest, t_game *src, t_player *p1_dest, t_player *p1_src, t_player *p2_dest, t_player *p2_src) {
	if (!dest || !src)
		return;

	dest->height = src->height;
	dest->width = src->width;
	dest->cellSize = src->cellSize;
	dest->spacing = src->spacing;
	dest->count = src->count;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++)
			dest->table[i][j] = src->table[i][j];
	}

	if (p1_dest && p1_src)
		*p1_dest = *p1_src;
	if (p2_dest && p2_src)
		*p2_dest = *p2_src;
}

/* Get all valid move directions for a player*/
void get_valid_move_directions(t_game *game, t_player *player, t_player *opponent, char moves[8], int *count) {
	static const int dirs[8][2] = {
		{0, -1}, {0, 1}, {-1, 0}, {1, 0},
		{1, -1}, {-1, -1}, {-1, 1}, {1, 1}
	};
	static const char dir_keys[8] = { 'w', 's', 'a', 'd', 'e', 'q', 'z', 'x' };

	int n = 0;
	for (int i = 0; i < 8; i++) {
		int nx = player->x + dirs[i][0];
		int ny = player->y + dirs[i][1];

		if (nx < 0 || nx > game->width || ny < 0 || ny > game->height)
			continue;
		if (game->table[nx][ny] == -1)
			continue;
		if (opponent && nx == opponent->x && ny == opponent->y)
			continue;

		if (moves)
			moves[n++] = dir_keys[i];
		else
			n++;
	}
	if (count)
		*count = n;
}

/* Get all cells that can be removed*/
void get_valid_remove_cells(t_game *game, t_player *p1, t_player *p2, int cells[][2], int *count) {
	int n = 0;
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			if (game->table[x][y] == -1)
				continue;
			if ((p1 && x == p1->x && y == p1->y) || (p2 && x == p2->x && y == p2->y))
				continue;

			if (cells) {
				cells[n][0] = x;
				cells[n][1] = y;
			}
			n++;
		}
	}
	if (count)
		*count = n;
}

/* Evaluate board position for AI
   Considers mobility and center control */
int evaluate(t_game *game, t_player *ai, t_player *human) {
	int ai_moves = count_valid_moves(game, ai, human);
	int human_moves = count_valid_moves(game, human, ai);
	
	/* Terminal states */
	if (ai_moves == 0)
		return -10000;  /* AI loses */
	if (human_moves == 0)
		return 10000;   /* AI wins */
	
	/* Mobility: more moves = better */
	int mobility_score = (ai_moves - human_moves) * 100;
	
	/* Position: closer to center = better */
	int center = BOARD_SIZE / 2;
	int ai_center_dist = abs(ai->x - center) + abs(ai->y - center);
	int human_center_dist = abs(human->x - center) + abs(human->y - center);
	int position_score = (human_center_dist - ai_center_dist) * 10;
	
	return mobility_score + position_score;
}

/* Minimax algorithm with alpha-beta pruning for AI decision making
   Searches game tree to find best move and cell to remove */
int minimax_alpha_beta(t_game *game, t_player *ai, t_player *human, int depth, int alpha, int beta, int maximizing, char *best_move, int *best_remove_x, int *best_remove_y) {
	if (depth == 0)
		return evaluate(game, ai, human);
	
	/* Set current player based on turn */
	t_player *current_player;
	t_player *opponent;
	if (maximizing) {
		current_player = ai;
		opponent = human;
	} else {
		current_player = human;
		opponent = ai;
	}
	
	/* Check for game over */
	int move_count;
	get_valid_move_directions(game, current_player, opponent, NULL, &move_count);
	if (move_count == 0) {
		if (maximizing)
			return -10000;
		else
			return 10000;
	}
	
	/* Maximizing player (AI) */
	if (maximizing) {
		int max_eval = INT_MIN;
		char valid_moves[8];
		int move_cnt;
		get_valid_move_directions(game, ai, human, valid_moves, &move_cnt);
		
		/* Try each possible move */
		for (int m = 0; m < move_cnt; m++) {
			t_game temp_game;
			temp_game.table = malloc(sizeof(int *) * BOARD_SIZE);
			for (int i = 0; i < BOARD_SIZE; i++)
				temp_game.table[i] = malloc(sizeof(int) * BOARD_SIZE);

			t_player temp_ai, temp_human;
			copy_game_state(&temp_game, game, &temp_ai, ai, &temp_human, human);
			
			/* Simulate move */
			move(&temp_game, valid_moves[m], &temp_ai, &temp_human);
			
			/* Get possible cells to remove */
			int remove_cells[49][2];
			int remove_cnt;
			get_valid_remove_cells(&temp_game, &temp_ai, &temp_human, remove_cells, &remove_cnt);
			
			/* Try each cell removal */
			for (int r = 0; r < remove_cnt; r++) {
				t_game temp_game2;
				temp_game2.table = malloc(sizeof(int *) * BOARD_SIZE);
				for (int i = 0; i < BOARD_SIZE; i++)
					temp_game2.table[i] = malloc(sizeof(int) * BOARD_SIZE);

				t_player temp_ai2, temp_human2;
				copy_game_state(&temp_game2, &temp_game, &temp_ai2, &temp_ai, &temp_human2, &temp_human);
				
				/* Simulate cell removal */
				remove_cell(&temp_game2, remove_cells[r][0], remove_cells[r][1]);
				
				/* Recursive evaluation */
				int eval = minimax_alpha_beta(&temp_game2, &temp_ai2, &temp_human2, depth - 1, alpha, beta, 0, NULL, NULL, NULL);
				
				/* Clean up */
				for (int i = 0; i < BOARD_SIZE; i++)
					free(temp_game2.table[i]);
				free(temp_game2.table);
				
				/* Update best move */
				if (eval > max_eval) {
					max_eval = eval;
					if (best_move && best_remove_x && best_remove_y) {
						*best_move = valid_moves[m];
						*best_remove_x = remove_cells[r][0];
						*best_remove_y = remove_cells[r][1];
					}
				}
				
				/* Alpha-beta pruning */
				if (alpha < eval)
					alpha = eval;
				if (beta <= alpha)
					break;
			}
			
			for (int i = 0; i < BOARD_SIZE; i++)
				free(temp_game.table[i]);
			free(temp_game.table);
			
			if (beta <= alpha)
				break;
		}
		return max_eval;
	} else {
		/* Minimizing player (Human) */
		int min_eval = INT_MAX;
		char valid_moves[8];
		int move_cnt;
		get_valid_move_directions(game, human, ai, valid_moves, &move_cnt);
		
		/* Try each possible move */
		for (int m = 0; m < move_cnt; m++) {
			/* Create temp game */
			t_game temp_game;
			temp_game.table = malloc(sizeof(int *) * BOARD_SIZE);
			for (int i = 0; i < BOARD_SIZE; i++)
				temp_game.table[i] = malloc(sizeof(int) * BOARD_SIZE);

			t_player temp_ai, temp_human;
			copy_game_state(&temp_game, game, &temp_ai, ai, &temp_human, human);
			
			/* Simulate move */
			move(&temp_game, valid_moves[m], &temp_human, &temp_ai);
			
			int remove_cells[49][2];
			int remove_cnt;
			get_valid_remove_cells(&temp_game, &temp_ai, &temp_human, remove_cells, &remove_cnt);
			
			/* Try each cell removal */
			for (int r = 0; r < remove_cnt; r++) {
				t_game temp_game2;
				temp_game2.table = malloc(sizeof(int *) * BOARD_SIZE);
				for (int i = 0; i < BOARD_SIZE; i++)
					temp_game2.table[i] = malloc(sizeof(int) * BOARD_SIZE);

				t_player temp_ai2, temp_human2;
				copy_game_state(&temp_game2, &temp_game, &temp_ai2, &temp_ai, &temp_human2, &temp_human);
				
				/* Simulate cell removal */
				remove_cell(&temp_game2, remove_cells[r][0], remove_cells[r][1]);
				
				/* Recursive evaluation */
				int eval = minimax_alpha_beta(&temp_game2, &temp_ai2, &temp_human2, depth - 1, alpha, beta, 1, NULL, NULL, NULL);
				
				/* Clean up */
				for (int i = 0; i < BOARD_SIZE; i++)
					free(temp_game2.table[i]);
				free(temp_game2.table);
				
				/* Update minimum */
				if (min_eval > eval)
					min_eval = eval;
				/* Alpha-beta pruning */
				if (beta > eval)
					beta = eval;
				if (beta <= alpha)
					break;
			}
			
			for (int i = 0; i < BOARD_SIZE; i++)
				free(temp_game.table[i]);
			free(temp_game.table);
			
			if (beta <= alpha)
				break;
		}
		return min_eval;
	}
}
