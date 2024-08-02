#pragma once

#include <raylib.h>

class AssetManager {
public:
  Texture2D explosionTex, ship1, ship2;
  Music bgm;
  Sound explosion;

  AssetManager();
  ~AssetManager();
};
