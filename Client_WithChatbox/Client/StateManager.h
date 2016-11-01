

#ifndef CLIENT_WITHCHATBOX_STATEMANAGER_H
#define CLIENT_WITHCHATBOX_STATEMANAGER_H


#include <vector>
#include <unordered_map>
#include "State_ChatTest.h"
#include "SharedContext.h"

// Enumerating all possible states.
enum class StateType{ ChatTest = 1 };

// State container.
using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
// Type container.
using TypeContainer = std::vector<StateType>;
// State factory
// "We're using an unordered map here in order to map a specific state type to a specific function that will generate that type."
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager{
public:
    StateManager(SharedContext* l_shared);
    ~StateManager();

    // Has Update and Draw methods, "because it will be operated by the Game class", it's nice to keep the interface familiar.
    void Update(const sf::Time& l_time);
    void Draw();

    void ProcessRequests();

    // Helper methods for obtaining the context, as well as
    // determining if it currently has a certain state on the stack.
    SharedContext* GetContext();
    bool HasState(const StateType& l_type);

    void SwitchTo(const StateType& l_type); // Takes a StateType and changes the current state to one that corresponds to said type.
    void Remove(const StateType& l_type);   // Removes a state from the state stack by it's type.
private:
    // Methods.
    void CreateState(const StateType& l_type);
    void RemoveState(const StateType& l_type);

    // Maps the StateType l_type in the m_stateFactory map to a function that returns a pointer to newly allocated memory.
    // Because each state requires a pointer to the StateManager class in its constructor, we pass the 'this pointer' in.
    template<class T>
    void RegisterState(const StateType& l_type){
        // The map links the StateType to a std::function type, which can be set
        // to hold a body of a function through use of the lambda expression.
        m_stateFactory[l_type] = [this]() -> BaseState* // [Capture the 'this pointer'] -> specifying return type {execute};
        {
            return new T(this); // Return new someStateType(pointer to StateManager);
        };
    }

    // Members.
    SharedContext* m_shared;
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_stateFactory;
};


#endif //CLIENT_WITHCHATBOX_STATEMANAGER_H
