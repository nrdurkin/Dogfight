#include "Display.h"
#include "Colors.h"

#include <allegro5/allegro_font.h>
#include <string>

void GameDisplay::Register(ALLEGRO_EVENT_QUEUE *queue, Player *p, BulletController *b, EnemyController *e)
{
    al_register_event_source(queue, al_get_display_event_source(disp));
    player = p, bullets = b, enemies = e;
}

void GameDisplay::Draw()
{
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);

    al_clear_to_color(BLUE);

    SetScreenFrame(player->Draw());
    bullets->Draw();
    enemies->Draw(player->GetPos(), screenLeft, screenTop);

    al_flip_display();
}

void GameDisplay::End(int score)
{
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);

    al_clear_to_color(BLUE);

    ALLEGRO_FONT *roboto = al_load_font("roboto.ttf", 72, 0);
    al_draw_text(roboto, BLACK, width / 2, 400, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    std::string msg = "YOUR SCORE: " + std::to_string(score);
    al_draw_text(roboto, BLACK, width / 2, 600, ALLEGRO_ALIGN_CENTER, msg.c_str());
    roboto = al_load_font("roboto.ttf", 40, 0);
    al_draw_text(roboto, BLACK, width / 2, 800, ALLEGRO_ALIGN_CENTER, "Press R to restart");

    al_flip_display();
}