#include <raylib.h>

#include "jet_fighter.hpp"
#include "raylib.hpp"
#include "util.hpp"

int main() {
  Raylib *r = new Raylib;
  JetFighter *g = new JetFighter;
  Vector2 winSize = GetWindowSize();

  while (!g->ShouldClose()) {
    if (IsWindowResized()) {
      Vector2 neew = GetWindowSize();
      g->Resize(winSize, neew);
      r->Resize(winSize, neew);
      winSize = neew;
    }

    g->Update();
    r->Update();

    BeginDrawing();
    g->Draw();
    r->Draw();
    EndDrawing();
  }

  delete g;
  delete r;
  return 0;
}
