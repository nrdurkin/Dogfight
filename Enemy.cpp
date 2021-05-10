#include "Enemy.h"
#include "Colors.h"
#include "Settings.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Enemy::Enemy(double x, double y, BulletController &bullets) : posX(x),
                                                              posY(y),
                                                              velocity(ENEMY_SPEED),
                                                              angle(0),
                                                              size(ENEMY_SIZE),
                                                              fire_rate(ENEMY_FIRE_RATE),
                                                              cooldown(2 * fire_rate),
                                                              bullets(bullets) {}

void Enemy::Draw(std::pair<double, double> center, double screenLeft, double screenTop)
{
    //Coordinates for a triangle
    double x1 = 16 * size + posX,
           y1 = -20 * size + posY,
           x2 = -16 * size + posX,
           y2 = -20 * size + posY,
           x3 = posX,
           y3 = 20 * size + posY;

    auto rotate_point = [](double &x, double &y, double posX, double posY, double angle) {
        double temp = ((x - posX) * cos(angle) - (y - posY) * sin(angle)) + posX;
        y = ((x - posX) * sin(angle) + (y - posY) * cos(angle)) + posY;
        x = temp;
    };
    //rotate all of the coordinates around the center of the enemy
    rotate_point(x1, y1, posX, posY, angle);
    rotate_point(x2, y2, posX, posY, angle);
    rotate_point(x3, y3, posX, posY, angle);

    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, BLACK);

    if (Offscreen(screenLeft, screenTop))
        DrawIndicator(center, screenLeft, screenTop);

    if (DEBUG)
        DrawHitbox();
}

bool Enemy::Offscreen(double left, double top)
{
    return posX > left + DISPLAY_WIDTH || posX < left ||
           posY > top + DISPLAY_HEIGHT || posY < top;
}

void Enemy::DrawHitbox()
{
    al_draw_circle(posX - 5 * sin(angle + M_PI), posY + 5 * cos(angle + M_PI), 20 * size, WHITE, 1);
}

//If the enemy is off the side of the screen, find the closest point on screen
//and draw a red dot to signal direction
void Enemy::DrawIndicator(std::pair<double, double> center, double screenLeft, double screenTop)
{
    double offset = 20;
    //coordinates of top left and bottom right for recurring use
    double p1 = screenLeft + offset, p2 = screenTop + offset, p3 = screenLeft + DISPLAY_WIDTH - offset, p4 = screenTop + DISPLAY_HEIGHT - offset;

    auto intersection = [](double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
        double m1 = (y2 - y1) / (x2 - x1);
        double c1 = y1 - m1 * x1;
        if (x3 == x4)
            x3 += .0001;
        double m2 = (y4 - y3) / (x4 - x3);
        double c2 = y4 - m2 * x4;

        double intersection_X = (c2 - c1) / (m1 - m2);
        double intersection_Y = m1 * intersection_X + c1;
        return std::make_pair(intersection_X, intersection_Y);
    };

    auto valid = [p1, p2, p3, p4](std::pair<double, double> in) {
        return in.first >= p1 - 1 && in.second >= p2 - 1 && in.first <= p3 + 1 && in.second <= p4 + 1;
    };

    //Check 4 candidates for indicator location based on 4 sides.
    auto candidate1 = intersection(posX, posY, center.first, center.second, p1, p2, p3, p2);
    auto candidate2 = intersection(posX, posY, center.first, center.second, p1, p2, p1, p4);
    auto candidate3 = intersection(posX, posY, center.first, center.second, p3, p2, p3, p4);
    auto candidate4 = intersection(posX, posY, center.first, center.second, p1, p4, p3, p4);

    if (valid(candidate1) && posY < center.second)
        al_draw_filled_circle(candidate1.first, candidate1.second, 4, RED);
    if (valid(candidate2) && posX < center.first)
        al_draw_filled_circle(candidate2.first, candidate2.second, 4, RED);
    if (valid(candidate3) && posX > center.first)
        al_draw_filled_circle(candidate3.first, candidate3.second, 4, RED);
    if (valid(candidate4) && posY > center.second)
        al_draw_filled_circle(candidate4.first, candidate4.second, 4, RED);
}

