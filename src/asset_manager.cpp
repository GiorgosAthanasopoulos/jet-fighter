#include <raylib.h>

#include "asset_manager.hpp"
#include "config.hpp"

AssetManager::AssetManager() {
  explosionTex = LoadTexture("assets/explosion.png");
  ship1 = LoadTexture("assets/ship_1.png");
  ship2 = LoadTexture("assets/ship_2.png");

  bgm = LoadMusicStream("assets/bgm.mp3");
  SetMusicVolume(bgm, MUSIC_VOLUME);

  explosion = LoadSound("assets/explosion.mp3");
  SetSoundVolume(explosion, SOUND_VOLUME);
}

AssetManager::~AssetManager() {
  UnloadTexture(explosionTex);
  UnloadTexture(ship1);
  UnloadTexture(ship2);

  UnloadMusicStream(bgm);

  UnloadSound(explosion);
}
