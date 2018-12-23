/*
 * solver.c
 *
 *  Created on: Dec 18, 2018
 *      Author: Timor Eizenman & Ido Lerer
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#define DEFAULT 0

int is_finished(Board* game) {
	int i = 0, j = 0;
	for (; i < game->board_size; i++)
		for (j = 0; j < game->board_size; j++)
			if (game->current[i][j].value == DEFAULT)
				return 0;
	return 1;
}

int is_value_valid(Board* game, int row, int col, int value) {
	int i = 0, j = 0;
	int rows_to_add = 0, cols_to_add = 0;

	for (; i < row * col; i++) {
		if (game->current[row][i].value == value)
			return 0;
		if (game->current[i][col].value == value)
			return 0;
	}

	rows_to_add = (row / game->block_row) * game->block_row;
	cols_to_add = (col / game->block_col) * game->block_col;

	for (i = rows_to_add; i < rows_to_add + game->block_row; i++)
		for (j = cols_to_add; j < cols_to_add + game->block_col; j++)
			if (game->current[i][j].value == value)
				return 0;

	return 1;
}

int deterministic_backtrack(Board* game) {
	int value = 1;
	int row = 0, col = 0;

	if (is_finished(game) == 1)
		return 1;

	for (; row < game->board_size; row++) {
		for (col = 0; col < game->board_size; col++) {
			if (game->current[row][col].value == DEFAULT && game->current[row][col].isFixed == 0) {
				for (value = 1; value <= game->board_size; value++) {
					if (is_value_valid(game, row, col, value)) {
						game->current[row][col].value = value;
						if (deterministic_backtrack(game))
							return 1;
					}
				}
				game->current[row][col].value = DEFAULT;
				return 0;
			}
		}
	}
	return 1;
}

void find_options(Board* game, int row, int col) {
	int value = 0;

	for (; value < game->board_size; value++)
		if (is_value_valid(game, row, col, value))
			insert_option(&game->current[row][col], value, game->board_size);
}

int randomized_backtrack(Board* game) {
	int value, index, count;
	int row = 0, col = 0, i = 0;

	if (is_finished(game) == 1)
		return 1;

	for (; row < game->board_size; row++) {
		for (col = 0; col < game->board_size; col++) {
			if (game->current[row][col].value == DEFAULT) {
				find_options(game, row, col);
				count = game->current[row][col].countOptions;
				if (count == 1)
					index = 0;

				for (; i <= count; i++) {
					index = rand() % game->current[row][col].countOptions;
					value = game->current[row][col].options[index];
					game->current[row][col].value = value;
					if (randomized_backtrack(game))
						return 1;

					game->current[row][col].countOptions--;
					remove_option(&game->current[row][col], index, game->board_size);
				}

				game->current[row][col].value = DEFAULT;
				return 0;
			}
		}
	}
	return 1;
}


