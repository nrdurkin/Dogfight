#ifndef BULLET_H
#define BULLET_H

#include "Settings.h"
#include <vector>

class Bullet
{
public:
    Bullet(double x, double y, double angle, bool player_bullet);
    void Draw();
    void Tick();
    bool Delete(std::pair<double, double> center);
    double getX() { return posX; }
    double getY() { return posY; }
    double getSize() { return size; }
    bool OriginPlayer() { return player_bullet; } //Need to track if player shot for friendly fire

private:
    double posX, posY, angle, velocity, size;
    bool player_bullet;
};

class BulletController
{
public:
    BulletController() {}
    void Reset();
    void SpawnBullet(double x, double y, double angle, bool player_bullet = FRIENDLY_FIRE);
    void Draw();
    void Tick(std::pair<double, double> center);
    void DeleteBullet(Bullet *b);
    std::vector<Bullet *> GetBullets() { return bullets; }

private:
    std::vector<Bullet *> bullets;
};

#endif