#include "Bullet.h"
#include "Settings.h"
#include <allegro5/allegro_primitives.h>
#include "Colors.h"

Bullet::Bullet(double x, double y, double angle, bool player_bullet) : posX(x),
                                                                       posY(y),
                                                                       angle(angle),
                                                                       velocity(BULLET_SPEED),
                                                                       size(BULLET_SIZE),
                                                                       player_bullet(player_bullet) {}

void Bullet::Draw()
{
    al_draw_filled_circle(posX, posY, size, WHITE);
}

void Bullet::Tick()
{
    posX += sin(angle) * velocity;
    posY -= cos(angle) * velocity;
}

//When Bullets get too far away from the player, despawn them
bool Bullet::Delete(std::pair<double, double> center)
{
    return sqrt(pow(center.first - posX, 2) + pow(center.second - posY, 2)) > (DISPLAY_WIDTH + DISPLAY_HEIGHT * 2);
}

void BulletController::Reset()
{
    for (auto b : bullets)
        delete b;
    bullets.clear();
}

void BulletController::SpawnBullet(double x, double y, double angle, bool player_bullet)
{
    bullets.push_back(new Bullet(x, y, angle, player_bullet));
}

void BulletController::Draw()
{
    for (auto b : bullets)
        b->Draw();
}
void BulletController::Tick(std::pair<double, double> center)
{
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i]->Tick();
        //Check if any bullets have left the play area
        if (bullets[i]->Delete(center))
        {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }
    }
}

//Delete a specific bullet when it collides with an object.
void BulletController::DeleteBullet(Bullet *b)
{
    auto it = find(bullets.begin(), bullets.end(), b);
    if (it != bullets.end())
        bullets.erase(it);
    delete b;
}