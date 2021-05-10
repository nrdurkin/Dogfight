#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include "Bullet.h"

class Player
{
public:
    Player(std::pair<int, int> dims, BulletController &bullets);
    void Reset();
    std::pair<double, double> Draw();
    void Tick();
    bool Collide();
    std::pair<double, double> GetPos() { return std::make_pair(posX, posY); }
    int GetHP() { return HP; }

private:
    void DrawLocalGrid();
    void DrawHitbox();
    void Fire();
    double posX, posY, frameX, frameY, velocity, angle, size;
    int cooldown, fire_rate;
    std::pair<int, int> dims;
    BulletController &bullets;
    int HP, maxHP;
};

#endif