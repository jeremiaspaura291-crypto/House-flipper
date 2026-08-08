#include "neon/Game.h"

#include <algorithm>
#include <cmath>

namespace neon {
namespace {
constexpr float PlayerSpeed = 270.0F;
constexpr float CollectibleSize = 18.0F;
constexpr float EnemySpawnInterval = 2.4F;
constexpr float Pi = 3.14159265358979323846F;
}

GameModel::GameModel(unsigned int seed) : rng_(seed) {
    reset();
}

void GameModel::reset() {
    state_ = GameState::Title;
    player_ = {Width * 0.5F, Height * 0.72F};
    score_ = 0;
    elapsed_ = 0.0F;
    enemyTimer_ = 0.0F;
    enemies_.clear();
    spawnCollectible();
}

void GameModel::start() {
    if (state_ == GameState::Title || state_ == GameState::GameOver) {
        state_ = GameState::Playing;
        if (state_ == GameState::Playing && enemies_.empty()) {
            addEnemy();
        }
    }
}

void GameModel::spawnCollectible() {
    std::uniform_real_distribution<float> x(50.0F, Width - 50.0F);
    std::uniform_real_distribution<float> y(80.0F, Height - 50.0F);
    collectible_ = {x(rng_), y(rng_)};
}

void GameModel::addEnemy() {
    std::uniform_real_distribution<float> x(32.0F, Width - 32.0F);
    std::uniform_real_distribution<float> speed(55.0F, 92.0F);
    enemies_.push_back({{x(rng_), -24.0F}, speed(rng_) + elapsed_ * 3.0F, 14.0F});
}

bool GameModel::overlaps(Rect a, Rect b) const noexcept {
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

void GameModel::endGame() {
    state_ = GameState::GameOver;
}

void GameModel::update(float dt, const std::array<bool, 4>& movement) {
    if (state_ != GameState::Playing) {
        return;
    }
    dt = std::clamp(dt, 0.0F, 0.05F);
    elapsed_ += dt;
    enemyTimer_ += dt;

    Vec2 direction{};
    if (movement[0]) direction.x -= 1.0F;
    if (movement[1]) direction.x += 1.0F;
    if (movement[2]) direction.y -= 1.0F;
    if (movement[3]) direction.y += 1.0F;
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0F) {
        direction.x /= length;
        direction.y /= length;
    }
    player_.x = std::clamp(player_.x + direction.x * PlayerSpeed * dt, 20.0F, Width - 20.0F);
    player_.y = std::clamp(player_.y + direction.y * PlayerSpeed * dt, 62.0F, Height - 20.0F);

    if (enemyTimer_ >= EnemySpawnInterval) {
        enemyTimer_ = 0.0F;
        addEnemy();
    }

    for (auto& enemy : enemies_) {
        const Vec2 delta{player_.x - enemy.position.x, player_.y - enemy.position.y};
        const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (distance > 0.01F) {
            enemy.position.x += delta.x / distance * enemy.speed * dt;
            enemy.position.y += delta.y / distance * enemy.speed * dt;
        }
        if (distance < enemy.radius + 13.0F) {
            endGame();
        }
    }

    if (overlaps(playerBounds(), {collectible_.x - CollectibleSize * 0.5F,
                                  collectible_.y - CollectibleSize * 0.5F,
                                  CollectibleSize, CollectibleSize})) {
        ++score_;
        spawnCollectible();
        if (score_ % 5 == 0) {
            addEnemy();
        }
    }
}

} // namespace neon
