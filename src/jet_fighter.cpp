#include <raylib.h>
#include <sstream>
#include <string>

#include "config.hpp"
#include "jet_fighter.hpp"
#include "player_id.hpp"
#include "util.hpp"

JetFighter::JetFighter()
    : p1({KEY_P1_MOVE_UP, KEY_P1_MOVE_DOWN, KEY_P1_MOVE_LEFT, KEY_P1_MOVE_RIGHT,
          KEY_P1_SHOOT, true, WHITE}),
      p2({KEY_P2_MOVE_UP, KEY_P2_MOVE_DOWN, KEY_P2_MOVE_LEFT, KEY_P2_MOVE_RIGHT,
          KEY_P2_SHOOT, false, BLACK}) {
  winSize = GetWindowSize();
  PlayMusicStream(am.bgm);
  Restart();
}

JetFighter::~JetFighter() {
  for (size_t i = 0; i < bullets.size(); ++i) {
    delete bullets[i];
  }
}

bool JetFighter::ShouldClose() { return WindowShouldClose(); }

void JetFighter::Update() {
  HandleMusic();
  HandlePause();
  if (paused) {
    return;
  }
  if (winner != NONE) {
    HandleWin();
    return;
  }
  HandlePlayers();
  HandleTimers();
  HandleExplosions();
  HandleBullets();
}

void JetFighter::Draw() {
  ClearBackground(WIN_BG);
  if (paused) {
    DrawPause();
    return;
  }
  if (winner != NONE) {
    DrawWin();
    return;
  }
  DrawEntities();
  DrawTimer();
  DrawScores();
}

void JetFighter::Resize(Vector2 old, Vector2 neew) {
  p1.Resize(old, neew);
  p2.Resize(old, neew);
  for (size_t i = 0; i < explosions.size(); ++i) {
    explosions[i].Resize(old, neew);
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i]->Resize(old, neew);
  }
}

void JetFighter::Restart() {
  paused = false;
  winner = NONE;
  score1 = score2 = 0;
  gameTimer = GAME_TIME;
  p1.Reset();
  p2.Reset();
  explosions.clear();
  bullets.clear();
}

void JetFighter::HandleMusic() {
  if (IsKeyPressed(KEY_MUTE)) {
    if (IsMusicStreamPlaying(am.bgm)) {
      PauseMusicStream(am.bgm);
    } else {
      ResumeMusicStream(am.bgm);
    }
  }
  UpdateMusicStream(am.bgm);
}

void JetFighter::HandlePause() {
  if (IsKeyPressed(KEY_PAUSE)) {
    paused = !paused;
  }
}

void JetFighter::HandleWin() {
  if (IsKeyPressed(KEY_RESTART)) {
    Restart();
  }
}

void JetFighter::HandlePlayers() {
  if (auto b = p1.Update()) {
    bullets.push_back(*b);
  }
  if (auto b = p2.Update()) {
    bullets.push_back(*b);
  }
}

void JetFighter::HandleTimers() {
  gameTimer -= GetFrameTime();
  if (gameTimer <= 0.0f) {
    winner = score1 > score2 ? P1 : score1 == score2 ? DRAW : P2;
  }
}

void JetFighter::DrawPause() {
  const char *t = "Paused";
  Vector2 tSize = AssertTextFitsInViewport(t, LARGE_FONT_SIZE, winSize / 2);
  DrawText(t, winSize.x / 2 - tSize.x / 2, winSize.y / 2 - tSize.y / 2, tSize.y,
           PAUSED_TEXT_COLOR);
}

void JetFighter::DrawWin() {
  const char *t = winner == P1   ? "Winner: P1"
                  : winner == P2 ? "Winner: P2"
                                 : "Winner: Draw";
  Vector2 tSize = AssertTextFitsInViewport(t, LARGE_FONT_SIZE, winSize / 2);
  DrawText(t, winSize.x / 2 - tSize.x / 2, winSize.y / 2 - tSize.y / 2, tSize.y,
           WINNER_TEXT_COLOR);
}

void JetFighter::DrawEntities() {
  p1.Draw(am.ship1);
  p2.Draw(am.ship2);
  for (size_t i = 0; i < explosions.size(); ++i) {
    explosions[i].Draw(am.explosionTex);
  }
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i]->Draw();
  }
}

void JetFighter::DrawTimer() {
  float timeRemaining = gameTimer / GAME_TIME;
  DrawRectangle(0, winSize.y - TIMER_H, timeRemaining * winSize.x, TIMER_H,
                TIMER_COLOR);
}

void JetFighter::DrawScores() {
  std::ostringstream ss1;
  ss1 << score1;
  std::string s1 = ss1.str();
  const char *t1 = s1.data();
  Vector2 maxSize = winSize / SCORE_TEXT_SIZE_RATIO;
  Vector2 t1Size = AssertTextFitsInViewport(t1, FONT_SIZE, maxSize);
  DrawText(t1, 0, winSize.y - t1Size.y - SCORE_TEXT_PADDING, t1Size.y,
           SCORE_TEXT_COLOR);

  std::ostringstream ss2;
  ss2 << score2;
  std::string s2 = ss2.str();
  const char *t2 = s2.data();
  Vector2 t2Size = AssertTextFitsInViewport(t2, FONT_SIZE, maxSize);
  DrawText(t2, winSize.x - t2Size.x - SCORE_TEXT_PADDING,
           winSize.y - t2Size.y - SCORE_TEXT_PADDING, t2Size.y,
           SCORE_TEXT_COLOR);
}

void JetFighter::HandleExplosions() {
  for (size_t i = 0; i < explosions.size(); ++i) {
    explosions[i].Update();

    if (explosions[i].GetLifetime() <= 0.0f) {
      explosions.erase(explosions.begin() + i);
    }
  }
}

void JetFighter::HandleBullets() {
  for (size_t i = 0; i < bullets.size(); ++i) {
    bullets[i]->Update();

    Rectangle bulletRec = bullets[i]->GetRec();
    if (CheckCollisionRecs(bulletRec, p1.GetRec()) &&
        bullets[i]->GetOwner() != PlayerId::P1 && !p1.GetInvincible()) {
      RegisterHit(i, PlayerId::P1);
    }
    if (CheckCollisionRecs(bulletRec, p2.GetRec()) &&
        bullets[i]->GetOwner() != PlayerId::P2 && !p2.GetInvincible()) {
      RegisterHit(i, PlayerId::P2);
    }
  }
}

void JetFighter::RegisterHit(int bulletIndex, PlayerId killed) {
  Rectangle collRec =
      GetCollisionRec(bullets[bulletIndex]->GetRec(),
                      killed == PlayerId::P1 ? p1.GetRec() : p2.GetRec());
  explosions.push_back(Explosion({collRec.x, collRec.y}));

  PlaySound(am.explosion);

  if (killed == PlayerId::P1) {
    score2++;
    p1.Reset();
  } else {
    score1++;
    p2.Reset();
  }

  delete bullets[bulletIndex];
  bullets.erase(bullets.begin() + bulletIndex);
}
