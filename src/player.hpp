#pragma once

#include <optional>
#include <raylib.h>

#include "bullet.hpp"
#include "flash.hpp"
#include "player_config.hpp"

class Player {
public:
  Player(PlayerConfig pc);
  ~Player();
  std::optional<Bullet *> Update();
  void Draw(Texture2D tex);
  void Resize(Vector2 old, Vector2 neew);
  Rectangle GetRec();
  void Reset();
  bool GetInvincible();

private:
  Vector2 pos, size, speed;
  float angle;
  PlayerConfig pc;
  float shootTimer;
  Flash flash;

  void HandleFlash();
  void HandleMovement();
  std::optional<Bullet *> HandleShooting();
};
