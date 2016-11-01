#include "Game.h"

int main() {
    // Program entry point.
    Game game;

    while(!game.GetWindow()->IsDone()){
        game.Update();
        game.Render();
        game.LateUpdate(); // Calls restart clock and StateManager.ProcessRequests()
    }

    return 0;
}