void Enemy::Tick(std::pair<double, double> player)
{
    //Set a to range [b,c]
    auto clamp = [](double &a, double b, double c) {
        a = (a < b) ? b : ((a > c) ? c : a);
        return a;
    };

    posX += velocity * sin(angle + M_PI);
    posY -= velocity * cos(angle + M_PI);

    auto pointAngle = [](double x1, double y1,
                         double x2, double y2,
                         double x3, double y3) { return atan2(y2 - y1, x2 - x1) - atan2(y3 - y2, x3 - x2); };

    //Find where we have to rotate to be pointing at the player and rotate there
    double target = pointAngle(player.first, player.second, posX, posY,
                               posX + 100 * sin(angle),
                               posY - 100 * cos(angle));
    if (target > 2.8 || target < -2.8)
        target = .1;
    clamp(target, -.1, .1);

    angle += target;

    Fire();
}

void Enemy::Fire()
{
    cooldown--;
    if (cooldown < 0)
        cooldown = 0;
    if (cooldown == 0)
    {
        bullets.SpawnBullet(posX - sin(angle) * 20 * size, posY + cos(angle) * 20 * size, angle + M_PI);
        cooldown = fire_rate;
    }
}

//Check if hit by bullet. Return status.
bool Enemy::Collide()
{
    for (auto b : bullets.GetBullets())
    {
        double bx = b->getX();
        double by = b->getY();
        //First check, bullet in vicinity of enemy, and originated from the player
        if (b->OriginPlayer() && bx < posX + 100 * size && bx > posX - 100 * size && by < posY + 100 * size && by > posY - 100 * size)
        {
            double dist = sqrt(pow(posX - 5 * sin(angle + M_PI) - bx, 2) + pow(posY + 5 * cos(angle + M_PI) - by, 2));
            //second check, bullet has entered the enemies hitbox. This enemy should die.
            if (dist < 20 * size + b->getSize())
            {
                bullets.DeleteBullet(b);
                return true;
            }
        }
    }
    return false;
}

void EnemyController::Reset()
{
    for (auto e : enemies)
        delete e;
    enemies.clear();
    wave = 0;
    trigger = 0;
}

void EnemyController::SpawnEnemy(double x, double y)
{
    Enemy *e = new Enemy(x, y, bullets);
    enemies.push_back(e);
}
void EnemyController::Tick(std::pair<double, double> center)
{
    if (enemies.size() <= trigger)
        SpawnWave(center);
    for (auto e : enemies)
        e->Tick(center);
}
void EnemyController::Draw(std::pair<double, double> center, double screenLeft, double screenTop)
{
    for (auto e : enemies)
        e->Draw(center, screenLeft, screenTop);
}
int EnemyController::Collide()
{
    int collisions = 0;
    for (auto e : enemies)
        if (e->Collide())
        {
            collisions++;
            enemies.erase(find(enemies.begin(), enemies.end(), e));
            delete e;
        }
    return collisions;
}

//Spawn waves based on two functions.
//They set a trigger condition, and the number of enemies that should be spawned this wave
void EnemyController::SpawnWave(std::pair<double, double> center)
{
    wave++;

    //MISC math model for spawning
    double LOG_DENOM = 1 + pow(M_E, (.6 * (3.6 - wave)));
    double TRAIL_CONST = .05 * pow(wave, 1.5);

    int target = floor(15 / LOG_DENOM + TRAIL_CONST);
    trigger = floor(4 / LOG_DENOM + TRAIL_CONST + 1);
    //end MISC math model

    while (enemies.size() < target)
    {
        double newX = center.first;
        double newY = center.second;
        while (abs(center.first - newX) < DISPLAY_WIDTH && abs(center.second - newY) < DISPLAY_HEIGHT)
        {
            newX = center.first + (rand() % (5 * DISPLAY_WIDTH)) - 2.5 * DISPLAY_WIDTH;
            newY = center.second + (rand() % (4 * DISPLAY_WIDTH)) - 2 * DISPLAY_WIDTH;
        }
        SpawnEnemy(newX, newY);
    }
}