#include "game.h"

int exit_game; 
int x;
int y;
int curr_sprite = 0;
BITMAP *player[11];

void input() {
    // readkey();
    // end_game = 1;
    if (key[KEY_SPACE]) {
        exit_game = 1;
    }
}

void process() {

}

void output() {
    x++;
    circlefill (screen, x - 1, y, 5, TRANS); /* erase from last place */
    circlefill (screen, x, y, 5, 15);        /* draw first time */

    if ((x % 10) == 0) {
        if (curr_sprite == 0) {
            curr_sprite = 1;
        } else {
            curr_sprite = 0;
        }
         
    }
    draw_sprite(screen, player[curr_sprite], 100, 20);
    
    if (x > 319) {
        x = 0;
    }
}
