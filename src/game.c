#include "game.h"
#include "allegro/inline/draw.inl"
#include "allegro/keyboard.h"
#include <math.h>

int exit_game;
struct spritePos player;
struct enemyData enem1;

// int moving = STOP_RIGHT;
int curr_sprite = 0;
int counter = 0;
// BITMAP *player[11];
// BITMAP *enemy1[11];
BITMAP *bg;
BITMAP *tiles;

void input() {
    // readkey();
    // end_game = 1;
    if (key[KEY_ESC]) {
        exit_game = 1;
    }
    if (player.moving < STOPPOS) {
        if (player.moving == MOVING_LEFT || player.moving == PUNCH_LEFT) {
            player.moving = STOP_LEFT;
        } else {
            player.moving = STOP_RIGHT;
        }
    }
    if (key[KEY_SPACE]) {
        if (player.moving == STOP_LEFT) {
            player.moving = PUNCH_LEFT;
        } else {
            player.moving = PUNCH_RIGHT;
        }
    } else if (key[KEY_LEFT]) {
        player.moving = MOVING_LEFT;
    } else if (key[KEY_RIGHT]) {
        player.moving = MOVING_RIGHT;
    }
}

int random_range(unsigned int low, unsigned int high) {
    return low + (rand() % (high - low));
}

int point_distance(unsigned int x, unsigned int targetX) {
    return abs((int)(x - targetX));
}

void enemy_animations(struct enemyData *enem) {
    if (enem->moving == MOVING_RIGHT || enem->moving == MOVING_LEFT) {
        if (enem->curr_sprite == ANIM_WALK1) {
            enem->curr_sprite = ANIM_WALK2;
        } else {
            enem->curr_sprite = ANIM_WALK1;
        }
    } else if (enem->moving == PUNCH_RIGHT || enem->moving == PUNCH_LEFT) {
        enem->curr_sprite = ANIM_PUNCH;
    } else {
        enem->curr_sprite = 0;
    }
}

void enemy_decisions(struct enemyData *enem) {
    if (point_distance(player.x, enem->x) >= 30) {
        if ((counter % 30) == 0) {
            enem->targetX = player.x;
        }
        if (point_distance(enem->targetX, enem->x) >= 30) {
            if (enem->x > enem->targetX) {
                enem->x--;
            } else if (enem->x < enem->targetX) {
                enem->x++;
            }
        }

        if (enem->x > player.x) {
            enem->moving = MOVING_LEFT;
        } else {
            enem->moving = MOVING_RIGHT;
        }
    } else {
        if (enem->moving == MOVING_LEFT) {
            enem->moving = STOP_LEFT;
        } else if (enem->moving == MOVING_RIGHT) {
            enem->moving = STOP_RIGHT;
        }
    }
}

void process() {
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-maker-part-1-player-movement-attacks-and-basic-enemies--cms-26147t
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-maker-part-2-combat-and-basic-enemy-ai--cms-26148t
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-makercombo-attacks-more-ai-and-health-pickups--cms-26471t
    // delete sprite
    if (player.moving == MOVING_RIGHT && player.x < 300) {
        player.x++;
    }
    if (player.moving == MOVING_LEFT && player.x > 1) {
        player.x--;
    }
    if ((counter % 10) == 0) {
        if (player.moving == MOVING_RIGHT || player.moving == MOVING_LEFT) {
            if (player.curr_sprite == ANIM_WALK1) {
                player.curr_sprite = ANIM_WALK2;
            } else {
                player.curr_sprite = ANIM_WALK1;
            }
        } else if (player.moving == PUNCH_RIGHT ||
                   player.moving == PUNCH_LEFT) {
            player.curr_sprite = ANIM_PUNCH;
        } else {
            player.curr_sprite = 0;
        }
    }
    ///// ENEMY

    // enemy tries to go to player
    /*if (!enem1.targetX) {
        enem1.targetX = random_range(player.x - 10, player.x + 10);
    }*/

    enemy_decisions(&enem1);

    if ((counter % 10) == 0) {
        enemy_animations(&enem1);
    }
}

void output() {
    counter++;
    blit(bg, screen, player.x, player.y, player.x, player.y, 40, 40);

    blit(bg, screen, enem1.x, enem1.y, enem1.x, enem1.y, 40, 40);

    // redraw pair or impair?
    if (player.moving & 1) {
        draw_sprite_h_flip(screen, player.sprite[player.curr_sprite], player.x,
                           player.y);
    } else {
        draw_sprite(screen, player.sprite[player.curr_sprite], player.x,
                    player.y);
    }

    // redraw pair or impair?
    if (enem1.moving & 1) {
        draw_sprite_h_flip(screen, enem1.sprite[enem1.curr_sprite], enem1.x,
                           enem1.y);
    } else {
        draw_sprite(screen, enem1.sprite[enem1.curr_sprite], enem1.x, enem1.y);
    }

    if (counter > 319) {
        counter = 0;
    }
}

/**
blit
– Stands for “BLock Transfer”
– Most important graphics function
– Takes a rectangular block of one bitmap and draws it onto
another
**/
