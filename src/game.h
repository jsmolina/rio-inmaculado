#ifndef GAME_H
#define GAME_H

#include "enem.h"
#include "helpers.h"
#include <allegro/keyboard.h>

#define TRANS makecol(255, 0, 255)
#define LIFEBAR 11
#define MENU 0
#define GAME_OVER 250
#define MISIFU_ALLEY 15
#define MISIFU_CHEESE 16
#define CASTIGO_FINALIZADO 20
#define MIGUEL_WALK_CYCLE 2


// TODO define maximum vertical distance to allow before two objects can no
// longer interact

extern int exit_game; /* flag we'll set to end the game */
extern unsigned char level;
extern unsigned char sub_level;
extern unsigned char next_level;
extern unsigned char starting_level_counter;
extern spritePos player;
extern char locked_elevator;
extern char urinated;
extern char coursnave_completed;
extern char yellow_key;
extern char blue_key;
extern LevelData levels[TOTAL_LEVELS];
extern BITMAP *player_head, *player_lifebar, *girl, *key_sprite, *key_sprite_blue;
extern MIDI *music;
extern SAMPLE *alleytheme;
extern SAMPLE *hit;
extern SAMPLE *dog_theme;

extern int counter;
extern BITMAP *bg;
extern char slow_cpu;


// processes user input
void input();
// processes state changes, e.g., based on user input
void process();
// syncs current state to the screen
void output();
// setups next level
void increase_level_and_load();
// opens the levels.nfo file which includes actionable parameters for each level
void load_levels();
// opens the current playable level
void load_level();
void draw_lives();
void draw_lifebar();

#endif

/**
Niveles:
1: clases
2: cours navé
3: sala informática: alleycat piscina
4: parking
5: calle
 */