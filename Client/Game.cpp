#include <stdlib.h>
#include "Game.h"

// Create window, create context and create StateManager(passing it the context)
Game::Game(): m_window("MMMG", sf::Vector2u(800, 600)), m_stateManager(&m_context){
    m_clock.restart();
    srand(time(nullptr));

    // Setting the Window and EventManager for the shared context.
    m_context.m_wind = &m_window;
    // It seems to me: Window creates the EventManager in it's Setup method while adding callbacks.
    m_context.m_eventManager = m_window.GetEventManager();
    // Begin the 'domino effect' by switching to the introduction state, which in due time will switch to other states.
    m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game(){ }

sf::Time Game::GetElapsed(){ return m_clock.getElapsedTime(); }
void Game::RestartClock(){ m_elapsed = m_clock.restart(); }
Window* Game::GetWindow(){ return &m_window; }

void Game::Update(){
    m_window.Update();
    m_stateManager.Update(m_elapsed);
}

void Game::Render(){
    m_window.BeginDraw();
    // Render here.
    m_stateManager.Draw(); // StateManager draws states in correct order.
    m_window.EndDraw();
}

void Game::LateUpdate(){
    m_stateManager.ProcessRequests(); // Currently just handles removal of states from the stack.
    RestartClock();
}