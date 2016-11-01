// StateManager.cpp -- Implements the StateManager class.

#include "StateManager.h"


StateManager::StateManager(SharedContext* l_shared)
        : m_shared(l_shared)
{
    RegisterState<State_ChatTest>(StateType::ChatTest);
}

StateManager::~StateManager(){
    for (auto &itr : m_states){
        itr.second->OnDestroy();
        delete itr.second;
    }
}

void StateManager::Update(const sf::Time& l_time){
    if (m_states.empty()){ return; }
    // Check transcendence flag to determine whether the top state allows others to be updated.
    // The state or states that need to get updated then have their Update() methods invoked
    // with elapsed time passed as the argument (commonly known as delta time == change in time)
    if (m_states.back().second->IsTranscendent() && m_states.size() > 1){
        auto itr = m_states.end();
        while (itr != m_states.begin()){
            if (itr != m_states.end()){
                if (!itr->second->IsTranscendent()){
                    break;
                }
            }
            --itr;
        }
        for (; itr != m_states.end(); ++itr){
            itr->second->Update(l_time);
        }
    } else {
        m_states.back().second->Update(l_time);
    }
}

void StateManager::Draw(){
    if (m_states.empty()){ return; }
    if (m_states.back().second->IsTransparent() && m_states.size() > 1){
        // The current state IsTransparent and there are more than one state.
        // To correctly render transparent states, their Draw() methods must be called in a correct order.
        // Where the latest state on the stack is drawn on screen last.
        // Iterate through the states backwards until a state is found that is either not transparent or is the first state on the stack.
        auto itr = m_states.end();
        while (itr != m_states.begin()){
            if (itr != m_states.end()){
                if (!itr->second->IsTransparent()){
                    break;
                }
            }
            --itr;
        }
        // Calls Draw() for all states from the state found above, to the last state on the stack.
        // Rendering multiple states in correct order.
        for (; itr != m_states.end(); ++itr){
            itr->second->Draw();
        }
    }
        // If there is only one state on the stack or if the current state(m_states.back, "top of stack")
        // is not transparent, invoke it's draw method.
    else {
        m_states.back().second->Draw();
    }
}

SharedContext* StateManager::GetContext(){ return m_shared; }


// Returns true if a state of l_type is found.
// If such as state is not found, or if the state is about to be removed, it returns false.
bool StateManager::HasState(const StateType& l_type){
    for (auto itr = m_states.begin();
         itr != m_states.end(); ++itr)
    {
        if (itr->first == l_type){
            auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
            if (removed == m_toRemove.end()){ return true; }
            return false;
        }
    }
    return false;
}

void StateManager::ProcessRequests(){
    while (m_toRemove.begin() != m_toRemove.end()){
        RemoveState(*m_toRemove.begin());
        m_toRemove.erase(m_toRemove.begin());
    }
}

void StateManager::SwitchTo(const StateType& l_type){
    m_shared->m_eventManager->SetCurrentState(l_type); // Will modify an internal data member which keeps track of the games current state.
    for (auto itr = m_states.begin();
         itr != m_states.end(); ++itr)
    {
        // If the state is found:
        if (itr->first == l_type){
            m_states.back().second->Deactivate();       // Deactivate() current state.
            StateType tmp_type = itr->first;            // Two temp variables holds the state type
            BaseState* tmp_state = itr->second;         // and the pointer to a state object, so we not loose it's information when we:
            m_states.erase(itr);                        // Erase the state from the vector. (After this operation all the iterators to the container are invalidated, but we don't need them anymore.)
            m_states.emplace_back(tmp_type, tmp_state); // Place it back on top of the stack.
            tmp_state->Activate();                      // We then Activate() the state.
            return;
        }
    }

    // State with l_type wasn't found.
    if (!m_states.empty()){ m_states.back().second->Deactivate(); }
    CreateState(l_type);
    m_states.back().second->Activate();
}

// Adds a state to be removed, which is later processed by ProcessRequests(), which again calls RemoveState.
void StateManager::Remove(const StateType& l_type){
    m_toRemove.push_back(l_type);
}

// Private methods.

void StateManager::CreateState(const StateType& l_type){
    auto newState = m_stateFactory.find(l_type);
    if (newState == m_stateFactory.end()) // If the state cannot be created with m_stateFactory()
        return;
    BaseState* state = newState->second();
    m_states.emplace_back(l_type, state);
    state->OnCreate();
}

// Takes care of the actual removal of states, including resource de-allocation.
void StateManager::RemoveState(const StateType& l_type){
    for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
    {
        if (itr->first == l_type){
            itr->second->OnDestroy();
            delete itr->second;
            m_states.erase(itr);
            return;
        }
    }
}
