#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <allegro5/allegro.h>

#include "Display.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Controller
{
public:
    Controller();
    void Begin();

private:
    void ProcessFrame();
    void RegisterSources();
    void Tick();
    void Reset();

    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;

    GameDisplay display;
    BulletController bullets;
    Player player;
    EnemyController enemies;
    int score = 0;
};

#endif