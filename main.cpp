#define ALLEGRO_NO_MAGIC_MAIN
#include "Controller.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int real_main(int argc, char **argv)
{
    Controller game;
    game.Begin();
    return 0;
}

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    return al_run_main(argc, argv, real_main);
}