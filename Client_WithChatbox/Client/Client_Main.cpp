#include "Game.h"



// Main loop of chat client.
int main(int argc, char** argv){

    /// Choose between using (1):server info from console and (2): Static server info.
    // In the OnCreate method in State_ChatTest.cpp
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