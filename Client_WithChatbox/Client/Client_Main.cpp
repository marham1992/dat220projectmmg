#include "Game.h"



// Main loop of chat client.
int main(int argc, char** argv){

    // Program entry point.
    Game game;
    while(!game.GetWindow()->IsDone()){
        game.Update();
        game.Render();
        game.LateUpdate(); // Calls restart clock and StateManager.ProcessRequests()
    }


//    std::cout << "Quitting..." << std::endl;
//    sf::sleep(sf::seconds(1.f));

    return 0;
}