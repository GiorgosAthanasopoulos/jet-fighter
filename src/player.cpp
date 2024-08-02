#include <cmath>
#include <raylib.h>
#include <rlgl.h>

#include "config.hpp"
#include "player.hpp"
#include "player_config.hpp"
#include "util.hpp"

Player::Player(PlayerConfig pc) {
  this->pc = pc;
  Resize(GetWindowSize(), GetWindowSize());
  Reset();
}

Player::~Player() {}

std::optional<Bullet *> Player::Update() {
  HandleFlash();
  HandleMovement();
  if (flash.flashing) {
    return {};
  }

  if (auto b = HandleShooting()) {
    return *b;
  }

  return {};
}

void Player::Draw(Texture2D tex) {
  if (!flash.visible) {
    return;
  }
  rlPushMatrix();
  rlTranslatef(pos.x + size.x / 2, pos.y + size.y / 2, 0);
  rlRotatef(angle, 0, 0, 1);
  rlScalef(2, 2, 0);
  rlTranslatef(-tex.width / 2.0f, -tex.height / 2.0f, 0);
  DrawTexture(tex, 0, 0, WHITE);
  rlPopMatrix();
  if (DEBUG) {
    DrawRectangleLinesEx({pos.x, pos.y, size.x, size.y}, DEBUG_HTBX_THICK,
                         DEBUG_HTBX_COLOR);
  }
}

void Player::Resize(Vector2 old, Vector2 neew) {
  speed = CalculateSpeed(PLAYER_SPEED_RATIO);
  size = CalculateSize(PLAYER_SIZE_RATIO);
  pos = MaintainPosAfterResize(old, neew, pos);
}

Rectangle Player::GetRec() { return {pos.x, pos.y, size.x, size.y}; }

void Player::Reset() {
  flash.flashing = true;
  flash.visible = false;
  flash.flashTime = 0;
  flash.initialHideTime = FLASH_INITIAL_HIDE_TIME;

  pos = {GetWindowWidth() * (pc.playerLeft ? 1 : 2) / 3,
         GetWindowHeight() - 2 * size.y};
  shootTimer = 0;
  angle = 0;
}

void Player::HandleFlash() {
  float frameTime = GetFrameTime();

  if (flash.flashing) {
    if (flash.initialHideTime > 0.0f) {
      flash.initialHideTime -= frameTime;
    } else {
      flash.flashTime += frameTime;
    }
    flash.visible =
        fmod(flash.flashTime, 0.1f) < 0.05f && flash.initialHideTime <= 0.0f;
    if (flash.flashTime >= FLASH_TIME) {
      flash.flashing = false;
      flash.visible = true;
    }
  }
}

void Player::HandleMovement() {
  Vector2 vel = {0, 0};

  if (IsKeyDown(pc.kU) && IsKeyDown(pc.kD)) {
    vel = {0, 0};
  } else if (IsKeyDown(pc.kU)) {
    vel.x += sinf(DEG2RAD * angle) * speed.x;
    vel.y -= cosf(DEG2RAD * angle) * speed.y;
  } else if (IsKeyDown(pc.kD)) {
    vel.x -= std::sinf(DEG2RAD * angle) * speed.x;
    vel.y += std::cosf(DEG2RAD * angle) * speed.y;
  }

  if (IsKeyDown(pc.kL) && !IsKeyDown(pc.kR)) {
    angle -= TURN_ANGLE_INTERVAL;
  }
  if (IsKeyDown(pc.kR) && !IsKeyDown(pc.kL)) {
    angle += TURN_ANGLE_INTERVAL;
  }

  pos += vel * GetFrameTime();
  pos = WorldBoundaryCheck(pos, size);
}

std::optional<Bullet *> Player::HandleShooting() {
  shootTimer -= GetFrameTime();

  if (IsKeyPressed(pc.kShoot) && shootTimer <= 0.0f) {
    shootTimer = SHOOT_TIMER;
    return new Bullet(pos + size / 2, angle, pc.color, pc.playerLeft ? P1 : P2);
  }

  return {};
}

bool Player::GetInvincible() { return flash.flashing; }
