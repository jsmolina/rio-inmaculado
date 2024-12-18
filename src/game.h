#ifndef GAME_H
#define GAME_H

#include "enem.h"
#include "helpers.h"
#include <allegro/keyboard.h>

#define TRANS makecol(255, 0, 255)

// TODO define maximum vertical distance to allow before two objects can no
// longer interact

extern int exit_game; /* flag we'll set to end the game */
extern int level;
extern int level_enemies;
extern struct spritePos player;

extern int counter;
// extern int moving;
// extern BITMAP *player[11];
// extern BITMAP *enemy1[11];
extern BITMAP *bg;
extern char slow_cpu;

extern void input();
extern void process();
extern void output();
extern void increase_level_and_load();
extern void load_level(int lvl);

#endif

/**
Niveles:
1: clases
2: cours navé
3: sala informática: alleycat piscina
4: parking
5: calle
 */