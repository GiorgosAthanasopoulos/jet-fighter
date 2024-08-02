#include <raylib.h>

#include "util.hpp"

float GetWindowWidth() { return GetRenderWidth(); }
float GetWindowHeight() { return GetRenderHeight(); }
Vector2 GetWindowSize() { return {GetWindowWidth(), GetWindowHeight()}; }

Vector2 operator/(Vector2 a, float b) { return {a.x / b, a.y / b}; }
void operator+=(Vector2 &a, float b) {
  a.x += b;
  a.y += b;
}
void operator-=(Vector2 &a, float b) {
  a.x -= b;
  a.y -= b;
}
bool operator<(Vector2 a, float b) { return a.x < b || a.y < b; }
bool operator>(Vector2 a, Vector2 b) { return a.x > b.x || a.y > b.x; }
Vector2 operator/(Vector2 a, Vector2 b) { return {a.x / b.x, a.y / b.y}; }
Vector2 operator*(Vector2 a, Vector2 b) { return {a.x * b.x, a.y * b.y}; }
Vector2 operator*(Vector2 a, float b) { return {a.x * b, a.y * b}; }
void operator+=(Vector2 &a, Vector2 b) {
  a.x += b.x;
  a.y += b.y;
}
Vector2 operator+(Vector2 a, Vector2 b) { return {a.x + b.x, a.y + b.y}; }

Vector2 AssertTextFitsInViewport(const char *text, int fontSize,
                                 Vector2 maxSize) {
  float textW = MeasureText(text, fontSize);

  while (textW > maxSize.x || fontSize > maxSize.y) {
    fontSize--;
    textW = MeasureText(text, fontSize);
  }

  return {textW, (float)fontSize};
}

Vector2 CalculateSize(float sizeToWinSizeRatio) {
  return GetWindowSize() / sizeToWinSizeRatio;
}
Vector2 CalculateSpeed(float speedToWinSizeRaTio) {
  return GetWindowSize() / speedToWinSizeRaTio;
}
Vector2 MaintainPosAfterResize(Vector2 old, Vector2 neew, Vector2 pos) {
  return pos / old * neew;
}

Vector2 WorldBoundaryCheck(Vector2 pos, Vector2 size) {
  if (pos.x < 0) {
    pos.x = 0;
  }
  if (pos.y < 0) {
    pos.y = 0;
  }
  if (pos.x > GetWindowWidth() - size.x) {
    pos.x = GetWindowWidth() - size.x;
  }
  if (pos.y > GetWindowHeight() - size.y) {
    pos.y = GetWindowHeight() - size.y;
  }
  return pos;
}
