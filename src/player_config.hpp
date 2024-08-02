#pragma once

#include <raylib.h>

struct PlayerConfig {
  KeyboardKey kU, kD, kL, kR, kShoot;
  bool playerLeft;
  Color color;
};
