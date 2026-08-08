#pragma once

#include <array>
#include <cstddef>
#include <random>
#include <vector>

namespace neon {

struct Vec2 {
    float x{};
    float y{};
};

struct Rect {
    float x{};
    float y{};
    float w{};
    float h{};
};

enum class GameState { Title, Playing, GameOver };

enum class Action { None, Left, Right, Up, Down, Start, Restart };

struct Enemy {
    Vec2 position{};
    float speed{};
    float radius{14.0F};
};

class GameModel {
public:
    static constexpr float Width = 960.0F;
    static constexpr float Height = 540.0F;

    explicit GameModel(unsigned int seed = 1337U);

    void reset();
    void update(float dt, const std::array<bool, 4>& movement);
    void start();

    [[nodiscard]] GameState state() const noexcept { return state_; }
    [[nodiscard]] Vec2 player() const noexcept { return player_; }
    [[nodiscard]] Vec2 collectible() const noexcept { return collectible_; }
    [[nodiscard]] const std::vector<Enemy>& enemies() const noexcept { return enemies_; }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] float elapsed() const noexcept { return elapsed_; }
    [[nodiscard]] Rect playerBounds() const noexcept { return {player_.x - 13.0F, player_.y - 13.0F, 26.0F, 26.0F}; }

private:
    void spawnCollectible();
    void addEnemy();
    void endGame();
    [[nodiscard]] bool overlaps(Rect a, Rect b) const noexcept;

    GameState state_{GameState::Title};
    Vec2 player_{};
    Vec2 collectible_{};
    std::vector<Enemy> enemies_{};
    int score_{};
    float elapsed_{};
    float enemyTimer_{};
    std::mt19937 rng_;
};

} // namespace neon
