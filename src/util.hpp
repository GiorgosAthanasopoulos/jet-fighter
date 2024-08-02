#pragma once

#include <raylib.h>

float GetWindowWidth();
float GetWindowHeight();
Vector2 GetWindowSize();

Vector2 operator/(Vector2 a, float b);
void operator+=(Vector2 &a, float b);
void operator-=(Vector2 &a, float b);
bool operator<(Vector2 a, float b);
bool operator>(Vector2 a, Vector2 b);
Vector2 operator/(Vector2 a, Vector2 b);
Vector2 operator*(Vector2 a, Vector2 b);
Vector2 operator*(Vector2 a, float b);
void operator+=(Vector2 &a, Vector2 b);
Vector2 operator+(Vector2 a, Vector2 b);

Vector2 AssertTextFitsInViewport(const char *text, int fontSize,
                                 Vector2 maxSize);

Vector2 CalculateSize(float sizeToWinSizeRatio);
Vector2 CalculateSpeed(float speedToWinSizeRaTio);
Vector2 MaintainPosAfterResize(Vector2 old, Vector2 neew, Vector2 pos);

Vector2 WorldBoundaryCheck(Vector2 pos, Vector2 size);
