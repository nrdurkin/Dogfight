#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class GameDisplay
{
public:
    GameDisplay(int width, int height) : width(width), height(height), disp(al_create_display(width, height)) {}
    void Register(ALLEGRO_EVENT_QUEUE *queue, Player *player, BulletController *b, EnemyController *e);
    void Draw();
    void End(int score);
    std::pair<int, int> GetDim() { return std::make_pair(width, height); }
    void SetScreenFrame(std::pair<double, double> p)
    {
        screenLeft = p.first;
        screenTop = p.second;
    }

private:
    int width, height;
    double screenLeft = 0, screenTop = 0;
    ALLEGRO_DISPLAY *disp;
    Player *player;
    BulletController *bullets;
    EnemyController *enemies;
};

#endif