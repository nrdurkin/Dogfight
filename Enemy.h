#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"
#include <vector>

class Enemy
{
public:
    Enemy(double x, double y, BulletController &bullets);
    void Draw(std::pair<double, double> center, double screenLeft, double screenTop);
    void Tick(std::pair<double, double> target);
    void Fire();
    bool Collide();

private:
    void DrawHitbox();
    bool Offscreen(double left, double top);
    void DrawIndicator(std::pair<double, double> center, double screenLeft, double screenTop);
    double posX, posY, velocity, angle, size;
    double fire_rate, cooldown;
    BulletController &bullets;
};

class EnemyController
{
public:
    EnemyController(BulletController &bullets) : bullets(bullets) {}
    void Reset();
    void SpawnEnemy(double x, double y);
    void Tick(std::pair<double, double> center);
    void Draw(std::pair<double, double> center, double screenLeft, double screenTop);
    int Collide();

private:
    void SpawnWave(std::pair<double, double> center);
    std::vector<Enemy *> enemies;
    BulletController &bullets;
    int wave = 0, trigger = 0;
};

#endif