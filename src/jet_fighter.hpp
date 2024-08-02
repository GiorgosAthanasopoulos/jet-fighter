#pragma once

#include <raylib.h>
#include <vector>

#include "asset_manager.hpp"
#include "bullet.hpp"
#include "explosion.hpp"
#include "player.hpp"

class JetFighter {
public:
  JetFighter();
  ~JetFighter();
  bool ShouldClose();
  void Update();
  void Draw();
  void Resize(Vector2 old, Vector2 neew);

private:
  Vector2 winSize;
  bool paused;
  enum { P1, P2, DRAW, NONE } winner;
  int score1, score2;
  float gameTimer;
  Player p1, p2;
  AssetManager am;
  std::vector<Explosion> explosions;
  std::vector<Bullet *> bullets;

  void Restart();

  void HandleMusic();
  void HandlePause();
  void HandleWin();
  void HandlePlayers();
  void HandleTimers();
  void HandleExplosions();
  void HandleBullets();

  void DrawPause();
  void DrawWin();
  void DrawEntities();
  void DrawTimer();
  void DrawScores();

  void RegisterHit(int bulletIndex, PlayerId killed);
};
