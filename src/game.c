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

    if (player.y_moving < STOPPOS) {
        player.y_moving = STOPPOS;
    }

    // now check again keys
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

    if (key[KEY_UP]) {
        player.y_moving = MOVING_UP;
    } else if (key[KEY_DOWN]) {
        player.y_moving = MOVING_DOWN;
    }
}

int random_range(unsigned int low, unsigned int high) {
    return low + (rand() % (high - low));
}

int point_distance(unsigned int x, unsigned int targetX) {
    return abs((int)(x - targetX));
}

void enemy_animations(struct enemyData *enem) {
    if (enem->is_hit > 0) {
        enem->curr_sprite = ANIM_HITTED;
        enem->is_hit--;
        return;
    }

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
    int distance;
    int x_distance;
    // do not take decisions: you are hitted
    if (enem->is_hit > 0) {
        return;
    }

    x_distance = point_distance(player.x, enem->x);
    // check hits
    if (player.moving == PUNCH_LEFT && enem->x <= player.x &&
        x_distance <= 26) {
        enem1.is_hit = HIT_DURATION;
    }
    if (player.moving == PUNCH_RIGHT && player.x <= enem->x &&
        x_distance <= 26) {
        enem1.is_hit = HIT_DURATION;
    }

    // check movements
    if (x_distance >= 26) {
        if ((counter % 30) == 0) {
            enem->targetX = player.x;
        }
        if (x_distance >= 26) {
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
        if (point_distance(player.y, enem->y) >= 2 && (counter % 2) == 0) {
            if (enem->y > player.y) {
                enem->y_moving = MOVING_UP;
                enem->y--;
            } else {
                enem->y_moving = MOVING_DOWN;
                enem->y++;
            }
        } else {
            enem->y_moving = STOPPOS;

            if (enem->moving == MOVING_LEFT) {
                enem->moving = STOP_LEFT;
            } else if (enem->moving == MOVING_RIGHT) {
                enem->moving = STOP_RIGHT;
            }
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
    } else if (player.moving == MOVING_LEFT && player.x > 1) {
        player.x--;
    }

    if (player.y_moving == MOVING_DOWN && player.y < 151) {
        player.y++;
    } else if (player.y_moving == MOVING_UP && player.y > 140) {
        player.y--;
    }

    if ((counter % 10) == 0) {
        if (player.moving == MOVING_RIGHT || player.moving == MOVING_LEFT ||
            player.y_moving == MOVING_UP || player.y_moving == MOVING_DOWN) {
            if (player.curr_sprite == ANIM_WALK1) {
                player.curr_sprite = ANIM_WALK2;
            } else {
                player.curr_sprite = ANIM_WALK1;
            }
        } else if (player.moving == PUNCH_RIGHT ||
                   player.moving == PUNCH_LEFT) {
            player.curr_sprite = ANIM_PUNCH;
            // TODO 15/10: sprite de hit
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

void draw_player() {
    // redraw pair or impair?
    if (player.moving & 1) {
        draw_sprite_h_flip(screen, player.sprite[player.curr_sprite], player.x,
                           player.y);
    } else {
        draw_sprite(screen, player.sprite[player.curr_sprite], player.x,
                    player.y);
    }
}

void draw_enemies() {
    // redraw pair or impair?
    if (enem1.moving & 1) {
        draw_sprite_h_flip(screen, enem1.sprite[enem1.curr_sprite], enem1.x,
                           enem1.y);
    } else {
        draw_sprite(screen, enem1.sprite[enem1.curr_sprite], enem1.x, enem1.y);
    }
}

void output() {
    counter++;
    // clean
    blit(bg, screen, player.x, 120, player.x, 120, 40, 80);
    blit(bg, screen, enem1.x, 120, enem1.x, 120, 40, 80);
    // draw in order depending on Y
    if (player.y < enem1.y) {
        draw_player();
        draw_enemies();
    } else {
        draw_enemies();
        draw_player();
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
