#include <allegro.h>
#include <stdio.h>

#define MAX_ITEMS 8
#define MENU_X 40
#define MENU_Y 60
#define CONFIG_FILE "sound.cfg"

enum {MAIN_MENU, SOUND_MENU, MIDI_MENU, SB_MENU};
enum {NONE, SB, SB10, SB15, SB20, SBP, SB16, ESS};

// Variables de configuración
struct {
    int sound_card;
    int midi_card;
    int sb_port;
    int sb_dma ;
    int sb_irq;
    int sb_freq ;
    int flip_pan;
} config;

int menu_state = MAIN_MENU;
int selected_item = 0;
int exit_program = 0;

const char *sound_cards[] = {
    "Ninguno", "Sound Blaster", "SB Pro 1.0", "SB Pro 1.5",
    "SB Pro 2.0", "SB Pro", "SB16", "ESS AudioDrive", NULL
};

const char *midi_devices[] = {
    "Ninguno", "OPL", "OPL2", "OPLX", "OPL3", 
    "Sound Blaster", "MPU-401", "Dispositivo Digital", "AWE32", NULL
};

void draw_main_menu() {
    textout_centre_ex(screen, font, "CONFIGURACION DE SONIDO", 
                     SCREEN_W/2, 20, 0xFFFFFF, -1);
    
    char buffer[50];
    for(int i=0; i<4; i++) {
        int color = (i == selected_item) ? 0xFFFFFF : 0xFFFFAA;
        
        switch(i) {
            case 0: sprintf(buffer, "Tarjeta de sonido: %s", sound_cards[config.sound_card]); break;
            case 1: sprintf(buffer, "Dispositivo MIDI: %s", midi_devices[config.midi_card]); break;
            case 2: sprintf(buffer, "Config. SB [%03X,%d,%d]", config.sb_port, config.sb_dma, config.sb_irq); break;
            case 3: sprintf(buffer, "Guardar y Salir"); break;
        }
        
        textout_ex(screen, font, (i == selected_item) ? ">" : " ", 
                  MENU_X-10, MENU_Y + i*20, color, -1);
        textout_ex(screen, font, buffer, MENU_X, MENU_Y + i*20, color, -1);
    }
}

void handle_input() {
    static int debounce = 0;
    
    if(key[KEY_UP] && !debounce) {
        selected_item = (selected_item - 1 + 4) % 4;
        debounce = 10;
    }
    if(key[KEY_DOWN] && !debounce) {
        selected_item = (selected_item + 1) % 4;
        debounce = 10;
    }
    
    if(key[KEY_ENTER]) {
        switch(menu_state) {
            case MAIN_MENU:
                if(selected_item == 3) exit_program = 1;
                else menu_state = selected_item + 1;
                break;
                
            case SOUND_MENU:
                config.sound_card = (config.sound_card + 1) % 8;
                break;
                
            case MIDI_MENU:
                config.midi_card = (config.midi_card + 1) % 9;
                break;
        }
        rest(150);
    }
    
    if(key[KEY_ESC]) {
        if(menu_state != MAIN_MENU) menu_state = MAIN_MENU;
        else exit_program = 1;
    }
    
    if(debounce > 0) debounce--;
}

void save_config() {
    set_config_file(CONFIG_FILE);
    
    set_config_int("sound", "sound_card", config.sound_card);
    set_config_int("midi", "midi_card", config.midi_card);
    set_config_hex("sb", "port", config.sb_port);
    set_config_int("sb", "dma", config.sb_dma);
    set_config_int("sb", "irq", config.sb_irq);
    set_config_int("sb", "freq", config.sb_freq);
}

void load_config() {
    set_config_file(CONFIG_FILE);
    
    config.sound_card = get_config_int("sound", "sound_card", DIGI_SB16);
    config.midi_card = get_config_int("midi", "midi_card", MIDI_OPL3);
    config.sb_port = get_config_hex("sb", "port", 0x220);
    config.sb_dma = get_config_int("sb", "dma", 1);
    config.sb_irq = get_config_int("sb", "irq", 5);
    config.sb_freq = get_config_int("sb", "freq", 4);
}

int main() {
    allegro_init();
    install_keyboard();
    set_color_depth(8);
    set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0);
    
    //load_config();
    
    while(!exit_program) {
        clear(screen);
        
        switch(menu_state) {
            case MAIN_MENU:
                draw_main_menu();
                break;
                
            case SOUND_MENU:
                textout_centre_ex(screen, font, "SELECCION DE TARJETA DE SONIDO", 
                                SCREEN_W/2, 20, 0xFFFFFF, -1);
                for(int i=0; i<8; i++) {
                    int color = (i == config.sound_card) ? 0xFFFFFF : 0xFFFFAA;
                    textprintf_centre_ex(screen, font, SCREEN_W/2, 60 + i*20,
                                       color, -1, "%s", sound_cards[i]);
                }
                break;
                
            case MIDI_MENU:
                textout_centre_ex(screen, font, "SELECCION DE DISPOSITIVO MIDI", 
                                SCREEN_W/2, 20, 0xFFFFFF, -1);
                for(int i=0; i<9; i++) {
                    int color = (i == config.midi_card) ? 0xFFFFFF : 0xFFFFAA;
                    textprintf_centre_ex(screen, font, SCREEN_W/2, 60 + i*20,
                                       color, -1, "%s", midi_devices[i]);
                }
                break;
        }
        
        handle_input();
        vsync();
    }
    
    save_config();
    allegro_exit();
    return 0;
}
END_OF_MAIN()
