#include <cmath>
#include <raylib.h>

#include "bullet.hpp"
#include "config.hpp"
#include "player_id.hpp"
#include "util.hpp"

Bullet::Bullet(Vector2 pos, float angle, Color color, PlayerId owner) {
  this->pos = pos;
  this->angle = angle;
  this->color = color;
  this->owner = owner;
  Resize(GetWindowSize(), GetWindowSize());
}

Bullet::~Bullet() {}

void Bullet::Update() {
  Vector2 vel;
  vel.x = std::sin(angle * DEG2RAD) * speed.x;
  vel.y = -std::cos(angle * DEG2RAD) * speed.y;
  pos += vel * GetFrameTime();
}

void Bullet::Draw() {
  DrawCircleV(pos, size.x / 2, color);
  if (DEBUG) {
    Rectangle bulletRec = GetRec();
    DrawRectangleLinesEx(bulletRec, DEBUG_HTBX_THICK, DEBUG_HTBX_COLOR);
  }
}

void Bullet::Resize(Vector2 old, Vector2 neew) {
  speed = CalculateSpeed(BULLET_SPEED_RATIO);
  size = CalculateSize(BULLET_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, neew, pos);
}

Rectangle Bullet::GetRec() {
  return {pos.x - size.x / 2, pos.y - size.y / 2, size.x, size.y};
}

PlayerId Bullet::GetOwner() { return owner; }
