#include "neon/Game.h"

#include <array>
#include <string>

#include <raylib.h>

namespace {
Color Background{10, 16, 35, 255};
Color Panel{20, 29, 58, 255};
Color Cyan{75, 226, 232, 255};
Color Pink{255, 91, 145, 255};
Color Yellow{255, 215, 92, 255};

void drawCentered(const char* text, float y, int size, Color color) {
    DrawText(text, static_cast<int>(480.0F - MeasureText(text, size) * 0.5F), static_cast<int>(y), size, color);
}

void drawWorld(const neon::GameModel& game) {
    ClearBackground(Background);
    DrawRectangle(0, 0, 960, 56, Panel);
    DrawLine(0, 56, 960, 56, Color{48, 65, 108, 255});
    DrawText("HOUSE FLIPPER", 24, 16, 24, Cyan);
    DrawText(TextFormat("SCORE  %02d", game.score()), 790, 18, 20, Yellow);

    for (int i = 0; i < 12; ++i) {
        const int x = 40 + i * 83;
        const int y = 92 + (i % 3) * 137;
        DrawCircle(x, y, 1.5F, Color{42, 72, 111, 180});
    }

    const auto star = game.collectible();
    DrawCircleV({star.x, star.y}, 13.0F, Yellow);
    DrawCircleV({star.x, star.y}, 6.0F, Color{255, 245, 185, 255});

    for (const auto& enemy : game.enemies()) {
        DrawCircleV({enemy.position.x, enemy.position.y}, enemy.radius + 4.0F, Color{255, 91, 145, 65});
        DrawCircleV({enemy.position.x, enemy.position.y}, enemy.radius, Pink);
        DrawCircleV({enemy.position.x - 4.0F, enemy.position.y - 3.0F}, 3.0F, Color{255, 207, 225, 255});
    }

    const auto player = game.player();
    DrawCircleV({player.x, player.y}, 19.0F, Color{75, 226, 232, 65});
    DrawRectangle(static_cast<int>(player.x - 13), static_cast<int>(player.y - 13), 26, 26, Cyan);
    DrawRectangle(static_cast<int>(player.x - 7), static_cast<int>(player.y - 7), 14, 14, Color{220, 255, 255, 255});
}
}

int main() {
    InitWindow(960, 540, "House Flipper 2D");
    SetTargetFPS(60);
    neon::GameModel game;

    while (!WindowShouldClose()) {
        if (game.state() == neon::GameState::Title && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            game.start();
        }
        if (game.state() == neon::GameState::GameOver && IsKeyPressed(KEY_R)) {
            game.reset();
            game.start();
        }

        const Vector2 touch = GetTouchPosition(0);
        const bool touching = GetTouchPointCount() > 0;
        const bool touchLeft = touching && touch.x < 320.0F;
        const bool touchRight = touching && touch.x > 640.0F;
        const bool touchUp = touching && touch.y < 180.0F;
        const bool touchDown = touching && touch.y > 360.0F;
        std::array<bool, 4> movement{
            IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || touchLeft,
            IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || touchRight,
            IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || touchUp,
            IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || touchDown,
        };
        game.update(GetFrameTime(), movement);

        BeginDrawing();
        if (game.state() == neon::GameState::Title) {
            ClearBackground(Background);
            drawCentered("HOUSE FLIPPER", 125, 64, Cyan);
            drawCentered("Recolecta energía. Esquiva los drones.", 215, 24, Color{186, 203, 237, 255});
            drawCentered("WASD / FLECHAS para moverte", 298, 21, Yellow);
            drawCentered("Pulsa ENTER para comenzar", 358, 26, Pink);
            drawCentered("Un prototipo 2D en C++", 480, 18, Color{100, 124, 170, 255});
        } else {
            drawWorld(game);
            if (game.state() == neon::GameState::GameOver) {
                DrawRectangle(0, 0, 960, 540, Color{7, 10, 24, 190});
                drawCentered("SISTEMA DESTRUIDO", 176, 46, Pink);
                drawCentered(TextFormat("Puntuación final: %d", game.score()), 250, 26, Yellow);
                drawCentered("Pulsa R para reiniciar", 325, 24, Cyan);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
