#ifndef ENEM_H
#define ENEM_H
#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>

#define MAX_ENEMIES 3
#define VARIANT_JOHNY 0

extern struct enemyData enemies[MAX_ENEMIES];

extern void init_level_enemies(int total_enemies, char first_load);
extern void all_enemy_animations();
extern void all_enemy_decisions(struct spritePos * playr);
extern void all_draw_enemies();
// returns true if player has Y upper than all enemies
extern char player_over_all_enemies(int player_y);
extern void redraw_bg_enemy_positions();
extern void unload_enemies();

#endif