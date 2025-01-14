#ifndef ENEM_H
#define ENEM_H
#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>

#define MAX_ENEMIES 3
#define VARIANT_JOHNY 0

extern enemyData enemies[MAX_ENEMIES];

// initializes enemies on level
extern void init_level_enemies(int total_enemies, int maxX, char first_load);
// Animations for all enemies
extern void all_enemy_animations();
// AI for all enemies
extern void all_enemy_decisions(spritePos * playr);
// draw all enemies
extern void all_draw_enemies();
// returns true if player has Y upper than all enemies
extern char player_over_all_enemies(int player_y);
// true if has any enemy on path
extern char enemy_on_path(int new_player_x, int play_y);
// redraws background over all the enemy positions
extern void redraw_bg_enemy_positions();
// frees up memory sprites for enemies
extern void unload_enemies();

#endif