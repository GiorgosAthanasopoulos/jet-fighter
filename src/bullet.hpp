#pragma once

#include <raylib.h>

#include "player_id.hpp"

class Bullet {
public:
  Bullet(Vector2 pos, float angle, Color color, PlayerId owner);
  ~Bullet();
  void Update();
  void Draw();
  void Resize(Vector2 old, Vector2 neew);
  Rectangle GetRec();
  PlayerId GetOwner();

private:
  PlayerId owner;
  Vector2 pos, size, speed;
  float angle;
  Color color;
};
