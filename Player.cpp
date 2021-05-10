#include "Player.h"
#include "Settings.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "KeyTracker.h"
#include "Bullet.h"
#include "Colors.h"

Player::Player(std::pair<int, int> dims, BulletController &bullets) : posX(dims.first / 2),
                                                                      posY(dims.second / 2),
                                                                      frameX(posX), frameY(posY),
                                                                      velocity(0), angle(0), size(PLAYER_SIZE),
                                                                      cooldown(0), fire_rate(PLAYER_FIRE_RATE),
                                                                      dims(dims),
                                                                      bullets(bullets),
                                                                      HP(PLAYER_HP),
                                                                      maxHP(PLAYER_HP) {}

void Player::Reset()
{
    posX = dims.first / 2;
    posY = dims.second / 2;
    frameX = posX, frameY = posY;
    velocity = 0, angle = 0;
    HP = PLAYER_HP;
}

std::pair<double, double> Player::Draw()
{
    if (DEBUG)
        DrawLocalGrid();

    //Move the center to the player
    ALLEGRO_TRANSFORM trans;
    al_build_transform(&trans, frameX, frameY, 1, 1, angle);
    al_use_transform(&trans);

    //Draw the player
    al_draw_filled_triangle(0, 0, 10 * size, 10 * size, 0, 8 * size, BLACK);
    al_draw_filled_triangle(0, 0, 0, 8 * size, -10 * size, 10 * size, BLACK);
    al_draw_filled_triangle(-15 * size, 5 * size, 0, -10 * size, 0, 1 * size, BLACK);
    al_draw_filled_triangle(+15 * size, 5 * size, 0, -10 * size, 0, 1 * size, BLACK);
    al_draw_filled_rectangle(-4 * size, -10 * size, +4 * size, +8 * size, BLACK);
    al_draw_filled_triangle(-4 * size, -10 * size, 0, -15 * size, 4 * size, -10 * size, BLACK);

    //Move back to the absolute reference frame
    al_build_transform(&trans, frameX - posX, frameY - posY, 1, 1, 0);
    al_use_transform(&trans);

    //Draw healthbar
    al_draw_filled_rectangle(posX - 30, posY + 40, posX + 30, posY + 50, RED);
    al_draw_filled_rectangle(posX - 30, posY + 40, posX - 30 + (double(HP) / maxHP) * 60, posY + 50, GREEN);

    if (DEBUG)
        DrawHitbox();

    return std::make_pair(posX - frameX, posY - frameY);
}

void Player::DrawHitbox()
{
    al_draw_circle(posX, posY, 10 * size, WHITE, 1);
}

void Player::Tick()
{
    //Set a to range [b,c]
    auto clamp = [](double &a, double b, double c) {
        a = (a < b) ? b : ((a > c) ? c : a);
        return a;
    };

    if (KeyTracker::get().IsDown(ALLEGRO_KEY_RIGHT))
        angle += PLAYER_ANGLE_CHANGE;
    if (KeyTracker::get().IsDown(ALLEGRO_KEY_LEFT))
        angle -= PLAYER_ANGLE_CHANGE;
    if (KeyTracker::get().IsDown(ALLEGRO_KEY_UP))
        velocity += PLAYER_VELOCITY_CHANGE;
    else
        velocity -= PLAYER_VELOCITY_CHANGE * .6;

    clamp(velocity, 0.0, PLAYER_MAX_VELOCITY);

    //track absolute position, and where the player is in the frame.
    posX += velocity * sin(angle);
    posY -= velocity * cos(angle);

    frameX += velocity * sin(angle);
    frameY -= velocity * cos(angle);

    clamp(frameX, dims.first / 2.0 - 250, dims.first / 2.0 + 250);
    clamp(frameY, dims.second / 2.0 - 200, dims.second / 2.0 + 200);

    if (KeyTracker::get().IsDown(ALLEGRO_KEY_SPACE))
        Fire();
}

void Player::DrawLocalGrid()
{
    double xOffset = fmod(frameX - posX, 100) - 100;
    double yOffset = fmod(frameY - posY, 100) - 100;

    for (double i = xOffset; i <= dims.first; i += 100)
        al_draw_line(i, 0, i, dims.second, BLACK, 1);
    for (double i = yOffset; i <= dims.second; i += 100)
        al_draw_line(0, i, dims.first, i, BLACK, 1);
}

void Player::Fire()
{
    cooldown--;
    if (cooldown < 0)
        cooldown = 0;
    if (cooldown == 0)
    {
        bullets.SpawnBullet(posX + sin(angle) * 10 * size, posY - cos(angle) * 10 * size, angle, true);
        cooldown = fire_rate;
    }
}

bool Player::Collide()
{
    for (auto b : bullets.GetBullets())
    {
        double bx = b->getX();
        double by = b->getY();
        if (bx < posX + 100 * size && bx > posX - 100 * size && by < posY + 100 * size && by > posY - 100 * size)
        {
            double dist = sqrt(pow(posX - bx, 2) + pow(posY - by, 2));
            if (dist < 10 * size + b->getSize())
            {
                bullets.DeleteBullet(b);
                if (!INVINCIBLE)
                    HP--;
                return true;
            }
        }
    }
    return false;
}