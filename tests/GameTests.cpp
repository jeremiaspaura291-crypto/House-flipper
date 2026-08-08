#include "neon/Game.h"

#include <array>
#include <cassert>
#include <iostream>

int main() {
    neon::GameModel game(42U);
    assert(game.state() == neon::GameState::Title);
    assert(game.score() == 0);

    game.start();
    assert(game.state() == neon::GameState::Playing);
    assert(!game.enemies().empty());

    const auto start = game.player();
    game.update(0.1F, {false, true, false, false});
    assert(game.player().x > start.x);

    game.update(1.0F, {false, false, true, false});
    assert(game.player().y < start.y + 10.0F);

    game.reset();
    assert(game.state() == neon::GameState::Title);
    assert(game.elapsed() == 0.0F);

    std::cout << "neon_tests: all checks passed\n";
    return 0;
}
