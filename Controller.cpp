#include "Controller.h"
#include "Settings.h"
#include "Player.h"
#include "KeyTracker.h"
#include "Bullet.h"

Controller::Controller() : timer(al_create_timer(1.0 / FPS)),
                           queue(al_create_event_queue()),
                           display(DISPLAY_WIDTH, DISPLAY_HEIGHT),
                           bullets(),
                           player(Player(display.GetDim(), bullets)),
                           enemies(bullets)
{
    RegisterSources();
}

void Controller::RegisterSources()
{
    al_register_event_source(queue, al_get_keyboard_event_source());
    display.Register(queue, &player, &bullets, &enemies);
    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void Controller::Begin()
{
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            KeyTracker::get().KeyDown(event.keyboard.keycode);
        if (event.type == ALLEGRO_EVENT_KEY_UP)
            KeyTracker::get().KeyUp(event.keyboard.keycode);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || KeyTracker::get().IsDown(ALLEGRO_KEY_ESCAPE))
            break;

        if (event.type == ALLEGRO_EVENT_TIMER)
            ProcessFrame();
    }
}

void Controller::ProcessFrame()
{
    if (player.GetHP() == 0)
    {
        display.End(score);
        if (KeyTracker::get().IsDown(ALLEGRO_KEY_R))
            Reset();
    }
    else
    {
        Tick();
        display.Draw();
    }
}

void Controller::Reset()
{
    player.Reset();
    enemies.Reset();
    bullets.Reset();
    score = 0;
}

void Controller::Tick()
{
    player.Tick();
    bullets.Tick(player.GetPos());
    enemies.Tick(player.GetPos());

    player.Collide();
    score += enemies.Collide();
}